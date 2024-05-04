#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "grid.h"
#include "hash.h"
#include "queue.h"
#include "raylib.h"

KDQ_INIT(Vector2)
KHASH_SET_INIT_INT(i32)

static int *_GRID;
static int *_VISIBLE_GRID;
static int SIZEM;
static int SIZEN;

const int *GRID;
const int *VISIBLE_GRID;

static const int DI[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
static const int DJ[8] = {0, 1, 1, 1, 0, -1, -1, -1};

static void generate_mines(int nmines);
static void count_mines(void);
static void clear_grids(void);

int grid_init(int m, int n, int nmines)
{
    _GRID = malloc(m * n * sizeof(int));
    _VISIBLE_GRID = malloc(m * n * sizeof(int));
    SIZEM = m;
    SIZEN = n;

    if (!_GRID || !_VISIBLE_GRID)
        return -1;
    if (m * n < nmines)
        return -2;

    SetRandomSeed(time(NULL));
    clear_grids();
    generate_mines(nmines);
    count_mines();

    GRID = _GRID;
    VISIBLE_GRID = _VISIBLE_GRID;

    return 0;
}

void grid_destroy(void)
{
    free(_GRID);
    free(_VISIBLE_GRID);
    _GRID = NULL;
    _VISIBLE_GRID = NULL;
    GRID = NULL;
    VISIBLE_GRID = NULL;
}

int open_cell(int i, int j)
{
    if (i < 0 || i >= SIZEM || j < 0 || j > SIZEN)
        return 0;

    int idx = i * SIZEN + j;
    if (_VISIBLE_GRID[idx] >= C_EMPTY)
        return 0;

    if (_GRID[idx] >= C_EMPTY) {
        _VISIBLE_GRID[idx] = _GRID[idx];
        return 1;
    } else if (_GRID[idx] == C_MINE && _VISIBLE_GRID[idx] != C_FLAG)
        _VISIBLE_GRID[idx] = C_BLAST;
    return 0;
}

void open_mines(void)
{
    for (int idx = 0; idx < SIZEM * SIZEN; idx++) {
        if (_GRID[idx] == C_MINE
            && !(_VISIBLE_GRID[idx] == C_BLAST
                 || _VISIBLE_GRID[idx] == C_FLAG)) {
            _VISIBLE_GRID[idx] = _GRID[idx];
        }
    }
}

void open_safe_cells(void)
{
    for (int idx = 0; idx < SIZEM * SIZEN; idx++)
        if (_GRID[idx] != C_MINE)
            _VISIBLE_GRID[idx] = _GRID[idx];
}

void toggle_flag(int i, int j)
{
    if (i < 0 || i >= SIZEM || j < 0 || j > SIZEN)
        return;

    int val = _VISIBLE_GRID[i * SIZEN + j];
    _VISIBLE_GRID[i * SIZEN + j] = val == C_FLAG ? C_CLOSED : C_FLAG;
}

int expand(int i, int j)
{
    if (i < 0 || i >= SIZEM || j < 0 || j > SIZEN)
        return 0;

    int ret;
    int nopened = 0;
    Vector2 *curv;
    kdq_Vector2_t *queue = kdq_init_Vector2();
    kh_i32_t *set = kh_init_i32();

    kdq_push_Vector2(queue, (Vector2) {i, j});

    while ((curv = kdq_pop_Vector2(queue)) != NULL) {
        int i = (int)curv->x;
        int j = (int)curv->y;
        kh_put_i32(set, (i + 1) * 100 + j, &ret);
        nopened += open_cell(i, j);
        if (_GRID[i * SIZEN + j] > 0)
            continue;
        for (int k = 0; k < 8; k++) {
            int ii = i + DI[k];
            int jj = j + DJ[k];
            if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN
                && _GRID[ii * SIZEN + jj] != C_MINE
                && (kh_get_i32(set, (ii + 1) * 100 + jj) == kh_end(set))) {
                kdq_push_Vector2(queue, (Vector2) {ii, jj});
            }
        }
    }

    kdq_destroy_Vector2(queue);
    kh_destroy_i32(set);
    return nopened;
}

