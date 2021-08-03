#pragma once

//MOVE TO NEW WORLD GENERATION THING
const int TERRAIN_AMPLIFIER = 200;

const int CHUNK_SIZE = 16;
const int CHUNKING_THREADS = 6;
const int CHUNKING_RADIUS = 40;
const int CHUNKING_DELETION_OFFSET = 1;

const int OCTREE_MAX_LEVEL = 4;

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
const int MAX_CHUNKS_TO_GENERATE = CHUNKING_THREADS * 10;
const int CHUNK_CUBIC_SIZE = (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
const int OCTREE_LENGTH = 1 << OCTREE_MAX_LEVEL;
const int CHUNKING_DELETION_RADIUS_SQUARED = (CHUNKING_RADIUS + CHUNKING_DELETION_OFFSET) * (CHUNKING_RADIUS + CHUNKING_DELETION_OFFSET);
const int OCTREE_DELETION_RADIUS_SQUARED = (3 * OCTREE_LENGTH + OCTREE_LENGTH / 2) * (3 * OCTREE_LENGTH + OCTREE_LENGTH / 2);
const int CHUNKING_RADIUS_SQUARED = (CHUNKING_RADIUS * CHUNKING_RADIUS);
const int OCTREE_FRUSTUM_CULLING = CHUNK_SIZE * 2;