#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "db.h"
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

/* Local (to module) variables */
static State CURRENT_STATE;
static Face CURRENT_FACE;
static bool FINISH_SCREEN; /* Should screen finish */
static Vector2 CELL_DOWN;  /* Last cell pressed */
static bool AUTO_FLAGS;    /* Secret mode */
static bool FIRST_CLICK;   /* First click? */

static int FLAGS_LEFT;              /* Number of flags */
static int NOPENED;                 /* Number of opened cells */
static double START_TIME, END_TIME; /* Timing */
static char MESSAGE[256];           /* Message to print on victory or defeat */

static void draw_header(void);
static Rectangle get_face_rect(void);

void init_gameplay_screen(void)
{
    CURRENT_STATE = S_PLAYING;
    CURRENT_FACE  = F_SMILE;
    FINISH_SCREEN = false;
    FIRST_CLICK   = true;
    FLAGS_LEFT    = NMINES;
    NOPENED       = 0;

    grid_destroy();
    if (grid_init(SIZEM, SIZEN, NMINES) < 0) {
        fprintf(stderr, "Can't initialize grid\n");
        abort();
    };
}

void unload_gameplay_screen(void)
{
    grid_destroy();
}

void update_gameplay_screen(void)
{
    Vector2 mouse_pos = GetMousePosition();

    /* Finish on ESCAPE */
    if (IsKeyPressed(KEY_ESCAPE)) {
        FINISH_SCREEN = true;
    }

    /* Feature for Sanek */
    if (IsKeyPressed(KEY_F)) {
        AUTO_FLAGS = AUTO_FLAGS ? false : true;
    }

    /* New game on click on face */
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mouse_pos, get_face_rect())) {
        init_gameplay_screen();
    }

    if (CURRENT_STATE == S_PLAYING) {
        /* Reset temporary states */
        CURRENT_FACE = F_SMILE;
        up_around(CELL_DOWN.x, CELL_DOWN.y);

        /* Offsets from {0, 0} when window resized. Need to center grid */
        int offx = (GetScreenWidth() - CELL_SIZE * SIZEN) / 2;
        int offy = (HEADER_HEIGHT + GetScreenHeight() - CELL_SIZE * SIZEM) / 2;

        int i   = (mouse_pos.y - offy) / CELL_SIZE;
        int j   = (mouse_pos.x - offx) / CELL_SIZE;
        int idx = i * SIZEN + j;

        /* Process click on header first */
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse_pos, get_face_rect())) {
                CURRENT_FACE = F_SMILE_DOWN;
            }
        }

        /* Process click on grid next */
        Rectangle grid_rect
            = {offx, offy, CELL_SIZE * SIZEN, CELL_SIZE * SIZEM};
        if (!CheckCollisionPointRec(mouse_pos, grid_rect)) {
            return;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            CURRENT_FACE = F_CLICK;
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
                CURRENT_FACE  = F_LOST;
                END_TIME      = GetTime();
                open_mines();
            } else if (VISIBLE_GRID[idx] == C_EMPTY) {
                NOPENED += discover(i, j);
            } else {
                int cur_nopened = open_around(i, j);
                if (cur_nopened == -1) {
                    CURRENT_STATE = S_LOST;
                    CURRENT_FACE  = F_LOST;
                    END_TIME      = GetTime();
                    open_mines();
                }
                NOPENED += cur_nopened;
                if (AUTO_FLAGS)
                    FLAGS_LEFT -= set_easy_flags(i, j);
            }

            if (NOPENED + NMINES == SIZEM * SIZEN) {
                CURRENT_STATE = S_WON;
                END_TIME      = GetTime();
                open_safe_cells();
                set_safe_flags();
                db_save_record((END_TIME - START_TIME),
                               DIFFICULTIES_STR[DIFFICULTY]);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse_pos = GetMousePosition();

            int i   = (mouse_pos.y - offy) / CELL_SIZE;
            int j   = (mouse_pos.x - offx) / CELL_SIZE;
            int idx = i * SIZEN + j;

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
    int offx = (GetScreenWidth() - CELL_SIZE * SIZEN) / 2;
    int offy = (HEADER_HEIGHT + GetScreenHeight() - CELL_SIZE * SIZEM) / 2;

    for (int i = 0; i < SIZEM; i++) {
        for (int j = 0; j < SIZEN; j++) {
            DrawTexture(cells[VISIBLE_GRID[i * SIZEN + j]],
                        offx + j * CELL_SIZE, offy + i * CELL_SIZE, LIGHTGRAY);
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

    if (CURRENT_STATE == S_PLAYING) {
        if (FIRST_CLICK)
            secs = 0;
        else
            secs = (int)(GetTime() - START_TIME);
    } else {
        secs = (int)(END_TIME - START_TIME);
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
    DrawTexture(faces[CURRENT_FACE], face_offset, pos_y, LIGHTGRAY);

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

bool finish_gameplay_screen(void)
{
    return FINISH_SCREEN;
}
