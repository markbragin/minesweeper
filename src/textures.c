#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textures.h"
#include "grid.h"
#include "raylib.h"

/* Path for all the media */
#ifndef RESOURCES_DIR
#define RESOURCES_DIR "resources"
#endif


Texture cells[14];
Texture counter[11];
Texture faces[5];

bool textures_loaded = false;

static void load_cells(const char *resources_dir);
static void load_counter(const char *resources_dir);
static void load_faces(const char *resources_dir);

void load_textures(void)
{
    if (textures_loaded)
        return;

    char resources_dir[BUFSIZ];
    if (opendir("resources")) { /* Try relative path first */
        strcpy(resources_dir, "resources");
    } else if (opendir(RESOURCES_DIR)) { /* Try absolute path */
        strcpy(resources_dir, RESOURCES_DIR);
    } else {
        fprintf(stderr, "Can't find resources. Abort.\n");
        abort();
    }

    load_cells(resources_dir);
    load_counter(resources_dir);
    load_faces(resources_dir);
    textures_loaded = true;
}

static void load_cells(const char *resources_dir)
{
    char filepath[BUFSIZ];
    Image im_cells[14];

    sprintf(filepath, "%s/%s", resources_dir, "cells/celldown.png");
    im_cells[C_EMPTY]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cell1.png");
    im_cells[C_ONE]    = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cell2.png");
    im_cells[C_TWO]    = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cell3.png");
    im_cells[C_THREE]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cell4.png");
    im_cells[C_FOUR]   = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cell5.png");
    im_cells[C_FIVE]   = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cell6.png");
    im_cells[C_SIX]    = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cell6.png");
    im_cells[C_SEVEN]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cell8.png");
    im_cells[C_EIGHT]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cellup.png");
    im_cells[C_CLOSED] = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cellmine.png");
    im_cells[C_MINE]   = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/cellflag.png");
    im_cells[C_FLAG]   = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/blast.png");
    im_cells[C_BLAST]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "cells/celldown.png");
    im_cells[C_DOWN]   = LoadImage(filepath);

    for (int i = 0; i < sizeof(im_cells) / sizeof(im_cells[0]); i++) {
        ImageResize(&im_cells[i], CELL_SIZE, CELL_SIZE);
        cells[i] = LoadTextureFromImage(im_cells[i]); /* Create texture */
        UnloadImage(im_cells[i]); /* Unload image since not needed */
    }
}

static void load_counter(const char *resources_dir)
{
    char filepath[BUFSIZ];
    Image im_counter[11];

    sprintf(filepath, "%s/%s", resources_dir, "counter/counter0.png");
    im_counter[0]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter1.png");
    im_counter[1]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter2.png");
    im_counter[2]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter3.png");
    im_counter[3]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter4.png");
    im_counter[4]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter5.png");
    im_counter[5]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter6.png");
    im_counter[6]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter7.png");
    im_counter[7]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter8.png");
    im_counter[8]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter9.png");
    im_counter[9]  = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "counter/counter-.png");
    im_counter[10] = LoadImage(filepath);

    for (int i = 0; i < sizeof(im_counter) / sizeof(im_counter[0]); i++) {
        ImageResize(&im_counter[i], COUNTER_WIDTH, COUNTER_HEIGHT);
        counter[i] = LoadTextureFromImage(im_counter[i]); /* Create texture */
        UnloadImage(im_counter[i]); /* Unload image since not needed */
    }
}

static void load_faces(const char *resources_dir)
{
    char filepath[BUFSIZ];
    Image im_faces[5];

    sprintf(filepath, "%s/%s", resources_dir, "faces/clickface.png");
    im_faces[0] = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "faces/lostface.png");
    im_faces[1] = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "faces/smileface.png");
    im_faces[2] = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "faces/smilefacedown.png");
    im_faces[3] = LoadImage(filepath);
    sprintf(filepath, "%s/%s", resources_dir, "faces/winface.png");
    im_faces[4] = LoadImage(filepath);

    for (int i = 0; i < sizeof(im_faces) / sizeof(im_faces[0]); i++) {
        ImageResize(&im_faces[i], FACE_SIZE, FACE_SIZE);
        faces[i] = LoadTextureFromImage(im_faces[i]); /* Create texture */
        UnloadImage(im_faces[i]); /* Unload image since not needed */
    }
}

void unload_textures(void)
{
    for (int i = 0; i < sizeof(cells) / sizeof(cells[0]); i++)
        UnloadTexture(cells[i]);
    for (int i = 0; i < sizeof(counter) / sizeof(counter[0]); i++)
        UnloadTexture(counter[i]);
    for (int i = 0; i < sizeof(faces) / sizeof(faces[0]); i++)
        UnloadTexture(faces[i]);
}
