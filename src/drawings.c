#include "drawings.h"
#include "raylib.h"

Rectangle draw_centered_text(const char *text, int font_size, Color color)
{
    int tx, ty;
    Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 2);

    tx = (GetScreenWidth() - text_size.x) / 2;
    ty = (GetScreenHeight() - text_size.y) / 2;
    DrawTextEx(GetFontDefault(), text, (Vector2) {tx, ty}, font_size, 2, color);

    return (Rectangle) {tx, ty, text_size.x, text_size.y};
}

void dim_area(Rectangle area)
{
    Color color = (Color) {0, 0, 0, 200};
    DrawRectangleRec(area, color);
}
