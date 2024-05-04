#ifndef _GRID_H_
#define _GRID_H_

enum CELL_TYPE {
    C_BLAST = -4,
    C_CLOSED,
    C_FLAG,
    C_MINE,
    C_EMPTY,
    C_ONE,
    C_TWO,
    C_THREE,
    C_FOUR,
    C_FIVE,
    C_SIX,
    C_SEVEN,
    C_EIGHT,
};

extern const int *GRID;
extern const int *VISIBLE_GRID;

int grid_init(int m, int n, int nmines);
void grid_destroy(void);

int open_cell(int i, int j);
int open_around(int i, int j);
int expand(int i, int j);

void set_easy_flags(int i, int j);
void toggle_flag(int i, int j);

void open_mines(void);
void open_safe_cells(void);
void print_grid(const int *grid, int m, int n);

#endif
