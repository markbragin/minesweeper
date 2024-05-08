#ifndef SCREENS_H
#define SCREENS_H

typedef enum GameScreen {
    UNKNOWN = -1,
    MAIN_MENU = 0,
    GAMEPLAY,
} GameScreen;

extern GameScreen currentScreen;
extern const int CELL_SIZE;

/* Gameplay screen */
void init_gameplay_screen(int m, int n, int nmines);
void unload_gameplay_screen(void);
void update_gameplay_screen(void);
void draw_gameplay_screen(void);

#endif
