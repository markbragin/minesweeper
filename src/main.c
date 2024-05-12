#include <stdlib.h>

#include "config.h"
#include "db.h"
#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Shared variables */
GameScreen current_screen = UNKNOWN;

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

    while (!WindowShouldClose()) {
        update_draw_frame_();
    }

    unload_main_menu_screen();
    unload_gameplay_screen();
    unload_textures();
    db_close();

    return 0;
}

static void update_draw_frame_(void)
{
    switch (current_screen) {
    case MAIN_MENU: {
        update_main_menu_screen();
        if (finish_main_menu_screen()) {
            int width  = sizen * CELL_SIZE;
            int height = HEADER_HEIGHT + sizem * CELL_SIZE;
            SetWindowMinSize(width, height);
            SetWindowSize(width, height);
            init_gameplay_screen();
            current_screen = GAMEPLAY;
        }
        break;
    }
    case GAMEPLAY: {
        update_gameplay_screen();
        if (finish_gameplay_screen()) {
            init_main_menu_screen();
            current_screen = MAIN_MENU;
            SetWindowMinSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
            SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
        }
    }
    default:
        break;
    }

    BeginDrawing();
    ClearBackground(GRAY);

    switch (current_screen) {
    case MAIN_MENU:
        draw_main_menu_screen();
        break;
    case GAMEPLAY:
        draw_gameplay_screen();
        break;
    default:
        break;
    }

    EndDrawing();
}
