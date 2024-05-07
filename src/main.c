#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "grid.h"
#include "raylib.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int CELL_SIZE;

void draw_centered_text(const char *text, int font_size, Color color);
void dim_screen(void);

int main(void)
{
    bool alive = true;
    bool won = false;
    bool first_click = true;
    int m = 10;
    int n = 10;
    int nmines = 10;
    int nopened = 0;
    double start_time, end_time;

    char message[256];

    CELL_SIZE = 40;
    SCREEN_WIDTH = CELL_SIZE * n;
    SCREEN_HEIGHT = CELL_SIZE * m;

    if (grid_init(m, n, nmines) < 0) {
        fprintf(stderr, "Can't initialize grid\n");
        abort();
    };

    /* Images for cells */
    Image im_cell1
        = LoadImageSvg("resources/svg/cell1.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell2
        = LoadImageSvg("resources/svg/cell2.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell3
        = LoadImageSvg("resources/svg/cell3.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell4
        = LoadImageSvg("resources/svg/cell4.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell5
        = LoadImageSvg("resources/svg/cell5.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell6
        = LoadImageSvg("resources/svg/cell6.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell7
        = LoadImageSvg("resources/svg/cell7.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell8
        = LoadImageSvg("resources/svg/cell8.svg", CELL_SIZE, CELL_SIZE);
    Image im_cellup
        = LoadImageSvg("resources/svg/cellup.svg", CELL_SIZE, CELL_SIZE);
    Image im_celldown
        = LoadImageSvg("resources/svg/celldown.svg", CELL_SIZE, CELL_SIZE);
    Image im_cellmine
        = LoadImageSvg("resources/svg/cellmine.svg", CELL_SIZE, CELL_SIZE);
    Image im_cellflag
        = LoadImageSvg("resources/svg/cellflag.svg", CELL_SIZE, CELL_SIZE);
    Image im_cellblast
        = LoadImageSvg("resources/svg/blast.svg", CELL_SIZE, CELL_SIZE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper");
    SetTargetFPS(60);

    /* Textures for cells */
    Texture2D cell1 = LoadTextureFromImage(im_cell1);
    Texture2D cell2 = LoadTextureFromImage(im_cell2);
    Texture2D cell3 = LoadTextureFromImage(im_cell3);
    Texture2D cell4 = LoadTextureFromImage(im_cell4);
    Texture2D cell5 = LoadTextureFromImage(im_cell5);
    Texture2D cell6 = LoadTextureFromImage(im_cell6);
    Texture2D cell7 = LoadTextureFromImage(im_cell7);
    Texture2D cell8 = LoadTextureFromImage(im_cell8);
    Texture2D cellup = LoadTextureFromImage(im_cellup);
    Texture2D celldown = LoadTextureFromImage(im_celldown);
    Texture2D cellmine = LoadTextureFromImage(im_cellmine);
    Texture2D cellflag = LoadTextureFromImage(im_cellflag);
    Texture2D cellblast = LoadTextureFromImage(im_cellblast);

    UnloadImage(im_cell1);
    UnloadImage(im_cell2);
    UnloadImage(im_cell3);
    UnloadImage(im_cell4);
    UnloadImage(im_cell5);
    UnloadImage(im_cell6);
    UnloadImage(im_cell7);
    UnloadImage(im_cell8);
    UnloadImage(im_cellup);
    UnloadImage(im_celldown);
    UnloadImage(im_cellmine);
    UnloadImage(im_cellflag);
    UnloadImage(im_cellblast);

    while (!WindowShouldClose()) {
        /* Reload game on KEY_R press */
        if (IsKeyPressed(KEY_R)) {
            grid_destroy();
            if (grid_init(m, n, nmines) < 0) {
                fprintf(stderr, "Can't initialize grid\n");
                abort();
            };
            won = false;
            alive = true;
            nopened = 0;
            first_click = true;
            start_time = 0;
            end_time = 0;
        }
        if (alive && !won) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse_pos = GetMousePosition();
                int i = mouse_pos.y / CELL_SIZE;
                int j = mouse_pos.x / CELL_SIZE;
                int idx = i * n + j;
                int cur_nopened = 0;

                if (first_click) {
                    first_click = false;
                    start_time = GetTime();
                }

                nopened += open_cell(i, j);

                if (VISIBLE_GRID[idx] == C_EMPTY)
                    nopened += discover(i, j);

                cur_nopened = open_around(i, j);
                if (VISIBLE_GRID[idx] == C_BLAST || cur_nopened == -1) {
                    alive = false;
                    open_mines();
                }
                nopened += cur_nopened;

                if (nopened + nmines == m * n) {
                    won = true;
                    end_time = GetTime();
                    open_safe_cells();
                    set_safe_flags();
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                Vector2 mouse_pos = GetMousePosition();
                int i = mouse_pos.y / CELL_SIZE;
                int j = mouse_pos.x / CELL_SIZE;
                int idx = i * n + j;
                if (VISIBLE_GRID[idx] == C_CLOSED
                    || VISIBLE_GRID[idx] == C_FLAG) {
                    toggle_flag(i, j);
                }
            }
        }

        BeginDrawing();
        ClearBackground(WHITE);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                switch (VISIBLE_GRID[i * n + j]) {
                case C_BLAST:
                    DrawTexture(cellblast, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_CLOSED:
                    DrawTexture(cellup, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_FLAG:
                    DrawTexture(cellflag, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_MINE:
                    DrawTexture(cellmine, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_EMPTY:
                    DrawTexture(celldown, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_ONE:
                    DrawTexture(cell1, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_TWO:
                    DrawTexture(cell2, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_THREE:
                    DrawTexture(cell3, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_FOUR:
                    DrawTexture(cell4, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_FIVE:
                    DrawTexture(cell5, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_SIX:
                    DrawTexture(cell6, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_SEVEN:
                    DrawTexture(cell7, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                case C_EIGHT:
                    DrawTexture(cell8, j * CELL_SIZE, i * CELL_SIZE, GRAY);
                    break;
                }
            }
        }

        if (won) {
            sprintf(message, "You won! %.3f", (end_time - start_time));
            dim_screen();
            draw_centered_text(message, CELL_SIZE, GREEN);
        } else if (!alive) {
            dim_screen();
            draw_centered_text("You lost", CELL_SIZE, RED);
        }

        EndDrawing();
    }

    UnloadTexture(cell1);
    UnloadTexture(cell2);
    UnloadTexture(cell3);
    UnloadTexture(cell4);
    UnloadTexture(cell5);
    UnloadTexture(cell6);
    UnloadTexture(cell7);
    UnloadTexture(cell8);
    UnloadTexture(cellup);
    UnloadTexture(celldown);
    UnloadTexture(cellmine);
    UnloadTexture(cellflag);
    UnloadTexture(cellblast);

    CloseWindow();

    return 0;
}

void draw_centered_text(const char *text, int font_size, Color color)
{
    int tx, ty;
    Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 2);

    tx = (SCREEN_WIDTH - text_size.x) / 2;
    ty = (SCREEN_HEIGHT - text_size.y) / 2;
    DrawTextEx(GetFontDefault(), text, (Vector2) {tx, ty}, CELL_SIZE, 2,
               color);
}

void dim_screen(void)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {0, 0, 0, 100});
}
