#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

#include "config.h"

typedef enum GameScreen {
    UNKNOWN   = -1,
    MAIN_MENU = 0,
    GAMEPLAY,
} GameScreen;

/* Used for array indexes. Don't change numbers! */
typedef enum Face {
    F_CLICK = 0,
    F_LOST,
    F_SMILE,
    F_SMILE_DOWN,
    F_WIN
} Face;

extern GameScreen current_screen;

extern int sizem;
extern int sizen;
extern int nmines;
extern Difficulty difficulty;

/* Main menu screen */
void init_main_menu_screen(void);
void unload_main_menu_screen(void);
void update_main_menu_screen(void);
void draw_main_menu_screen(void);
bool finish_main_menu_screen(void);

/* Gameplay screen */
void init_gameplay_screen(void);
void unload_gameplay_screen(void);
void update_gameplay_screen(void);
void draw_gameplay_screen(void);
bool finish_gameplay_screen(void);

#endif
