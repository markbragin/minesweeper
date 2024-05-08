#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "raylib.h"

extern const int CELL_SIZE;

extern Texture cells[13];   /* Textures for cells */
extern bool textures_loaded; /* Already loaded? */

void load_textures(void);
void unload_textures(void);

#endif
