#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "raylib.h"

extern const int CELL_SIZE;
extern const int COUNTER_WIDTH;
extern const int COUNTER_HEIGHT;
extern const int FACE_SIZE;

extern Texture cells[];    /* Textures for cells */
extern Texture counter[];  /* Textures for digits for counter */
extern Texture faces[];     /* Textures for faces */
extern bool textures_loaded; /* Already loaded? */

void load_textures(void);
void unload_textures(void);

#endif
