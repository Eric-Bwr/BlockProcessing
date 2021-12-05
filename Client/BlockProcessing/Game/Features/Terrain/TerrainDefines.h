#pragma once

const int CHUNK_SIZE = 20;

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
const int CHUNK_SIZE_PLUS_ONE = (CHUNK_SIZE + 1);
const int CHUNK_CUBIC_SIZE = (CHUNK_SIZE_PLUS_ONE * CHUNK_SIZE_PLUS_ONE * CHUNK_SIZE_PLUS_ONE);
const int OCTREE_LENGTH = OCTREE_MAX_LEVEL * OCTREE_MAX_LEVEL;
const int OCTREE_DELETION_RADIUS_SQUARED = (3 * OCTREE_LENGTH + OCTREE_LENGTH / 2) * (3 * OCTREE_LENGTH + OCTREE_LENGTH / 2);
const int OCTREE_FRUSTUM_CULLING = CHUNK_SIZE * 2;