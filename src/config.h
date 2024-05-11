#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "raylib.h"

extern const int DEFAULT_SCREEN_WIDTH;
extern const int DEFAULT_SCREEN_HEIGHT;
extern const int HEADER_HEIGHT;
extern const int FACE_SIZE;
extern const int COUNTER_WIDTH;
extern const int COUNTER_HEIGHT;
extern const int CELL_SIZE;

/* Used for array indexes. Don't change numbers! */
typedef enum Difficulty {
    D_EASY = 0,
    D_MEDIUM,
    D_HARD
} Difficulty;

extern const char *DIFFICULTIES_STR[];

extern int FONT_SIZE;
extern Color HOVER_COLOR;
extern Color BUTTON_COLOR;

/* Main menu buttons */
extern int MM_BUTTON_WIDTH;
extern int MM_BUTTON_HEIGHT;
extern int MM_BUTTON_GAP;

#endif
