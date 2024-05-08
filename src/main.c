#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Shared variables */
GameScreen currentScreen = UNKNOWN;
const int CELL_SIZE      = 40;
const int HEADER_HEIGHT  = 40;
const int COUNTER_WIDTH  = 20;
const int COUNTER_HEIGHT = 40;
const int FACE_SIZE      = 36;

/* Local (to module) variables */
static const int SCREEN_WIDTH  = 400;
static const int SCREEN_HEIGHT = 440;

/* Local functions */
static void update_draw_frame(void);

int main(void)
{
    /* Init window */
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    /* Load textures for cells */
    load_textures();

    currentScreen = MAIN_MENU;
    init_main_menu_screen();
    init_gameplay_screen(10, 10, 10);

    while (!WindowShouldClose()) {
        update_draw_frame();
    }

    unload_gameplay_screen();

    return 0;
}

static void update_draw_frame(void)
{
    switch (currentScreen) {
    case MAIN_MENU:
        update_main_menu_screen();
        break;
    case GAMEPLAY:
        update_gameplay_screen();
        break;
    default:
        break;
    }

    BeginDrawing();
    ClearBackground(GRAY);

    switch (currentScreen) {
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
