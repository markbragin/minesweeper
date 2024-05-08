#ifndef _GRID_H_
#define _GRID_H_

typedef enum CELL_TYPE {
    C_EMPTY,
    C_ONE,
    C_TWO,
    C_THREE,
    C_FOUR,
    C_FIVE,
    C_SIX,
    C_SEVEN,
    C_EIGHT,
    C_BLAST,
    C_CLOSED,
    C_FLAG,
    C_MINE,
} CELL_TYPE;

/* Const pointers to allocated grids. NULL if unallocated */
extern const int *GRID;
extern const int *VISIBLE_GRID;

/* Inits grid. Caller must call grid_destroy to free memory */
int grid_init(int m, int n, int nmines);

/* Destroys grid */
void grid_destroy(void);

/* Opens cell and returns numbers of opened cells */
int open_cell(int i, int j);

/* Opens cells around given one if it's unambiguous. Returns number of opened
 * cells or -1 if a mine is opened */
int open_around(int i, int j);

/* Discovers safe cells around given cell. Returns number of opened cells */
int discover(int i, int j);

/* Sets flags around given cell if it's unambiguous */
void set_easy_flags(int i, int j);

/* Toggles flag on the visible grid.
 * Returns 1 if set, -1 if unset, 0 if neither */
int toggle_flag(int i, int j);

/* Opens all the mines on the visible grid if cell is not C_FLAG or C_BLAST.
 * Used when lost */
void open_mines(void);

/* Opens all the safe cells on the visible grid.
 * Used when won */
void open_safe_cells(void);

/* Sets valid flags on the visible grid.
 * Used when won */
void set_safe_flags(void);

/* Prints grid to stdout */
void print_grid(const int *grid, int m, int n);

#endif
