#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

    Rectangle *cells = malloc(m * m * sizeof(Rectangle));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            cells[i * n + j] = (Rectangle) {j * CELL_SIZE, i * CELL_SIZE,
                                            CELL_SIZE, CELL_SIZE};

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minesweeper");
    SetTargetFPS(60);

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
                    DrawText("#", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(C_CLOSED));
                    break;
                case C_FLAG:
                    DrawText("!", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(C_FLAG));
                    break;
                case C_MINE:
                    DrawText("*", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(C_MINE));
                    break;
                case C_EMPTY:
                    DrawText("", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(0));
                    break;
                case 1:
                    DrawText("1", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(1));
                    break;
                case 2:
                    DrawText("2", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(2));
                    break;
                case 3:
                    DrawText("3", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(3));
                    break;
                case 4:
                    DrawText("4", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(4));
                    break;
                case 5:
                    DrawText("5", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(5));
                    break;
                case 6:
                    DrawText("6", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(6));
                    break;
                case 7:
                    DrawText("7", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(7));
                    break;
                case 8:
                    DrawText("8", j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE,
                             CELL_SIZE, get_color(8));
                    break;
                }
                DrawRectangleLinesEx(cells[i * n + j], 0.5, BLACK);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    free(cells);

    return 0;
}

Color get_color(int val)
{
    switch (val) {
    case C_ONE:
        return BLUE;
    case C_TWO:
        return GREEN;
    case C_THREE:
        return PINK;
    case C_FOUR:
        return MAGENTA;
    case C_FIVE:
        return ORANGE;
    case C_SIX:
        return ORANGE;
    case C_SEVEN:
        return ORANGE;
    case C_EIGHT:
        return ORANGE;
    case C_CLOSED:
        return GRAY;
    case C_FLAG:
    case C_MINE:
        return RED;
    case C_EMPTY:
        return WHITE;
    default:
        return GRAY;
    }
}
