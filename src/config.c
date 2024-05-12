#include "config.h"
#include "raylib.h"

const int CELL_SIZE      = 30;
const int HEADER_HEIGHT  = 40;
const int COUNTER_WIDTH  = 20;
const int COUNTER_HEIGHT = 40;
const int FACE_SIZE      = 36;

const int DEFAULT_SCREEN_WIDTH  = CELL_SIZE * 10;
const int DEFAULT_SCREEN_HEIGHT = CELL_SIZE * 10 + HEADER_HEIGHT;

const Color BACKGROUND_COLOR = {200, 200, 200, 255};
const Color ACCENT_COLOR     = {0, 0, 0, 255};

const int SIZEM[3]  = {10, 16, 16};
const int SIZEN[3]  = {10, 16, 30};
const int NMINES[3] = {10, 40, 99};

const char *DIFFICULTIES_STR[3] = {"Easy", "Medium", "Hard"};

const int MM_FONT_SIZE  = 40;
const Color HOVER_COLOR = {110, 110, 110, 255};

/* Main menu buttons */
const int MM_BUTTON_WIDTH  = 200;
const int MM_BUTTON_HEIGHT = 50;
const int MM_BUTTON_GAP    = 20;

const int CUP_ICON_SIZE = MM_BUTTON_HEIGHT;

const int HS_BUTTON_HEIGHT = MM_BUTTON_HEIGHT;
const int HS_FONT_SIZE     = MM_FONT_SIZE / 2;
