#ifndef SCREENS_H
#define SCREENS_H

typedef enum GameScreen {
    UNKNOWN = -1,
    MAIN_MENU = 0,
    GAMEPLAY,
} GameScreen;

extern GameScreen currentScreen;
extern const int CELL_SIZE;
extern const int COUNTER_WIDTH;
extern const int COUNTER_HEIGHT;
extern const int HEADER_HEIGHT;

/* Main menu screen */
void init_main_menu_screen(void);
void unload_main_menu_screen(void);
void update_main_menu_screen(void);
void draw_main_menu_screen(void);

/* Gameplay screen */
void init_gameplay_screen(int m, int n, int nmines);
void unload_gameplay_screen(void);
void update_gameplay_screen(void);
void draw_gameplay_screen(void);

#endif
