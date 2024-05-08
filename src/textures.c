#include "textures.h"
#include "grid.h"
#include "raylib.h"

/* Path for all the media */
#ifndef RESOURCES_DIR
#define RESOURCES_DIR "resources"
#endif

Texture cells[13];
Texture counter[11];
Texture faces[5];

bool textures_loaded = false;

static void load_cells(void);
static void load_counter(void);
static void load_faces(void);

void load_textures(void)
{
    if (textures_loaded)
        return;

    load_cells();
    load_counter();
    load_faces();
    textures_loaded = true;
}

static void load_cells(void)
{
    Image im_cells[13];
    im_cells[C_EMPTY]  = LoadImage(RESOURCES_DIR "/cells/celldown.png");
    im_cells[C_ONE]    = LoadImage(RESOURCES_DIR "/cells/cell1.png");
    im_cells[C_TWO]    = LoadImage(RESOURCES_DIR "/cells/cell2.png");
    im_cells[C_THREE]  = LoadImage(RESOURCES_DIR "/cells/cell3.png");
    im_cells[C_FOUR]   = LoadImage(RESOURCES_DIR "/cells/cell4.png");
    im_cells[C_FIVE]   = LoadImage(RESOURCES_DIR "/cells/cell5.png");
    im_cells[C_SIX]    = LoadImage(RESOURCES_DIR "/cells/cell6.png");
    im_cells[C_SEVEN]  = LoadImage(RESOURCES_DIR "/cells/cell7.png");
    im_cells[C_EIGHT]  = LoadImage(RESOURCES_DIR "/cells/cell8.png");
    im_cells[C_CLOSED] = LoadImage(RESOURCES_DIR "/cells/cellup.png");
    im_cells[C_MINE]   = LoadImage(RESOURCES_DIR "/cells/cellmine.png");
    im_cells[C_FLAG]   = LoadImage(RESOURCES_DIR "/cells/cellflag.png");
    im_cells[C_BLAST]  = LoadImage(RESOURCES_DIR "/cells/blast.png");

    for (int i = 0; i < 13; i++) {
        ImageResize(&im_cells[i], CELL_SIZE, CELL_SIZE);
        cells[i] = LoadTextureFromImage(im_cells[i]); /* Create texture */
        UnloadImage(im_cells[i]); /* Unload image since not needed */
    }
}

static void load_counter(void)
{
    Image im_counter[11];
    im_counter[0]  = LoadImage(RESOURCES_DIR "/counter/counter0.png");
    im_counter[1]  = LoadImage(RESOURCES_DIR "/counter/counter1.png");
    im_counter[2]  = LoadImage(RESOURCES_DIR "/counter/counter2.png");
    im_counter[3]  = LoadImage(RESOURCES_DIR "/counter/counter3.png");
    im_counter[4]  = LoadImage(RESOURCES_DIR "/counter/counter4.png");
    im_counter[5]  = LoadImage(RESOURCES_DIR "/counter/counter5.png");
    im_counter[6]  = LoadImage(RESOURCES_DIR "/counter/counter6.png");
    im_counter[7]  = LoadImage(RESOURCES_DIR "/counter/counter7.png");
    im_counter[8]  = LoadImage(RESOURCES_DIR "/counter/counter8.png");
    im_counter[9]  = LoadImage(RESOURCES_DIR "/counter/counter9.png");
    im_counter[10] = LoadImage(RESOURCES_DIR "/counter/counter-.png");

    for (int i = 0; i < 11; i++) {
        ImageResize(&im_counter[i], COUNTER_WIDTH, COUNTER_HEIGHT);
        counter[i] = LoadTextureFromImage(im_counter[i]); /* Create texture */
        UnloadImage(im_counter[i]); /* Unload image since not needed */
    }
}

static void load_faces(void)
{
    Image im_faces[5];
    im_faces[0] = LoadImage(RESOURCES_DIR "/faces/clickface.png");
    im_faces[1] = LoadImage(RESOURCES_DIR "/faces/lostface.png");
    im_faces[2] = LoadImage(RESOURCES_DIR "/faces/smileface.png");
    im_faces[3] = LoadImage(RESOURCES_DIR "/faces/smilefacedown.png");
    im_faces[4] = LoadImage(RESOURCES_DIR "/faces/winface.png");

    for (int i = 0; i < 5; i++) {
        ImageResize(&im_faces[i], FACE_SIZE, FACE_SIZE);
        faces[i] = LoadTextureFromImage(im_faces[i]); /* Create texture */
        UnloadImage(im_faces[i]); /* Unload image since not needed */
    }
}

void unload_textures(void)
{
    for (int i = 0; i < 13; i++)
        UnloadTexture(cells[i]);
    for (int i = 0; i < 11; i++)
        UnloadTexture(counter[i]);
    for (int i = 0; i < 3; i++)
        UnloadTexture(faces[i]);
}
