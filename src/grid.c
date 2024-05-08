#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "grid.h"
#include "hash.h"
#include "queue.h"
#include "raylib.h"

/* Queue and hashset for discovering cells (bfs) algo */
KDQ_INIT(Vector2)
KHASH_SET_INIT_INT(i32)

/* Grid params */
static int *_GRID;
static int *_VISIBLE_GRID;
static int SIZEM;
static int SIZEN;

/* Pointers to use grids outside module. (Bad thing?)*/
const int *GRID;
const int *VISIBLE_GRID;

/* Deltas for surrounding cells */
static const int DI[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
static const int DJ[8] = {0, 1, 1, 1, 0, -1, -1, -1};

/* Generates nmines mines on the board in random places */
static void generate_mines(int nmines);

/* Sets all cells to appropriate values (numbers of mines around) */
static void count_mines(void);

/* Sets all GRID's cells to C_EMPTY and VISIBLE_GRID's ones to C_CLOSED */
static void clear_grids(void);

/* Counts cells of given type around given cell in the grid */
static int count_around(const int *grid, int i, int j, CELL_TYPE type);

int grid_init(int m, int n, int nmines)
{
    _GRID         = malloc(m * n * sizeof(int));
    _VISIBLE_GRID = malloc(m * n * sizeof(int));
    SIZEM         = m;
    SIZEN         = n;

    if (!_GRID || !_VISIBLE_GRID)
        return -1;
    if (m * n < nmines)
        return -2;

    SetRandomSeed(time(NULL));
    clear_grids();
    generate_mines(nmines);
    count_mines();

    GRID         = _GRID;
    VISIBLE_GRID = _VISIBLE_GRID;

    return 0;
}

void grid_destroy(void)
{
    free(_GRID);
    free(_VISIBLE_GRID);
    _GRID         = NULL;
    _VISIBLE_GRID = NULL;
    GRID          = NULL;
    VISIBLE_GRID  = NULL;
}

int open_cell(int i, int j)
{
    /* Ignore bad indexes, opened cells and flags */
    int idx = i * SIZEN + j;
    if (i < 0 || i >= SIZEM || j < 0 || j > SIZEN
        || (C_EMPTY <= _VISIBLE_GRID[idx] && _VISIBLE_GRID[idx] <= C_EIGHT)
        || _VISIBLE_GRID[idx] == C_FLAG)
        return 0;

    /* Opens safely */
    if (C_EMPTY <= _GRID[idx] && _GRID[idx] <= C_EIGHT) {
        _VISIBLE_GRID[idx] = _GRID[idx];
        return 1;
    } else if (_GRID[idx] == C_MINE)
        _VISIBLE_GRID[idx] = C_BLAST; /* Opens not flaged mine */

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

void set_safe_flags(void)
{
    for (int idx = 0; idx < SIZEM * SIZEN; idx++)
        if (_GRID[idx] == C_MINE)
            _VISIBLE_GRID[idx] = C_FLAG;
}

int toggle_flag(int i, int j)
{
    /* Ignore bad indexes */
    if (i < 0 || i >= SIZEM || j < 0 || j > SIZEN)
        return 0;

    int idx = i * SIZEN + j;
    int val = _VISIBLE_GRID[idx];
    assert(val == C_CLOSED || val == C_FLAG);

    if (val == C_FLAG) {
        _VISIBLE_GRID[idx] = C_CLOSED;
        return -1;
    } else {
        _VISIBLE_GRID[idx] = C_FLAG;
        return 1;
    }
}

/* Used bfs algo */
int discover(int i, int j)
{
    /* Ignore bad indexes */
    if (i < 0 || i >= SIZEM || j < 0 || j > SIZEN)
        return 0;

    int ret;
    int nopened = 0;
    Vector2 *curv;
    kdq_Vector2_t *queue = kdq_init_Vector2();
    kh_i32_t *set        = kh_init_i32();

    kdq_push_Vector2(queue, (Vector2) {i, j});

    while ((curv = kdq_pop_Vector2(queue)) != NULL) {
        int i = (int)curv->x;
        int j = (int)curv->y;

        /* Here and later used (i + 1) * 100 + j key instead of i + j
         * to avoid collisions */
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
    /* Ignore bad indexes and flag */
    if (i < 0 || i >= SIZEM || j < 0 || j > SIZEN
        || _VISIBLE_GRID[i * SIZEN + j] == C_FLAG)
        return 0;

    bool valid  = true;
    int nopened = 0;
    int nmines  = count_around(_GRID, i, j, C_MINE);
    int nflags  = count_around(_VISIBLE_GRID, i, j, C_FLAG);

    /* If the number of mines and flags is the same, opens cells even if
     * it's unsafe (mine) */
    if (nmines == nflags) {
        for (int k = 0; k < 8; k++) {
            int ii  = i + DI[k];
            int jj  = j + DJ[k];
            int idx = ii * SIZEN + jj;
            if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN) {
                nopened += open_cell(ii, jj);
                int vval = _VISIBLE_GRID[idx];
                if (vval == C_EMPTY)
                    nopened += discover(ii, jj);
                else if (vval == C_BLAST)
                    valid = false;
            }
        }
    }

    if (nflags == nmines)
        return valid ? nopened : -1;
    else
        return 0;
}

void set_easy_flags(int i, int j)
{
    int val     = _GRID[i * SIZEN + j];
    int nclosed = count_around(_VISIBLE_GRID, i, j, C_CLOSED);
    int nflags  = count_around(_VISIBLE_GRID, i, j, C_FLAG);

    if (nclosed + nflags == val) {
        for (int k = 0; k < 8; k++) {
            int ii  = i + DI[k];
            int jj  = j + DJ[k];
            int idx = ii * SIZEN + jj;
            if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN)
                if (_GRID[idx] == C_MINE)
                    _VISIBLE_GRID[idx] = C_FLAG;
        }
    }
}

static int count_around(const int *grid, int i, int j, CELL_TYPE type)
{
    int cnt = 0;
    for (int k = 0; k < 8; k++) {
        int ii  = i + DI[k];
        int jj  = j + DJ[k];
        int idx = ii * SIZEN + jj;
        if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN) {
            if (grid[idx] == type)
                cnt++;
        }
    }
    return cnt;
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

    int N = 0;
    while (N < nmines) {
        int i   = GetRandomValue(0, SIZEM - 1);
        int j   = GetRandomValue(0, SIZEN - 1);
        int idx = i * SIZEN + j;
        if (_GRID[idx] == C_MINE) {
            continue;
        } else {
            _GRID[idx] = C_MINE;
            N++;
        }
    }
}

static void count_mines(void)
{
    for (int i = 0; i < SIZEM; i++) {
        for (int j = 0; j < SIZEN; j++) {
            int idx = i * SIZEN + j;
            if (_GRID[idx] == C_MINE)
                continue;

            int count = 0;
            for (int k = 0; k < 8; k++) {
                int ii = i + DI[k];
                int jj = j + DJ[k];
                /* If valid cell and it's a mine increment counter */
                if (0 <= ii && ii < SIZEM && 0 <= jj && jj < SIZEN
                    && _GRID[ii * SIZEN + jj] == C_MINE)
                    count++;
            }
            _GRID[idx] = count;
        }
    }
}

static void clear_grids(void)
{
    for (int i = 0; i < SIZEM * SIZEN; i++) {
        _GRID[i]         = C_EMPTY;
        _VISIBLE_GRID[i] = C_CLOSED;
    }
}
