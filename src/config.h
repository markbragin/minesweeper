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

extern const Color BACKGROUND_COLOR;
extern const Color ACCENT_COLOR;

/* Used for array indexes. Don't change numbers! */
typedef enum Difficulty {
    D_EASY = 0,
    D_MEDIUM,
    D_HARD
} Difficulty;

/* Sizes of grid of different difficulties */
extern const int SIZEM[3];
extern const int SIZEN[3];
extern const int NMINES[3];

/* Str representation of difficutlty*/
extern const char *DIFFICULTIES_STR[];

extern const int MM_FONT_SIZE;
extern const Color HOVER_COLOR;

/* Main menu buttons */
extern const int MM_BUTTON_WIDTH;
extern const int MM_BUTTON_HEIGHT;
extern const int MM_BUTTON_GAP;

/* High score icon */
extern const int CUP_ICON_SIZE;
extern const int HS_BUTTON_HEIGHT;
extern const int HS_FONT_SIZE;

#endif
