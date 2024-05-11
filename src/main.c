#include "config.h"
#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Shared variables */
GameScreen CURRENT_SCREEN = UNKNOWN;

/* Local functions */
static void update_draw_frame(void);

int main(void)
{
    /* Init window */
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Minesweeper");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    load_textures();

    CURRENT_SCREEN = MAIN_MENU;
    init_main_menu_screen();

    while (!WindowShouldClose()) {
        update_draw_frame();
    }

    unload_main_menu_screen();
    unload_gameplay_screen();
    unload_textures();

    return 0;
}

static void update_draw_frame(void)
{
    switch (CURRENT_SCREEN) {
    case MAIN_MENU: {
        update_main_menu_screen();
        if (finish_main_menu_screen()) {
            int width  = SIZEN * CELL_SIZE;
            int height = HEADER_HEIGHT + SIZEM * CELL_SIZE;
            SetWindowMinSize(width, height);
            SetWindowSize(width, height);
            init_gameplay_screen();
            CURRENT_SCREEN = GAMEPLAY;
        }
        break;
    }
    case GAMEPLAY: {
        update_gameplay_screen();
        if (finish_gameplay_screen()) {
            init_main_menu_screen();
            CURRENT_SCREEN = MAIN_MENU;
            SetWindowMinSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
            SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
        }
    }
    default:
        break;
    }

    BeginDrawing();
    ClearBackground(GRAY);

    switch (CURRENT_SCREEN) {
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
