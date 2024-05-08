#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "drawings.h"
#include "grid.h"
#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Local (to module) variables */
static bool ALIVE;                  /* Still playing? */
static bool WON;                    /* Won? */
static bool FIRST_CLICK;            /* First click? */
static int M;                       /* Number of rows */
static int N;                       /* Number of cols */
static int NMINES;                  /* Number of mines */
static int NOPENED;                 /* Number of opened cells */
static double START_TIME, END_TIME; /* Timing */
static char MESSAGE[256];           /* Message to print on victory or defeat */

void init_gameplay_screen(int m, int n, int nmines)
{
    ALIVE       = true;
    WON         = false;
    FIRST_CLICK = true;
    M           = m;
    N           = n;
    NMINES      = nmines;
    NOPENED     = 0;

    grid_destroy();
    if (grid_init(M, N, NMINES) < 0) {
        fprintf(stderr, "Can't initialize grid\n");
        abort();
    };
}

void unload_gameplay_screen(void)
{
    grid_destroy();
    return;
}

void update_gameplay_screen(void)
{
    if (IsKeyPressed(KEY_R)) {
        init_gameplay_screen(M, N, NMINES);
    }

    if (ALIVE && !WON) {
        int offx = (GetScreenWidth() - CELL_SIZE * N) / 2;
        int offy = (GetScreenHeight() - CELL_SIZE * M) / 2;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_pos = GetMousePosition();
            int cur_nopened   = 0;

            int i   = (mouse_pos.y - offy) / CELL_SIZE;
            int j   = (mouse_pos.x - offx) / CELL_SIZE;
            int idx = i * N + j;

            if (FIRST_CLICK) {
                FIRST_CLICK = false;
                START_TIME  = GetTime();
            }

            NOPENED += open_cell(i, j);

            if (VISIBLE_GRID[idx] == C_EMPTY)
                NOPENED += discover(i, j);

            cur_nopened = open_around(i, j);
            if (VISIBLE_GRID[idx] == C_BLAST || cur_nopened == -1) {
                ALIVE = false;
                open_mines();
            }
            NOPENED += cur_nopened;

            if (NOPENED + NMINES == M * N) {
                WON      = true;
                END_TIME = GetTime();
                open_safe_cells();
                set_safe_flags();
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse_pos = GetMousePosition();

            int i   = (mouse_pos.y - offy) / CELL_SIZE;
            int j   = (mouse_pos.x - offx) / CELL_SIZE;
            int idx = i * N + j;

            if (VISIBLE_GRID[idx] == C_CLOSED || VISIBLE_GRID[idx] == C_FLAG) {
                toggle_flag(i, j);
            }
        }
    }
}

void draw_gameplay_screen(void)
{
    int offx = (GetScreenWidth() - CELL_SIZE * N) / 2;
    int offy = (GetScreenHeight() - CELL_SIZE * M) / 2;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            CELL_TYPE type = VISIBLE_GRID[i * N + j];
            DrawTexture(cells[type], offx + j * CELL_SIZE, offy + i * CELL_SIZE,
                        LIGHTGRAY);
        }
    }

    if (WON) {
        sprintf(MESSAGE, "You won! %.3f", (END_TIME - START_TIME));
        dim_screen();
        draw_centered_text(MESSAGE, CELL_SIZE, GREEN);
    } else if (!ALIVE) {
        dim_screen();
        draw_centered_text("You lost", CELL_SIZE, RED);
    }
}
