#include <stdbool.h>

#include "config.h"
#include "raylib.h"
#include "screens.h"

/* Shared variables */
int sizem;
int sizen;
int nmines;
Difficulty difficulty;

static bool finish_screen_; /* Should screen finish */

/* Returns rectangle of i button, or {0, 0, 0, 0} on bad id.
 * Easy - 0 (D_EASY)
 * Medium - 1 (D_MEDIUM)
 * Hard - 2 (D_HARD) */
static Rectangle get_button_(int i);

void init_main_menu_screen(void)
{
    finish_screen_ = false;
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
            sizem         = 10;
            sizen         = 10;
            nmines        = 10;
            difficulty    = D_EASY;
            finish_screen_ = true;
        } else if (CheckCollisionPointRec(mouse_pos, get_button_(D_MEDIUM))) {
            sizem         = 16;
            sizen         = 16;
            nmines        = 40;
            difficulty    = D_MEDIUM;
            finish_screen_ = true;
        } else if (CheckCollisionPointRec(mouse_pos, get_button_(D_HARD))) {
            sizem         = 16;
            sizen         = 30;
            nmines        = 99;
            difficulty    = D_HARD;
            finish_screen_ = true;
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

bool finish_main_menu_screen(void)
{
    return finish_screen_;
}
