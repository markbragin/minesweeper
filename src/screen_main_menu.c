#include <stdbool.h>

#include "config.h"
#include "raylib.h"
#include "screens.h"

/* Returns rectangle of i button, or {0, 0, 0, 0} on bad id.
 * Easy - 0 (D_EASY)
 * Medium - 1 (D_MEDIUM)
 * Hard - 2 (D_HARD) */
static Rectangle get_button_(int i);

void init_main_menu_screen(void)
{
    SetWindowMinSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    return;
}

void unload_main_menu_screen(void)
{
    return;
}

void update_main_menu_screen(void)
{
    Vector2 mouse_pos = GetMousePosition();

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse_pos, get_button_(D_EASY))) {
            init_gameplay_screen(D_EASY);
            current_screen = GAMEPLAY;
        } else if (CheckCollisionPointRec(mouse_pos, get_button_(D_MEDIUM))) {
            init_gameplay_screen(D_MEDIUM);
            current_screen = GAMEPLAY;
        } else if (CheckCollisionPointRec(mouse_pos, get_button_(D_HARD))) {
            init_gameplay_screen(D_HARD);
            current_screen = GAMEPLAY;
        }
    }
}

void draw_main_menu_screen(void)
{
    int text_width, posx_text, posy_text;
    Rectangle buttons[]
        = {get_button_(D_EASY), get_button_(D_MEDIUM), get_button_(D_HARD)};

    /* Draw buttons */
    for (int i = D_EASY; i <= D_HARD; i++) {
        text_width = MeasureText(DIFFICULTIES_STR[i], FONT_SIZE);
        posx_text  = (GetScreenWidth() - text_width) / 2;
        posy_text  = (int)buttons[i].y + (MM_BUTTON_HEIGHT - FONT_SIZE) / 2;
        if (CheckCollisionPointRec(GetMousePosition(), buttons[i])) {
            DrawRectangle(buttons[i].x, buttons[i].y, MM_BUTTON_WIDTH,
                          MM_BUTTON_HEIGHT, HOVER_COLOR);
        }
        DrawText(DIFFICULTIES_STR[i], posx_text, posy_text, FONT_SIZE,
                 BUTTON_COLOR);
        DrawRectangleLines(buttons[i].x, buttons[i].y, MM_BUTTON_WIDTH,
                           MM_BUTTON_HEIGHT, BUTTON_COLOR);
    }
}

static Rectangle get_button_(int i)
{
    if (i < 0 || i > 2)
        return (Rectangle) {0, 0, 0, 0};

    int posx = ((GetScreenWidth() - MM_BUTTON_WIDTH) / 2);
    int posy
        = (GetScreenHeight() - MM_BUTTON_HEIGHT * 3 - 2 * MM_BUTTON_GAP) / 2
        + MM_BUTTON_HEIGHT * i + MM_BUTTON_GAP * i;
    return (Rectangle) {posx, posy, MM_BUTTON_WIDTH, MM_BUTTON_HEIGHT};
}
