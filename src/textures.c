#include <stdbool.h>

#include "config.h"
#include "grid.h"
#include "raylib.h"
#include "screens.h"
#include "textures.h"

/* Shared variables */
Texture cells[14];
Texture counter[11];
Texture faces[5];
Texture cup;

/* Local (to module) variables */
static bool textures_loaded_;
static void load_cells_(void);
static void load_counter_(void);
static void load_faces_(void);
static void load_cup_(void);

void load_textures(void)
{
    if (textures_loaded_)
        return;

    load_cells_();
    load_counter_();
    load_faces_();
    load_cup_();
    textures_loaded_ = true;
}

static void load_cells_(void)
{
    Image im_cells[14];

    im_cells[C_EMPTY]  = LoadImage("./resources/cells/celldown.png");
    im_cells[C_ONE]    = LoadImage("./resources/cells/cell1.png");
    im_cells[C_TWO]    = LoadImage("./resources/cells/cell2.png");
    im_cells[C_THREE]  = LoadImage("./resources/cells/cell3.png");
    im_cells[C_FOUR]   = LoadImage("./resources/cells/cell4.png");
    im_cells[C_FIVE]   = LoadImage("./resources/cells/cell5.png");
    im_cells[C_SIX]    = LoadImage("./resources/cells/cell6.png");
    im_cells[C_SEVEN]  = LoadImage("./resources/cells/cell7.png");
    im_cells[C_EIGHT]  = LoadImage("./resources/cells/cell8.png");
    im_cells[C_BLAST]  = LoadImage("./resources/cells/blast.png");
    im_cells[C_CLOSED] = LoadImage("./resources/cells/cellup.png");
    im_cells[C_FLAG]   = LoadImage("./resources/cells/cellflag.png");
    im_cells[C_MINE]   = LoadImage("./resources/cells/cellmine.png");
    im_cells[C_DOWN]   = LoadImage("./resources/cells/celldown.png");

    for (int i = 0; i < sizeof(im_cells) / sizeof(im_cells[0]); i++) {
        ImageResize(&im_cells[i], CELL_SIZE, CELL_SIZE);
        cells[i] = LoadTextureFromImage(im_cells[i]); /* Create texture */
        UnloadImage(im_cells[i]); /* Unload image since not needed */
    }
}

static void load_counter_(void)
{
    Image im_counter[11];

    im_counter[0]  = LoadImage("./resources/counter/counter0.png");
    im_counter[1]  = LoadImage("./resources/counter/counter1.png");
    im_counter[2]  = LoadImage("./resources/counter/counter2.png");
    im_counter[3]  = LoadImage("./resources/counter/counter3.png");
    im_counter[4]  = LoadImage("./resources/counter/counter4.png");
    im_counter[5]  = LoadImage("./resources/counter/counter5.png");
    im_counter[6]  = LoadImage("./resources/counter/counter6.png");
    im_counter[7]  = LoadImage("./resources/counter/counter7.png");
    im_counter[8]  = LoadImage("./resources/counter/counter8.png");
    im_counter[9]  = LoadImage("./resources/counter/counter9.png");
    im_counter[10] = LoadImage("./resources/counter/counter-.png");

    for (int i = 0; i < sizeof(im_counter) / sizeof(im_counter[0]); i++) {
        ImageResize(&im_counter[i], COUNTER_WIDTH, COUNTER_HEIGHT);
        counter[i] = LoadTextureFromImage(im_counter[i]); /* Create texture */
        UnloadImage(im_counter[i]); /* Unload image since not needed */
    }
}

static void load_faces_(void)
{
    Image im_faces[5];

    im_faces[F_CLICK]      = LoadImage("./resources/faces/clickface.png");
    im_faces[F_LOST]       = LoadImage("./resources/faces/lostface.png");
    im_faces[F_SMILE]      = LoadImage("./resources/faces/smileface.png");
    im_faces[F_SMILE_DOWN] = LoadImage("./resources/faces/smilefacedown.png");
    im_faces[F_WIN]        = LoadImage("./resources/faces/winface.png");

    for (int i = 0; i < sizeof(im_faces) / sizeof(im_faces[0]); i++) {
        ImageResize(&im_faces[i], FACE_SIZE, FACE_SIZE);
        faces[i] = LoadTextureFromImage(im_faces[i]); /* Create texture */
        UnloadImage(im_faces[i]); /* Unload image since not needed */
    }
}

static void load_cup_(void)
{
    Image im_cup = LoadImage("./resources/cup.png");
    ImageResize(&im_cup, CUP_ICON_SIZE, CUP_ICON_SIZE);
    cup = LoadTextureFromImage(im_cup);
    UnloadImage(im_cup);
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
