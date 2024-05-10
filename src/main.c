#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Grid parameters from MAIN_MENU */
extern int M;
extern int N;
extern int NMINES;

/* Shared variables */
GameScreen CURRENT_SCREEN = UNKNOWN;
const int CELL_SIZE       = 30;
const int HEADER_HEIGHT   = 40;
const int COUNTER_WIDTH   = 20;
const int COUNTER_HEIGHT  = 40;
const int FACE_SIZE       = 36;

/* Local (to module) variables */
static const int SCREEN_WIDTH  = CELL_SIZE * 10;
static const int SCREEN_HEIGHT = CELL_SIZE * 10 + HEADER_HEIGHT;

/* Local functions */
static void update_draw_frame(void);

int main(void)
{
    /* Init window */
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    /* Load textures for cells */
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
            int width  = N * CELL_SIZE;
            int height = HEADER_HEIGHT + M * CELL_SIZE;
            SetWindowMinSize(width, height);
            SetWindowSize(width, height);
            init_gameplay_screen(M, N, NMINES);
            CURRENT_SCREEN = GAMEPLAY;
        }
        break;
    }
    case GAMEPLAY: {
        update_gameplay_screen();
        if (finish_gameplay_screen()) {
            init_main_menu_screen();
            CURRENT_SCREEN = MAIN_MENU;
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
