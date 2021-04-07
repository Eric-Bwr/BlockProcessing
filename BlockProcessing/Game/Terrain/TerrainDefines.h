#pragma once

#define TERRAIN_SIZE 20
#define TERRAIN_AMPLIFIER 200
#define CHUNK_SIZE 30
#define CHUNKING_RADIUS 8
#define CHUNKING_THREADS 4
#define CHUNKING_DELETION_OFFSET 2

const float CHUNK_BORDER_WIDTH = 1.4;

const float PLAYER_BLOCK_DISTANCE = 15.0;
const float PLAYER_STEP_SIZE = 0.03;

#define OCTREE_MAX_LEVEL 2
#define OCTREE_LINE_WIDTH 4
#define OCTREE_LINE_WIDTH_AMPLIFIER 3
const float OCTREE_VISUALIZING_COLORS[(OCTREE_MAX_LEVEL + 1) * 3]{
        1.0, 1.0, 1.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        //0.0, 0.0, 1.0
};

#define TEXTURE_ATLAS_TILE_SIZE 32
#define TEXTURE_ATLAS_ROW_COUNT 16
#define TEXTURE_ATLAS_LOD_BIAS -2

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define WORLD_SIZE (CHUNK_SIZE * TERRAIN_SIZE)
#define CHUNKING_DELETION_RADIUS (CHUNKING_RADIUS + CHUNKING_DELETION_OFFSET)
#define CHUNK_CUBIC_SIZE (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)
#define OCTREE_LENGTH pow(2, OCTREE_MAX_LEVEL)