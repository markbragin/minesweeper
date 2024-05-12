#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

#include "config.h"

typedef enum GameScreen {
    UNKNOWN = -1,
    MAIN_MENU,
    GAMEPLAY,
    HIGH_SCORE
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
extern bool should_close;

/* Main menu screen */
void init_main_menu_screen(void);
void unload_main_menu_screen(void);
void update_main_menu_screen(void);
void draw_main_menu_screen(void);

/* Gameplay screen */
void init_gameplay_screen(Difficulty difficulty);
void unload_gameplay_screen(void);
void update_gameplay_screen(void);
void draw_gameplay_screen(void);

/* High score screen */
void init_high_score_screen(void);
void unload_high_score_screen(void);
void update_high_score_screen(void);
void draw_high_score_screen(void);

#endif
