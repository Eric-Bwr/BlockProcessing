#pragma once

//MOVE TO NEW WORLD GENERATION THING
const int TERRAIN_AMPLIFIER = 200;

const float TERRAIN_SIZE = 20.0f;
const int CHUNK_SIZE = 20;
const int CHUNKING_RADIUS = 8;
const int CHUNKING_THREADS = 12;
const int CHUNKING_DELETION_OFFSET = 2;

const int OCTREE_MAX_LEVEL = 4;

const int TEXTURE_ATLAS_TILE_SIZE = 32;
const int TEXTURE_ATLAS_ROW_COUNT = 16;
const int TEXTURE_ATLAS_LOD_BIAS = -2;
//----------------------------------------[DEBUG]----------------------------------------
const float CHUNK_BORDER_WIDTH = 1.4;

const float OCTREE_LINE_WIDTH = 2;
const float OCTREE_LINE_WIDTH_AMPLIFIER = 2;
const float OCTREE_VISUALIZING_COLORS[(OCTREE_MAX_LEVEL + 1) * 3]{
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "cmath"
const double WORLD_SIZE = (CHUNK_SIZE * TERRAIN_SIZE);
const int CHUNKING_DELETION_RADIUS = (CHUNKING_RADIUS + CHUNKING_DELETION_OFFSET);
const int CHUNK_CUBIC_SIZE = (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
const int OCTREE_LENGTH = 1 << OCTREE_MAX_LEVEL;
