#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "grid.h"
#include "raylib.h"

int main(void)
{
    bool lost = false;
    int m = 16;
    int n = 16;
    int cell_size = 40;
    int nmines = 40;
    int nopened = 0;

    const int SCREEN_WIDTH = cell_size * n;
    const int SCREEN_HEIGHT = cell_size * m;

    if (grid_init(m, n, nmines) < 0) {
        fprintf(stderr, "Can't initialize grid\n");
        abort();
    };

    Image im_cell1
        = LoadImageSvg("resources/svg/cell1.svg", cell_size, cell_size);
    Image im_cell2
        = LoadImageSvg("resources/svg/cell2.svg", cell_size, cell_size);
    Image im_cell3
        = LoadImageSvg("resources/svg/cell3.svg", cell_size, cell_size);
    Image im_cell4
        = LoadImageSvg("resources/svg/cell4.svg", cell_size, cell_size);
    Image im_cell5
        = LoadImageSvg("resources/svg/cell5.svg", cell_size, cell_size);
    Image im_cell6
        = LoadImageSvg("resources/svg/cell6.svg", cell_size, cell_size);
    Image im_cell7
        = LoadImageSvg("resources/svg/cell7.svg", cell_size, cell_size);
    Image im_cell8
        = LoadImageSvg("resources/svg/cell8.svg", cell_size, cell_size);
    Image im_cellup
        = LoadImageSvg("resources/svg/cellup.svg", cell_size, cell_size);
    Image im_celldown
        = LoadImageSvg("resources/svg/celldown.svg", cell_size, cell_size);
    Image im_cellmine
        = LoadImageSvg("resources/svg/cellmine.svg", cell_size, cell_size);
    Image im_cellflag
        = LoadImageSvg("resources/svg/cellflag.svg", cell_size, cell_size);
    Image im_cellblast
        = LoadImageSvg("resources/svg/blast.svg", cell_size, cell_size);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper");
    SetTargetFPS(60);

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
        if (IsKeyPressed(KEY_R)) {
            grid_destroy();
            if (grid_init(m, n, nmines) < 0) {
                fprintf(stderr, "Can't initialize grid\n");
                abort();
            };
            lost = false;
            nopened = 0;
        }
        if (!lost && nopened + nmines != m * n) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse_pos = GetMousePosition();
                int i = mouse_pos.y / cell_size;
                int j = mouse_pos.x / cell_size;
                int idx = i * n + j;
                int cur_nopened = 0;

                set_easy_flags(i, j);
                nopened += open_cell(i, j);

                if (VISIBLE_GRID[idx] == C_EMPTY)
                    nopened += expand(i, j);

                cur_nopened = open_around(i, j);
                if (VISIBLE_GRID[idx] == C_BLAST || cur_nopened == -1) {
                    lost = true;
                    open_mines();
                }
                nopened += cur_nopened;
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                Vector2 mouse_pos = GetMousePosition();
                int i = mouse_pos.y / cell_size;
                int j = mouse_pos.x / cell_size;
                int idx = i * n + j;
                if (VISIBLE_GRID[idx] == C_CLOSED) {
                    toggle_flag(i, j);
                    if (nopened + nmines == m * n)
                        open_safe_cells();
                } else if (VISIBLE_GRID[idx] == C_FLAG) {
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
                    DrawTexture(cellblast, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_CLOSED:
                    DrawTexture(cellup, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_FLAG:
                    DrawTexture(cellflag, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_MINE:
                    DrawTexture(cellmine, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_EMPTY:
                    DrawTexture(celldown, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_ONE:
                    DrawTexture(cell1, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_TWO:
                    DrawTexture(cell2, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_THREE:
                    DrawTexture(cell3, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_FOUR:
                    DrawTexture(cell4, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_FIVE:
                    DrawTexture(cell5, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_SIX:
                    DrawTexture(cell6, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_SEVEN:
                    DrawTexture(cell7, j * cell_size, i * cell_size, GRAY);
                    break;
                case C_EIGHT:
                    DrawTexture(cell8, j * cell_size, i * cell_size, GRAY);
                    break;
                }
            }
        }

        if (nopened + nmines == m * n) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                          (Color) {255, 255, 255, 100});
            DrawText("You win", 100, 180, cell_size, GREEN);
        } else if (lost) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                          (Color) {255, 255, 255, 100});
            DrawText("You lost", 100, 180, cell_size, RED);
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
