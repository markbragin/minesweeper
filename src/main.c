#include <stdbool.h>
#include <stdlib.h>

#include "config.h"
#include "db.h"
#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Shared variables */
GameScreen current_screen = UNKNOWN;
bool should_close = false;

/* Local functions */
static void update_draw_frame_(void);

int main(void)
{
    /* Trye to open DB */
    if (db_open()) {
        abort();
    }

    /* Init window */
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Minesweeper");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    load_textures();

    current_screen = MAIN_MENU;
    init_main_menu_screen();

    while (!WindowShouldClose() && !should_close) {
        update_draw_frame_();
    }

    unload_main_menu_screen();
    unload_gameplay_screen();
    unload_high_score_screen();
    unload_textures();
    db_close();

    return 0;
}

static void update_draw_frame_(void)
{
    switch (current_screen) {
    case MAIN_MENU:
        update_main_menu_screen();
        break;
    case GAMEPLAY:
        update_gameplay_screen();
        break;
    case HIGH_SCORE:
        update_high_score_screen();
        break;
    default:
        break;
    }

    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);

    switch (current_screen) {
    case MAIN_MENU:
        draw_main_menu_screen();
        break;
    case GAMEPLAY:
        draw_gameplay_screen();
        break;
    case HIGH_SCORE:
        draw_high_score_screen();
        break;
    default:
        break;
    }

    EndDrawing();
}