int open_around(int i, int j)
{
    if (i < 0 || i >= SIZEM || j < 0 || j > SIZEN
        || _VISIBLE_GRID[i * SIZEN + j] == C_FLAG)
        return 0;

    int valid = 1;
    int nmines = 0;
    int nflags = 0;
    int nopened = 0;

    for (int k = 0; k < 8; k++) {
        int ii = i + DI[k];
        int jj = j + DJ[k];
        int idx = ii * SIZEN + jj;
        if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN) {
            if (_GRID[idx] == C_MINE)
                nmines++;
            if (_VISIBLE_GRID[idx] == C_FLAG) {
                nflags++;
                if (_GRID[idx] != C_MINE)
                    valid = 0;
            }
        }
    }
    if (nmines == nflags) {
        for (int k = 0; k < 8; k++) {
            int ii = i + DI[k];
            int jj = j + DJ[k];
            int idx = ii * SIZEN + jj;
            if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN) {
                int gval = _GRID[idx];
                int vval = _VISIBLE_GRID[idx];
                if (gval == C_EMPTY || vval == C_CLOSED)
                    nopened += expand(ii, jj);
                else if ((gval == C_MINE && !(vval == C_FLAG))
                         || gval != C_MINE)
                    nopened += open_cell(ii, jj);
            }
        }
    }
    if (valid)
        return nopened;
    else {
        if (nflags == nmines)
            return -1;
        else
            return 0;
    }
}

void set_easy_flags(int i, int j)
{
    int nclosed = 0;
    int nflags = 0;
    int val = _GRID[i * SIZEN + j];
    for (int k = 0; k < 8; k++) {
        int ii = i + DI[k];
        int jj = j + DJ[k];
        int idx = ii * SIZEN + jj;
        if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN) {
            if (_VISIBLE_GRID[idx] == C_CLOSED)
                nclosed++;
            else if (_VISIBLE_GRID[idx] == C_FLAG)
                nflags++;
        }
    }
    if (nclosed + nflags == val) {
        for (int k = 0; k < 8; k++) {
            int ii = i + DI[k];
            int jj = j + DJ[k];
            int idx = ii * SIZEN + jj;
            if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN)
                if (_GRID[idx] == C_MINE)
                    _VISIBLE_GRID[idx] = C_FLAG;
        }
    }
}

void print_grid(const int *grid, int m, int n)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%2d ", grid[i * n + j]);
        }
        putchar('\n');
    }
}

static void generate_mines(int nmines)
{
    assert(SIZEM * SIZEN >= nmines);

    for (int N = 0; N < nmines; N++) {
        int i = GetRandomValue(0, SIZEM - 1);
        int j = GetRandomValue(0, SIZEN - 1);
        if (_GRID[i * SIZEN + j] == C_MINE) {
            N--;
            continue;
        } else {
            _GRID[i * SIZEN + j] = C_MINE;
        }
    }
}

static void count_mines(void)
{
    for (int i = 0; i < SIZEM; i++) {
        for (int j = 0; j < SIZEN; j++) {
            if (_GRID[i * SIZEN + j] == -1)
                continue;

            int count = 0;
            for (int k = 0; k < 8; k++) {
                int ii = i + DI[k];
                int jj = j + DJ[k];
                if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN
                    && _GRID[ii * SIZEN + jj] == -1)
                    count++;
            }
            _GRID[i * SIZEN + j] = count;
        }
    }
}

static void clear_grids(void)
{
    for (int i = 0; i < SIZEM; i++) {
        for (int j = 0; j < SIZEN; j++) {
            _GRID[i * SIZEN + j] = C_EMPTY;
            _VISIBLE_GRID[i * SIZEN + j] = C_CLOSED;
        }
    }
}
