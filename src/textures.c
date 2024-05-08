#include "textures.h"
#include "grid.h"
#include "raylib.h"

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
    im_cells[C_EMPTY]
        = LoadImageSvg("resources/cells/celldown.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_ONE]
        = LoadImageSvg("resources/cells/cell1.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_TWO]
        = LoadImageSvg("resources/cells/cell2.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_THREE]
        = LoadImageSvg("resources/cells/cell3.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_FOUR]
        = LoadImageSvg("resources/cells/cell4.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_FIVE]
        = LoadImageSvg("resources/cells/cell5.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_SIX]
        = LoadImageSvg("resources/cells/cell6.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_SEVEN]
        = LoadImageSvg("resources/cells/cell7.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_EIGHT]
        = LoadImageSvg("resources/cells/cell8.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_CLOSED]
        = LoadImageSvg("resources/cells/cellup.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_MINE]
        = LoadImageSvg("resources/cells/cellmine.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_FLAG]
        = LoadImageSvg("resources/cells/cellflag.svg", CELL_SIZE, CELL_SIZE);
    im_cells[C_BLAST]
        = LoadImageSvg("resources/cells/blast.svg", CELL_SIZE, CELL_SIZE);

    for (int i = 0; i < 13; i++) {
        cells[i] = LoadTextureFromImage(im_cells[i]); /* Create texture */
        UnloadImage(im_cells[i]); /* Unload image since not needed */
    }
}

static void load_counter(void)
{
    Image im_counter[11];
    im_counter[0]  = LoadImageSvg("resources/counter/counter0.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[1]  = LoadImageSvg("resources/counter/counter1.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[2]  = LoadImageSvg("resources/counter/counter2.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[3]  = LoadImageSvg("resources/counter/counter3.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[4]  = LoadImageSvg("resources/counter/counter4.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[5]  = LoadImageSvg("resources/counter/counter5.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[6]  = LoadImageSvg("resources/counter/counter6.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[7]  = LoadImageSvg("resources/counter/counter7.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[8]  = LoadImageSvg("resources/counter/counter8.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[9]  = LoadImageSvg("resources/counter/counter9.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);
    im_counter[10] = LoadImageSvg("resources/counter/counter-.svg",
                                  COUNTER_WIDTH, COUNTER_HEIGHT);

    for (int i = 0; i < 11; i++) {
        counter[i] = LoadTextureFromImage(im_counter[i]); /* Create texture */
        UnloadImage(im_counter[i]); /* Unload image since not needed */
    }
}

static void load_faces(void)
{
    Image im_faces[5];
    im_faces[0]
        = LoadImageSvg("resources/faces/clickface.svg", FACE_SIZE, FACE_SIZE);
    im_faces[1]
        = LoadImageSvg("resources/faces/lostface.svg", FACE_SIZE, FACE_SIZE);
    im_faces[2]
        = LoadImageSvg("resources/faces/smileface.svg", FACE_SIZE, FACE_SIZE);
    im_faces[3] = LoadImageSvg("resources/faces/smilefacedown.svg", FACE_SIZE,
                               FACE_SIZE);
    im_faces[4]
        = LoadImageSvg("resources/faces/winface.svg", FACE_SIZE, FACE_SIZE);

    for (int i = 0; i < 5; i++) {
        faces[i] = LoadTextureFromImage(im_faces[i]); /* Create texture */
        UnloadImage(im_faces[i]); /* Unload image since not needed */
    }
}

void unload_textures(void)
{
    for (int i = 0; i < sizeof(cells); i++)
        UnloadTexture(cells[i]);
}
