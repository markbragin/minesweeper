#include "config.h"
#include "raylib.h"

const int CELL_SIZE      = 30;
const int HEADER_HEIGHT  = 40;
const int COUNTER_WIDTH  = 20;
const int COUNTER_HEIGHT = 40;
const int FACE_SIZE      = 36;

const int DEFAULT_SCREEN_WIDTH  = CELL_SIZE * 10;
const int DEFAULT_SCREEN_HEIGHT = CELL_SIZE * 10 + HEADER_HEIGHT;

const char *DIFFICULTIES_STR[3] = {"Easy", "Medium", "Hard"};

int FONT_SIZE      = 40;
Color HOVER_COLOR  = {110, 110, 110, 255};
Color BUTTON_COLOR = BLACK;

/* Main menu buttons */
int MM_BUTTON_WIDTH  = 200;
int MM_BUTTON_HEIGHT = 50;
int MM_BUTTON_GAP    = 20;
