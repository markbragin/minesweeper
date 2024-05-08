#include <stdbool.h>

#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Shared variables */
GameScreen currentScreen = UNKNOWN;
const int CELL_SIZE      = 40;

/* Local (to module) variables */
static const int SCREEN_WIDTH  = 400;
static const int SCREEN_HEIGHT = 400;

/* Local functions */
static void update_draw_frame(void);

int main(void)
{
    /* Init window */
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper");
    SetWindowMinSize(400, 400);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    /* Load textures for cells */
    load_textures();

    currentScreen = GAMEPLAY;
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
    case GAMEPLAY:
        update_gameplay_screen();
        break;
    default:
        break;
    }

    BeginDrawing();
    ClearBackground(GRAY);

    switch (currentScreen) {
    case GAMEPLAY:
        draw_gameplay_screen();
        break;
    default:
        break;
    }

    EndDrawing();
}
