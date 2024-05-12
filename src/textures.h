#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "raylib.h"

extern Texture cells[];   /* Textures for cells */
extern Texture counter[]; /* Textures for digits for counter */
extern Texture faces[];   /* Textures for faces */
extern Texture cup;       /* Textures for cup icon */

void load_textures(void);
void unload_textures(void);

#endif
