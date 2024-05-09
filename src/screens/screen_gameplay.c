#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "drawings.h"
#include "grid.h"
#include "raylib.h"
#include "screens.h"
#include "textures.h"

typedef enum {
    S_PLAYING,
    S_WON,
    S_LOST
} State;

typedef enum {
    F_CLICK,
    F_LOST,
    F_SMILE,
    F_SMILE_DOWN,
    F_WON
} Face;

/* Local (to module) variables */

static Vector2 CELL_DOWN;
static State CURRENT_STATE;
static Face CURRENT_FACE;
static bool FIRST_CLICK;            /* First click? */
static int M;                       /* Number of rows */
static int N;                       /* Number of cols */
static int NMINES;                  /* Number of mines */
static int FLAGS_LEFT;              /* Number of flags */
static int NOPENED;                 /* Number of opened cells */
static double START_TIME, END_TIME; /* Timing */
static char MESSAGE[256];           /* Message to print on victory or defeat */

static void draw_header(void);
static Rectangle get_face_rect(void);

void init_gameplay_screen(int m, int n, int nmines)
{
    CURRENT_STATE = S_PLAYING;
    CURRENT_FACE  = F_SMILE;
    FIRST_CLICK   = true;
    M             = m;
    N             = n;
    NMINES        = nmines;
    FLAGS_LEFT    = nmines;
    NOPENED       = 0;

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
    Vector2 mouse_pos = GetMousePosition();

    /* New game if lost or won */
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mouse_pos, get_face_rect())) {
        init_gameplay_screen(M, N, NMINES);
    }

    if (CURRENT_STATE == S_PLAYING) {
        /* Up down cells around */
        up_around(CELL_DOWN.x, CELL_DOWN.y);

        /* Offsets from {0, 0} when window resized. Need to center grid */
        int offx = (GetScreenWidth() - CELL_SIZE * N) / 2;
        int offy = (HEADER_HEIGHT + GetScreenHeight() - CELL_SIZE * M) / 2;

        /* Check if click is on grid */
        Rectangle grid_rect = {offx, offy, CELL_SIZE * N, CELL_SIZE * M};
        if (!CheckCollisionPointRec(mouse_pos, grid_rect)) {
            return;
        }

        int i   = (mouse_pos.y - offy) / CELL_SIZE;
        int j   = (mouse_pos.x - offx) / CELL_SIZE;
        int idx = i * N + j;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            down_around(i, j);
            CELL_DOWN = (Vector2) {i, j};
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            up_around(CELL_DOWN.x, CELL_DOWN.y);
            if (mouse_pos.y <= HEADER_HEIGHT)
                return;

            if (FIRST_CLICK) {
                FIRST_CLICK = false;
                generate_mines(i, j);
                START_TIME = GetTime();
            }

            NOPENED += open_cell(i, j);

            if (VISIBLE_GRID[idx] == C_BLAST) {
                CURRENT_STATE = S_LOST;
                END_TIME      = GetTime();
                open_mines();
            } else if (VISIBLE_GRID[idx] == C_EMPTY) {
                NOPENED += discover(i, j);
            } else {
                int cur_nopened = open_around(i, j);
                if (cur_nopened == -1) {
                    CURRENT_STATE = S_LOST;
                    END_TIME      = GetTime();
                    open_mines();
                }
                NOPENED += cur_nopened;
            }

            if (NOPENED + NMINES == M * N) {
                CURRENT_STATE = S_WON;
                END_TIME      = GetTime();
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
                FLAGS_LEFT -= toggle_flag(i, j);
            }
        }
    }
}

void draw_gameplay_screen(void)
{
    draw_header();

    /* Offsets from {0, 0} when window resized. Need to center grid */
    int offx = (GetScreenWidth() - CELL_SIZE * N) / 2;
    int offy = (HEADER_HEIGHT + GetScreenHeight() - CELL_SIZE * M) / 2;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            DrawTexture(cells[VISIBLE_GRID[i * N + j]], offx + j * CELL_SIZE,
                        offy + i * CELL_SIZE, LIGHTGRAY);
        }
    }

    if (CURRENT_STATE == S_WON) {
        sprintf(MESSAGE, "You won! %.3f", (END_TIME - START_TIME));
        dim_area((Rectangle) {0, HEADER_HEIGHT, GetScreenWidth(),
                              GetScreenHeight() - HEADER_HEIGHT});
        draw_centered_text(MESSAGE, CELL_SIZE, GREEN);
    } else if (CURRENT_STATE == S_LOST) {
        dim_area((Rectangle) {0, HEADER_HEIGHT, GetScreenWidth(),
                              GetScreenHeight() - HEADER_HEIGHT});
        draw_centered_text("You lost", CELL_SIZE, RED);
    }
}

static void draw_header(void)
{
    int time_offset, face_offset, flags_offset, pos_y, secs, nflags;
    Face face;

    if (CURRENT_STATE == S_PLAYING) {
        face = F_SMILE;
        if (FIRST_CLICK)
            secs = 0;
        else
            secs = (int)(GetTime() - START_TIME);
    } else {
        secs = (int)(END_TIME - START_TIME);
        if (CURRENT_STATE == S_WON)
            face = F_WON;
        else if (CURRENT_STATE == S_LOST)
            face = F_LOST;
    }

    nflags = FLAGS_LEFT;
    pos_y  = (HEADER_HEIGHT - COUNTER_HEIGHT) / 2;

    /* Calculate offsets */
    flags_offset = COUNTER_WIDTH * 3;
    face_offset  = (GetScreenWidth() - FACE_SIZE) / 2;
    time_offset  = GetScreenWidth() - COUNTER_WIDTH * 2;

    /* Draw flags counter */
    int sign = nflags;
    for (int i = 0; i < 3 && -100 < nflags && nflags < 1000; i++) {
        int digit = abs(nflags % 10);
        if (i == 2 && sign < 0)
            DrawTexture(counter[10], flags_offset, pos_y, RED); /* 10 is minus*/
        else
            DrawTexture(counter[digit], flags_offset, pos_y, RED);
        flags_offset -= COUNTER_WIDTH;
        nflags /= 10;
    }

    /* Draw face */
    DrawTexture(faces[face], face_offset, pos_y, LIGHTGRAY);

    /* Draw time counter */
    for (int i = 0; i < 3; i++) {
        DrawTexture(counter[secs % 10], time_offset, pos_y, RED);
        secs /= 10;
        time_offset -= COUNTER_WIDTH;
    }
}

static Rectangle get_face_rect(void)
{
    int pos_y = (HEADER_HEIGHT - COUNTER_HEIGHT) / 2;
    int pos_x = (GetScreenWidth() - FACE_SIZE) / 2;
    return (Rectangle) {pos_x, pos_y, FACE_SIZE, FACE_SIZE};
}
