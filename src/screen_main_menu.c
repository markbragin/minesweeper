#include "config.h"
#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Returns rectangle of i button, or {0, 0, 0, 0} on bad id.
 * Easy - 0 (D_EASY)
 * Medium - 1 (D_MEDIUM)
 * Hard - 2 (D_HARD) */
static Rectangle get_difficulty_button_(int i);

static Rectangle get_high_score_button_(void);

void init_main_menu_screen(void)
{
    SetWindowMinSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    SetWindowSize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
}

void unload_main_menu_screen(void)
{
    return;
}

void update_main_menu_screen(void)
{
    Vector2 mouse_pos = GetMousePosition();

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse_pos, get_difficulty_button_(D_EASY))) {
            init_gameplay_screen(D_EASY);
            current_screen = GAMEPLAY;
        } else if (CheckCollisionPointRec(mouse_pos,
                                          get_difficulty_button_(D_MEDIUM))) {
            init_gameplay_screen(D_MEDIUM);
            current_screen = GAMEPLAY;
        } else if (CheckCollisionPointRec(mouse_pos,
                                          get_difficulty_button_(D_HARD))) {
            init_gameplay_screen(D_HARD);
            current_screen = GAMEPLAY;
        } else if (CheckCollisionPointRec(mouse_pos,
                                          get_high_score_button_())) {
            init_high_score_screen();
            current_screen = HIGH_SCORE;
        }
    }
}

void draw_main_menu_screen(void)
{
    Rectangle hs_button = get_high_score_button_();
    Rectangle dbuttons[]
        = {get_difficulty_button_(D_EASY), get_difficulty_button_(D_MEDIUM),
           get_difficulty_button_(D_HARD)};

    /* Draw high score button */
    DrawTexture(cup, hs_button.x, hs_button.y, BACKGROUND_COLOR);

    /* Draw difficulty buttons */
    for (int i = D_EASY; i <= D_HARD; i++) {
        int text_width = MeasureText(DIFFICULTIES_STR[i], MM_FONT_SIZE);
        int posx_text  = (GetScreenWidth() - text_width) / 2;
        int posy_text
            = (int)dbuttons[i].y + (MM_BUTTON_HEIGHT - MM_FONT_SIZE) / 2;
        if (CheckCollisionPointRec(GetMousePosition(), dbuttons[i])) {
            DrawRectangle(dbuttons[i].x, dbuttons[i].y, MM_BUTTON_WIDTH,
                          MM_BUTTON_HEIGHT, HOVER_COLOR);
        }
        DrawText(DIFFICULTIES_STR[i], posx_text, posy_text, MM_FONT_SIZE,
                 ACCENT_COLOR);
        DrawRectangleLines(dbuttons[i].x, dbuttons[i].y, MM_BUTTON_WIDTH,
                           MM_BUTTON_HEIGHT, ACCENT_COLOR);
    }
}

static Rectangle get_difficulty_button_(int i)
{
    if (i < D_EASY || i > D_HARD)
        return (Rectangle) {0, 0, 0, 0};

    int posx = ((GetScreenWidth() - MM_BUTTON_WIDTH) / 2);
    int posy
        = (GetScreenHeight() - MM_BUTTON_HEIGHT * 3 - 2 * MM_BUTTON_GAP) / 2
        + MM_BUTTON_HEIGHT * i + MM_BUTTON_GAP * i;
    return (Rectangle) {posx, posy, MM_BUTTON_WIDTH, MM_BUTTON_HEIGHT};
}

static Rectangle get_high_score_button_(void)
{
    int posx = (GetScreenWidth() - CUP_ICON_SIZE - 10);
    int posy = 10;
    return (Rectangle) {posx, posy, CUP_ICON_SIZE, CUP_ICON_SIZE};
}
