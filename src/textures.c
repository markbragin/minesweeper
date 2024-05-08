#include "textures.h"
#include "grid.h"
#include "raylib.h"

Texture cells[13];

bool textures_loaded = false;

void load_textures(void)
{
    if (textures_loaded)
        return;

    /* Images */
    Image images[13];
    images[C_EMPTY]
        = LoadImageSvg("resources/svg/celldown.svg", CELL_SIZE, CELL_SIZE);
    images[C_ONE]
        = LoadImageSvg("resources/svg/cell1.svg", CELL_SIZE, CELL_SIZE);
    images[C_TWO]
        = LoadImageSvg("resources/svg/cell2.svg", CELL_SIZE, CELL_SIZE);
    images[C_THREE]
        = LoadImageSvg("resources/svg/cell3.svg", CELL_SIZE, CELL_SIZE);
    images[C_FOUR]
        = LoadImageSvg("resources/svg/cell4.svg", CELL_SIZE, CELL_SIZE);
    images[C_FIVE]
        = LoadImageSvg("resources/svg/cell5.svg", CELL_SIZE, CELL_SIZE);
    images[C_SIX]
        = LoadImageSvg("resources/svg/cell6.svg", CELL_SIZE, CELL_SIZE);
    images[C_SEVEN]
        = LoadImageSvg("resources/svg/cell7.svg", CELL_SIZE, CELL_SIZE);
    images[C_EIGHT]
        = LoadImageSvg("resources/svg/cell8.svg", CELL_SIZE, CELL_SIZE);
    images[C_CLOSED]
        = LoadImageSvg("resources/svg/cellup.svg", CELL_SIZE, CELL_SIZE);
    images[C_MINE]
        = LoadImageSvg("resources/svg/cellmine.svg", CELL_SIZE, CELL_SIZE);
    images[C_FLAG]
        = LoadImageSvg("resources/svg/cellflag.svg", CELL_SIZE, CELL_SIZE);
    images[C_BLAST]
        = LoadImageSvg("resources/svg/blast.svg", CELL_SIZE, CELL_SIZE);

    /* Textures */
    for (int i = 0; i < 13; i++) {
        cells[i] = LoadTextureFromImage(images[i]); /* Create texture */
        UnloadImage(images[i]); /* Unload image since not needed */
    }

    textures_loaded = true;
}

void unload_textures(void)
{
    for (int i = 0; i < sizeof(cells); i++)
        UnloadTexture(cells[i]);
}
