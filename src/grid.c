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
static int *grid_;
static int *visible_grid_;
static int sizem_;
static int sizen_;
static int nmines_;

/* Pointers to use grids outside module. (Bad thing?)*/
const int *GRID;
const int *VISIBLE_GRID;

/* Deltas for surrounding cells */
static const int DI_[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
static const int DJ_[8] = {0, 1, 1, 1, 0, -1, -1, -1};

/* Sets all cells to appropriate values (numbers of mines around) */
static void count_mines_(void);

/* Sets all GRID's cells to C_EMPTY and VISIBLE_GRID's ones to C_CLOSED */
static void clear_grids_(void);

/* Counts cells of given type around given cell in the grid */
static int count_around_(const int *grid, int i, int j, CellType type);

int grid_init(int m, int n, int nmines)
{
    grid_         = malloc(m * n * sizeof(int));
    visible_grid_ = malloc(m * n * sizeof(int));
    sizem_        = m;
    sizen_        = n;
    nmines_       = nmines;

    if (!grid_ || !visible_grid_)
        return -1;
    if (m * n < nmines)
        return -2;

    clear_grids_();
    GRID         = grid_;
    VISIBLE_GRID = visible_grid_;

    return 0;
}

void grid_destroy(void)
{
    free(grid_);
    free(visible_grid_);
    grid_         = NULL;
    visible_grid_ = NULL;
    GRID          = NULL;
    VISIBLE_GRID  = NULL;
}

int open_cell(int i, int j)
{
    /* Ignore bad indexes, opened cells and flags */
    int idx = i * sizen_ + j;
    if (i < 0 || i >= sizem_ || j < 0 || j > sizen_
        || (C_EMPTY <= visible_grid_[idx] && visible_grid_[idx] <= C_EIGHT)
        || visible_grid_[idx] == C_FLAG)
        return 0;

    /* Opens safely */
    if (C_EMPTY <= grid_[idx] && grid_[idx] <= C_EIGHT) {
        visible_grid_[idx] = grid_[idx];
        return 1;
    } else if (grid_[idx] == C_MINE)
        visible_grid_[idx] = C_BLAST; /* Opens not flaged mine */

    return 0;
}

void open_mines(void)
{
    for (int idx = 0; idx < sizem_ * sizen_; idx++) {
        if (grid_[idx] == C_MINE
            && !(visible_grid_[idx] == C_BLAST
                 || visible_grid_[idx] == C_FLAG)) {
            visible_grid_[idx] = grid_[idx];
        }
    }
}

void open_safe_cells(void)
{
    for (int idx = 0; idx < sizem_ * sizen_; idx++)
        if (grid_[idx] != C_MINE)
            visible_grid_[idx] = grid_[idx];
}

void set_safe_flags(void)
{
    for (int idx = 0; idx < sizem_ * sizen_; idx++)
        if (grid_[idx] == C_MINE)
            visible_grid_[idx] = C_FLAG;
}

int toggle_flag(int i, int j)
{
    /* Ignore bad indexes */
    if (i < 0 || i >= sizem_ || j < 0 || j > sizen_)
        return 0;

    int idx = i * sizen_ + j;
    int val = visible_grid_[idx];
    assert(val == C_CLOSED || val == C_FLAG);

    if (val == C_FLAG) {
        visible_grid_[idx] = C_CLOSED;
        return -1;
    } else {
        visible_grid_[idx] = C_FLAG;
        return 1;
    }
}

/* Used bfs algo */
int discover(int i, int j)
{
    /* Ignore bad indexes */
    if (i < 0 || i >= sizem_ || j < 0 || j > sizen_)
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

        if (grid_[i * sizen_ + j] > 0)
            continue;

        for (int k = 0; k < 8; k++) {
            int ii = i + DI_[k];
            int jj = j + DJ_[k];
            if (0 <= ii && ii < sizem_ && 0 <= jj && jj < sizen_
                && grid_[ii * sizen_ + jj] != C_MINE
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
    if (i < 0 || i >= sizem_ || j < 0 || j > sizen_
        || visible_grid_[i * sizen_ + j] == C_FLAG)
        return 0;

    bool valid  = true;
    int nopened = 0;
    int nmines  = count_around_(grid_, i, j, C_MINE);
    int nflags  = count_around_(visible_grid_, i, j, C_FLAG);

    /* If the number of mines and flags is the same, opens cells even if
     * it's unsafe (mine) */
    if (nmines == nflags) {
        for (int k = 0; k < 8; k++) {
            int ii  = i + DI_[k];
            int jj  = j + DJ_[k];
            int idx = ii * sizen_ + jj;
            if (0 <= ii && ii < sizem_ && 0 <= jj && jj < sizen_) {
                nopened += open_cell(ii, jj);
                int vval = visible_grid_[idx];
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

void down_around(int i, int j)
{
    /* Ignore bad indexes and flag */
    if (i < 0 || i >= sizem_ || j < 0 || j > sizen_
        || visible_grid_[i * sizen_ + j] == C_FLAG)
        return;

    int idx = i * sizen_ + j;
    if (visible_grid_[idx] == C_CLOSED) {
        visible_grid_[idx] = C_DOWN;
    } else {
        for (int k = 0; k < 8; k++) {
            int ii  = i + DI_[k];
            int jj  = j + DJ_[k];
            int idx = ii * sizen_ + jj;
            if (0 <= ii && ii < sizem_ && 0 <= jj && jj < sizen_) {
                if (visible_grid_[idx] == C_CLOSED)
                    visible_grid_[idx] = C_DOWN;
            }
        }
    }
}

void up_around(int i, int j)
{
    /* Ignore bad indexes */
    if (i < 0 || i >= sizem_ || j < 0 || j > sizen_)
        return;

    int idx = i * sizen_ + j;
    if (visible_grid_[idx] == C_DOWN) {
        visible_grid_[idx] = C_CLOSED;
    } else {
        for (int k = 0; k < 8; k++) {
            int ii  = i + DI_[k];
            int jj  = j + DJ_[k];
            int idx = ii * sizen_ + jj;
            if (0 <= ii && ii < sizem_ && 0 <= jj && jj < sizen_) {
                if (visible_grid_[idx] == C_DOWN)
                    visible_grid_[idx] = C_CLOSED;
            }
        }
    }
}

int set_easy_flags(int i, int j)
{
    int val     = grid_[i * sizen_ + j];
    int nclosed = count_around_(visible_grid_, i, j, C_CLOSED);
    int nflags  = count_around_(visible_grid_, i, j, C_FLAG);

    if (nclosed + nflags == val) {
        for (int k = 0; k < 8; k++) {
            int ii  = i + DI_[k];
            int jj  = j + DJ_[k];
            int idx = ii * sizen_ + jj;
            if (0 <= ii && ii < sizem_ && 0 <= jj && jj < sizen_)
                if (grid_[idx] == C_MINE)
                    visible_grid_[idx] = C_FLAG;
        }
        return nclosed;
    }
    return 0;
}

static int count_around_(const int *grid, int i, int j, CellType type)
{
    int cnt = 0;
    for (int k = 0; k < 8; k++) {
        int ii  = i + DI_[k];
        int jj  = j + DJ_[k];
        int idx = ii * sizen_ + jj;
        if (0 <= ii && ii < sizem_ && 0 <= jj && jj < sizen_) {
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

int generate_mines(int i, int j)
{
    assert(sizem_ * sizen_ >= nmines_);

    /* -1 on bad indexes */
    if (i < 0 || i >= sizem_ || j < 0 || j > sizen_)
        return -1;

    SetRandomSeed(time(NULL));

    int N = 0;
    while (N < nmines_) {
        int ii  = GetRandomValue(0, sizem_ - 1);
        int jj  = GetRandomValue(0, sizen_ - 1);
        int idx = ii * sizen_ + jj;
        if ((ii == i && jj == j) || grid_[idx] == C_MINE) {
            continue;
        } else {
            grid_[idx] = C_MINE;
            N++;
        }
    }

    count_mines_();
    return 0;
}

static void count_mines_(void)
{
    for (int i = 0; i < sizem_; i++) {
        for (int j = 0; j < sizen_; j++) {
            int idx = i * sizen_ + j;
            if (grid_[idx] == C_MINE)
                continue;

            int count = 0;
            for (int k = 0; k < 8; k++) {
                int ii = i + DI_[k];
                int jj = j + DJ_[k];
                /* If valid cell and it's a mine increment counter */
                if (0 <= ii && ii < sizem_ && 0 <= jj && jj < sizen_
                    && grid_[ii * sizen_ + jj] == C_MINE)
                    count++;
            }
            grid_[idx] = count;
        }
    }
}

static void clear_grids_(void)
{
    for (int i = 0; i < sizem_ * sizen_; i++) {
        grid_[i]         = C_EMPTY;
        visible_grid_[i] = C_CLOSED;
    }
}
