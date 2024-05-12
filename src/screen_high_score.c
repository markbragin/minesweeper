#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "config.h"
#include "db.h"
#include "kvec.h"
#include "raylib.h"
#include "record.h"
#include "screens.h"

static r_array high_scores_[3]; /* Array of dynamic arrays of high scores */
static bool data_loaded_;       /* Already loaded? */
static Difficulty current_tab;  /* Current tab in terms of difficulty */
static Font font_;
static int cursor_;

/* Load data from db*/
static void init_arrays_(void);
/* Destroy arrays with data */
static void destroy_arrays_(void);

/* Returns rectangle of i button, or {0, 0, 0, 0} on bad id.
 * Easy - 0 (D_EASY)
 * Medium - 1 (D_MEDIUM)
 * Hard - 2 (D_HARD) */
static Rectangle get_difficulty_button_(int i);

/* Draw list of high scores */
static void draw_high_scores(void);
/* Draw buttons at the buttom */
static void draw_buttons(void);

void init_high_score_screen(void)
{
    cursor_      = 0;
    current_tab  = D_EASY;

    if (data_loaded_)
        return;

    init_arrays_();
    for (int i = D_EASY; i <= D_HARD; i++) {
        if (db_fetch_all_by_difficulty(i, &high_scores_[i]) != SQLITE_OK) {
            destroy_arrays_();
            should_close = true;
            return;
        }
    }

    if (!IsFontReady(font_))
        font_ = LoadFontEx("./resources/fonts/JetBrainsMonoNerdFont-Medium.ttf",
                           HS_FONT_SIZE, NULL, 0);
    data_loaded_ = true;
}

static void init_arrays_(void)
{
    kv_init(high_scores_[D_EASY]);
    kv_init(high_scores_[D_MEDIUM]);
    kv_init(high_scores_[D_HARD]);
}

static void destroy_arrays_(void)
{
    if (data_loaded_) {
        kv_destroy(high_scores_[D_EASY]);
        kv_destroy(high_scores_[D_MEDIUM]);
        kv_destroy(high_scores_[D_HARD]);
        data_loaded_ = false;
    }
}

void unload_high_score_screen(void)
{
    destroy_arrays_();
}

void update_high_score_screen(void)
{
    /* Main menu on ESCAPE */
    if (IsKeyPressed(KEY_ESCAPE)) {
        unload_high_score_screen();
        init_main_menu_screen();
        current_screen = MAIN_MENU;
        return;
    }

    /* Scroll */
    int max_cursor = kv_size(high_scores_[current_tab]) - 1;
    cursor_ -= GetMouseWheelMove();
    cursor_ = cursor_ < 0 ? 0 : cursor_;
    cursor_ = cursor_ > max_cursor ? max_cursor : cursor_;

    /* Clicks */
    Vector2 mouse_pos = GetMousePosition();

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse_pos, get_difficulty_button_(D_EASY))) {
            current_tab = D_EASY;
            cursor_     = 0;
        } else if (CheckCollisionPointRec(mouse_pos,
                                          get_difficulty_button_(D_MEDIUM))) {
            current_tab = D_MEDIUM;
            cursor_     = 0;
        } else if (CheckCollisionPointRec(mouse_pos,
                                          get_difficulty_button_(D_HARD))) {
            current_tab = D_HARD;
            cursor_     = 0;
        }
    }
}

void draw_high_score_screen(void)
{
    draw_high_scores();
    draw_buttons();
}

static void draw_high_scores(void)
{
    char str_time[100];
    char hs[120];
    struct tm tp;
    r_array arr = high_scores_[current_tab];
    for (int i = cursor_; i < kv_size(arr); i++) {
        Record cur  = kv_A(arr, i);
        Vector2 pos = {10, (i - cursor_) * HS_FONT_SIZE};
        time_t ts   = cur.timestamp;
        tp          = *localtime(&ts);
        strftime(str_time, 128, "%Y-%m-%d %H:%M:%S", &tp);
        sprintf(hs, "%7.3f %s", cur.time, str_time);
        DrawTextEx(font_, hs, pos, HS_FONT_SIZE, 1, ACCENT_COLOR);
    }
}

static void draw_buttons(void)
{
    Rectangle dbuttons[]
        = {get_difficulty_button_(D_EASY), get_difficulty_button_(D_MEDIUM),
           get_difficulty_button_(D_HARD)};

    /* Draw difficulty buttons */
    int hs_button_width = (GetScreenWidth() / 3);
    for (int i = D_EASY; i <= D_HARD; i++) {
        int text_width = MeasureText(DIFFICULTIES_STR[i], HS_FONT_SIZE);
        int posx_text = (int)dbuttons[i].x + (hs_button_width - text_width) / 2;
        int posy_text
            = (int)dbuttons[i].y + (HS_BUTTON_HEIGHT - HS_FONT_SIZE) / 2;

        if (CheckCollisionPointRec(GetMousePosition(), dbuttons[i])) {
            DrawRectangle(dbuttons[i].x, dbuttons[i].y, hs_button_width,
                          HS_BUTTON_HEIGHT, HOVER_COLOR);
        } else {
            DrawRectangle(dbuttons[i].x, dbuttons[i].y, hs_button_width,
                          HS_BUTTON_HEIGHT, BACKGROUND_COLOR);
        }
        DrawText(DIFFICULTIES_STR[i], posx_text, posy_text, HS_FONT_SIZE,
                 ACCENT_COLOR);
        /* DrawRectangleLines(dbuttons[i].x, dbuttons[i].y, hs_button_width, */
        /*                    HS_BUTTON_HEIGHT, ACCENT_COLOR); */
    }
}

static Rectangle get_difficulty_button_(int i)
{
    if (i < D_EASY || i > D_HARD)
        return (Rectangle) {0, 0, 0, 0};

    int hs_button_width = (GetScreenWidth() / 3);

    int posx = i * hs_button_width;
    int posy = (GetScreenHeight() - HS_BUTTON_HEIGHT);
    return (Rectangle) {posx, posy, hs_button_width, HS_BUTTON_HEIGHT};
}

void finalize_high_score_screen(void)
{
    UnloadFont(font_);
    destroy_arrays_();
}
