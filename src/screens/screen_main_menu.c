#include <stdbool.h>

#include "raylib.h"
#include "screens.h"

/* Shared variables */
int M;
int N;
int NMINES;

/* Local (to module) variables */
static int BUTTON_WIDTH  = 200;
static int BUTTON_HEIGHT = 50;
static int BUTTON_GAP    = 20;

static int FONT_SIZE      = 40;
static Color HOVER_COLOR  = {110, 110, 110, 255};
static Color BUTTON_COLOR = BLACK;

static bool FINISH_SCREEN; /* Should screen finish */

/* Returns rectangle of i button, or {0, 0, 0, 0} on bad id.
 * Easy - 1
 * Medium - 2
 * Hard - 3 */
static Rectangle get_button(int i);

void init_main_menu_screen(void)
{
    FINISH_SCREEN = false;
}

void unload_main_menu_screen(void)
{
    M = N = NMINES = 0;
    return;
}

void update_main_menu_screen(void)
{
    Vector2 mouse_pos = GetMousePosition();

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse_pos, get_button(1))) {
            M             = 10;
            N             = 10;
            NMINES        = 10;
            FINISH_SCREEN = true;
        } else if (CheckCollisionPointRec(mouse_pos, get_button(2))) {
            M             = 16;
            N             = 16;
            NMINES        = 40;
            FINISH_SCREEN = true;
        } else if (CheckCollisionPointRec(mouse_pos, get_button(3))) {
            M             = 16;
            N             = 30;
            NMINES        = 99;
            FINISH_SCREEN = true;
        }
    }
}

void draw_main_menu_screen(void)
{
    int text_width, posx_text, posy_text;
    Rectangle but1 = get_button(1);
    Rectangle but2 = get_button(2);
    Rectangle but3 = get_button(3);

    /* Draw first button*/
    text_width = MeasureText("Easy", FONT_SIZE);
    posx_text  = (GetScreenWidth() - text_width) / 2;
    posy_text  = (int)but1.y + (BUTTON_HEIGHT - FONT_SIZE) / 2;
    if (CheckCollisionPointRec(GetMousePosition(), but1))
        DrawRectangle(but1.x, but1.y, BUTTON_WIDTH, BUTTON_HEIGHT, HOVER_COLOR);
    DrawText("Easy", posx_text, posy_text, FONT_SIZE, BUTTON_COLOR);
    DrawRectangleLines(but1.x, but1.y, BUTTON_WIDTH, BUTTON_HEIGHT,
                       BUTTON_COLOR);

    /* Draw second button*/
    text_width = MeasureText("Medium", FONT_SIZE);
    posx_text  = (GetScreenWidth() - text_width) / 2;
    posy_text  = (int)but2.y + (BUTTON_HEIGHT - FONT_SIZE) / 2;
    if (CheckCollisionPointRec(GetMousePosition(), but2))
        DrawRectangle(but2.x, but2.y, BUTTON_WIDTH, BUTTON_HEIGHT, HOVER_COLOR);
    DrawText("Medium", posx_text, posy_text, FONT_SIZE, BUTTON_COLOR);
    DrawRectangleLines(but2.x, but2.y, BUTTON_WIDTH, BUTTON_HEIGHT,
                       BUTTON_COLOR);

    /* Draw third button*/
    text_width = MeasureText("Hard", FONT_SIZE);
    posx_text  = (GetScreenWidth() - text_width) / 2;
    posy_text  = (int)but3.y + (BUTTON_HEIGHT - FONT_SIZE) / 2;
    if (CheckCollisionPointRec(GetMousePosition(), but3))
        DrawRectangle(but3.x, but3.y, BUTTON_WIDTH, BUTTON_HEIGHT, HOVER_COLOR);
    DrawText("Hard", posx_text, posy_text, FONT_SIZE, BUTTON_COLOR);
    DrawRectangleLines(but3.x, but3.y, BUTTON_WIDTH, BUTTON_HEIGHT,
                       BUTTON_COLOR);
}

static Rectangle get_button(int i)
{
    if (i < 0 || i > 3)
        return (Rectangle) {0, 0, 0, 0};

    int posx = ((GetScreenWidth() - BUTTON_WIDTH) / 2);
    int posy = (GetScreenHeight() - BUTTON_HEIGHT * 3 - 2 * BUTTON_GAP) / 2
        + BUTTON_HEIGHT * (i - 1) + BUTTON_GAP * (i - 1);
    return (Rectangle) {posx, posy, BUTTON_WIDTH, BUTTON_HEIGHT};
}

bool finish_main_menu_screen(void)
{
    return FINISH_SCREEN;
}
