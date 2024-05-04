#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "grid.h"
#include "raylib.h"


const int CELL_SIZE = 40;

bool LOST;
Color get_color(int val);

int main(void)
{
    int m = 15;
    int n = 15;
    int nmines = 15;
    int flags = 0;

    const int SCREEN_WIDTH = CELL_SIZE * n;
    const int SCREEN_HEIGHT = CELL_SIZE * m;

    if (grid_init(m, n, nmines) < 0) {
        fprintf(stderr, "Can't initialize grid\n");
    };
    print_grid(GRID, m, n);
    putchar('\n');
    print_grid(VISIBLE_GRID, m, n);


    Image im_cell1 = LoadImageSvg("resources/svg/cell1.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell2 = LoadImageSvg("resources/svg/cell2.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell3 = LoadImageSvg("resources/svg/cell3.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell4 = LoadImageSvg("resources/svg/cell4.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell5 = LoadImageSvg("resources/svg/cell5.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell6 = LoadImageSvg("resources/svg/cell6.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell7 = LoadImageSvg("resources/svg/cell7.svg", CELL_SIZE, CELL_SIZE);
    Image im_cell8 = LoadImageSvg("resources/svg/cell8.svg", CELL_SIZE, CELL_SIZE);
    Image im_cellup = LoadImageSvg("resources/svg/cellup.svg", CELL_SIZE, CELL_SIZE);
    Image im_celldown = LoadImageSvg("resources/svg/celldown.svg", CELL_SIZE, CELL_SIZE);
    Image im_cellmine = LoadImageSvg("resources/svg/cellmine.svg", CELL_SIZE, CELL_SIZE);
    Image im_cellflag = LoadImageSvg("resources/svg/cellflag.svg", CELL_SIZE, CELL_SIZE);

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


    while (!WindowShouldClose()) {
        BeginDrawing();

        if (IsKeyPressed(KEY_R)) {
            grid_destroy();
            if (grid_init(m, n, nmines) < 0) {
                fprintf(stderr, "Can't initialize grid\n");
            };
            LOST = false;
            flags = 0;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse_pos = GetMousePosition();
            int i = mouse_pos.y / CELL_SIZE;
            int j = mouse_pos.x / CELL_SIZE;
            if (GRID[i * n + j] == C_EMPTY)
                expand(i, j);
            else if (GRID[i * n + j] == C_MINE
                     && VISIBLE_GRID[i * n + j] != C_FLAG) {
                LOST = true;
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                              (Color) {255, 255, 255, 200});
                DrawText("YOU DIED", 100, 180, CELL_SIZE, RED);
            } else if (VISIBLE_GRID[i * n + j] == C_CLOSED)
                open_cell(i, j);
            else
                open_around(i, j);
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse_pos = GetMousePosition();
            int i = mouse_pos.y / CELL_SIZE;
            int j = mouse_pos.x / CELL_SIZE;
            if (VISIBLE_GRID[i * n + j] == C_CLOSED) {
                toggle_flag(i, j);
                if (GRID[i * n + j] == C_MINE) {
                    flags++;
                    if (flags == nmines) {
                        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                      (Color) {255, 255, 255, 200});
                        DrawText("YOU WIN", 100, 180, CELL_SIZE, GREEN);
                    }
                }
            } else if (VISIBLE_GRID[i * n + j] == C_FLAG) {
                toggle_flag(i, j);
                if (GRID[i * n + j] == C_MINE)
                    flags--;
            }
        }

        if (LOST || nmines == flags) {
            EndDrawing();
            continue;
        }

        ClearBackground(WHITE);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                switch (VISIBLE_GRID[i * n + j]) {
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
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
