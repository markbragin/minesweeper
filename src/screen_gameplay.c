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

typedef enum State {
    S_PLAYING,
    S_WON,
    S_LOST
} State;

/* Local (to module) variables */
static State current_state_;
static Face current_face_;
static bool finish_screen_; /* Should screen finish */
static Vector2 cell_down_;  /* Last cell pressed */
static bool auto_flags_;    /* Secret mode */
static bool first_click_;   /* First click? */

static int flags_left_;              /* Number of flags */
static int nopened_;                 /* Number of opened cells */
static double start_time_, end_time; /* Timing */
static char message_[256];           /* Message to print on victory or defeat */

static void draw_header_(void);
static Rectangle get_face_rect_(void);

void init_gameplay_screen(void)
{
    current_state_ = S_PLAYING;
    current_face_  = F_SMILE;
    finish_screen_ = false;
    first_click_   = true;
    flags_left_    = nmines;
    nopened_       = 0;

    grid_destroy();
    if (grid_init(sizem, sizen, nmines) < 0) {
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
        finish_screen_ = true;
    }

    /* Feature for Sanek */
    if (IsKeyPressed(KEY_F)) {
        auto_flags_ = auto_flags_ ? false : true;
    }

    /* New game on click on face */
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mouse_pos, get_face_rect_())) {
        init_gameplay_screen();
    }

    if (current_state_ == S_PLAYING) {
        /* Reset temporary states */
        current_face_ = F_SMILE;
        up_around(cell_down_.x, cell_down_.y);

        /* Offsets from {0, 0} when window resized. Need to center grid */
        int offx = (GetScreenWidth() - CELL_SIZE * sizen) / 2;
        int offy = (HEADER_HEIGHT + GetScreenHeight() - CELL_SIZE * sizem) / 2;

        int i   = (mouse_pos.y - offy) / CELL_SIZE;
        int j   = (mouse_pos.x - offx) / CELL_SIZE;
        int idx = i * sizen + j;

        /* Process click on header first */
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse_pos, get_face_rect_())) {
                current_face_ = F_SMILE_DOWN;
            }
        }

        /* Process click on grid next */
        Rectangle grid_rect
            = {offx, offy, CELL_SIZE * sizen, CELL_SIZE * sizem};
        if (!CheckCollisionPointRec(mouse_pos, grid_rect)) {
            return;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            current_face_ = F_CLICK;
            down_around(i, j);
            cell_down_ = (Vector2) {i, j};
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            up_around(cell_down_.x, cell_down_.y);
            if (mouse_pos.y <= HEADER_HEIGHT)
                return;

            if (first_click_) {
                first_click_ = false;
                generate_mines(i, j);
                start_time_ = GetTime();
            }

            nopened_ += open_cell(i, j);

            if (VISIBLE_GRID[idx] == C_BLAST) {
                current_state_ = S_LOST;
                current_face_  = F_LOST;
                end_time       = GetTime();
                open_mines();
            } else if (VISIBLE_GRID[idx] == C_EMPTY) {
                nopened_ += discover(i, j);
            } else {
                int cur_nopened = open_around(i, j);
                if (cur_nopened == -1) {
                    current_state_ = S_LOST;
                    current_face_  = F_LOST;
                    end_time       = GetTime();
                    open_mines();
                }
                nopened_ += cur_nopened;
                if (auto_flags_)
                    flags_left_ -= set_easy_flags(i, j);
            }

            if (nopened_ + nmines == sizem * sizen) {
                current_state_ = S_WON;
                end_time       = GetTime();
                open_safe_cells();
                set_safe_flags();
                db_save_record((end_time - start_time_),
                               DIFFICULTIES_STR[difficulty]);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse_pos = GetMousePosition();

            int i   = (mouse_pos.y - offy) / CELL_SIZE;
            int j   = (mouse_pos.x - offx) / CELL_SIZE;
            int idx = i * sizen + j;

            if (VISIBLE_GRID[idx] == C_CLOSED || VISIBLE_GRID[idx] == C_FLAG) {
                flags_left_ -= toggle_flag(i, j);
            }
        }
    }
}

void draw_gameplay_screen(void)
{
    draw_header_();

    /* Offsets from {0, 0} when window resized. Need to center grid */
    int offx = (GetScreenWidth() - CELL_SIZE * sizen) / 2;
    int offy = (HEADER_HEIGHT + GetScreenHeight() - CELL_SIZE * sizem) / 2;

    for (int i = 0; i < sizem; i++) {
        for (int j = 0; j < sizen; j++) {
            DrawTexture(cells[VISIBLE_GRID[i * sizem + j]],
                        offx + j * CELL_SIZE, offy + i * CELL_SIZE, LIGHTGRAY);
        }
    }

    if (current_state_ == S_WON) {
        sprintf(message_, "You won! %.3f", (end_time - start_time_));
        dim_area((Rectangle) {0, HEADER_HEIGHT, GetScreenWidth(),
                              GetScreenHeight() - HEADER_HEIGHT});
        draw_centered_text(message_, CELL_SIZE, GREEN);
    } else if (current_state_ == S_LOST) {
        dim_area((Rectangle) {0, HEADER_HEIGHT, GetScreenWidth(),
                              GetScreenHeight() - HEADER_HEIGHT});
        draw_centered_text("You lost", CELL_SIZE, RED);
    }
}

static void draw_header_(void)
{
    int time_offset, face_offset, flags_offset, pos_y, secs, nflags;

    if (current_state_ == S_PLAYING) {
        if (first_click_)
            secs = 0;
        else
            secs = (int)(GetTime() - start_time_);
    } else {
        secs = (int)(end_time - start_time_);
    }

    nflags = flags_left_;
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
    DrawTexture(faces[current_face_], face_offset, pos_y, LIGHTGRAY);

    /* Draw time counter */
    for (int i = 0; i < 3; i++) {
        DrawTexture(counter[secs % 10], time_offset, pos_y, RED);
        secs /= 10;
        time_offset -= COUNTER_WIDTH;
    }
}

static Rectangle get_face_rect_(void)
{
    int pos_y = (HEADER_HEIGHT - COUNTER_HEIGHT) / 2;
    int pos_x = (GetScreenWidth() - FACE_SIZE) / 2;
    return (Rectangle) {pos_x, pos_y, FACE_SIZE, FACE_SIZE};
}

bool finish_gameplay_screen(void)
{
    return finish_screen_;
}
