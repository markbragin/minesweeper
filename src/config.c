#include "config.h"
#include "raylib.h"

const int CELL_SIZE      = 30;
const int HEADER_HEIGHT  = 40;
const int COUNTER_WIDTH  = 20;
const int COUNTER_HEIGHT = 40;
const int FACE_SIZE      = 36;

const int DEFAULT_SCREEN_WIDTH  = CELL_SIZE * 10;
const int DEFAULT_SCREEN_HEIGHT = CELL_SIZE * 10 + HEADER_HEIGHT;

const int SIZEM[3]  = {10, 16, 16};
const int SIZEN[3]  = {10, 16, 30};
const int NMINES[3] = {10, 40, 99};

const char *DIFFICULTIES_STR[3] = {"Easy", "Medium", "Hard"};

const int FONT_SIZE      = 40;
const Color HOVER_COLOR  = {110, 110, 110, 255};
const Color BUTTON_COLOR = {0, 0, 0, 255};

/* Main menu buttons */
const int MM_BUTTON_WIDTH  = 200;
const int MM_BUTTON_HEIGHT = 50;
const int MM_BUTTON_GAP    = 20;
