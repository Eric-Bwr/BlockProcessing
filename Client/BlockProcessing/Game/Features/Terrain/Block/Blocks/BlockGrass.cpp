#include "BlockGrass.h"

BlockGrass::BlockGrass() {
    name = "Grass";
    id = BLOCK_GRASS;
    paths[0] = "GrassSide.png";
    paths[1] = "Dirt.png";
    paths[2] = "Grass.png";
    textureBottom = 1;
    textureTop = 2;
}
