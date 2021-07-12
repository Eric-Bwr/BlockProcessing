#pragma once

//MOVE TO NEW WORLD GENERATION THING
#define TERRAIN_AMPLIFIER 200

#define TERRAIN_SIZE 20
#define CHUNK_SIZE 20
#define CHUNKING_RADIUS 8
#define CHUNKING_THREADS 4
#define CHUNKING_DELETION_OFFSET 2

const int OCTREE_MAX_LEVEL = 2;

#define TEXTURE_ATLAS_TILE_SIZE 32
#define TEXTURE_ATLAS_ROW_COUNT 16
#define TEXTURE_ATLAS_LOD_BIAS -2
//----------------------------------------[DEBUG]----------------------------------------
const float CHUNK_BORDER_WIDTH = 1.4;

const float OCTREE_LINE_WIDTH = 2;
const float OCTREE_LINE_WIDTH_AMPLIFIER = 2;
const float OCTREE_VISUALIZING_COLORS[(OCTREE_MAX_LEVEL + 1) * 3]{
        1.0, 1.0, 1.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "cmath"
const int WORLD_SIZE = (CHUNK_SIZE * TERRAIN_SIZE);
const int CHUNKING_DELETION_RADIUS = (CHUNKING_RADIUS + CHUNKING_DELETION_OFFSET);
const int CHUNK_CUBIC_SIZE = (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
const int OCTREE_LENGTH = pow(2, OCTREE_MAX_LEVEL);
