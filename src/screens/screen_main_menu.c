#include "drawings.h"
#include "raylib.h"
#include "screens.h"

static Rectangle button_borders;

void init_main_menu_screen(void)
{
    return;
}

void unload_main_menu_screen(void)
{
    return;
}

void update_main_menu_screen(void)
{
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(GetMousePosition(), button_borders)) {
        currentScreen = GAMEPLAY;
    }
}

void draw_main_menu_screen(void)
{
    button_borders = draw_centered_text("START", 50, GREEN);
}
