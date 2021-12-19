#pragma once

const int CHUNK_SIZE = 16;

const int OCTREE_MAX_LEVEL = 4;

//----------------------------------------[DEBUG]----------------------------------------
const float CHUNK_BORDER_WIDTH = 1.4;

const float OCTREE_LINE_WIDTH = 1;
const float OCTREE_LINE_WIDTH_AMPLIFIER = 1.5;
const float OCTREE_VISUALIZING_COLORS[(OCTREE_MAX_LEVEL + 1) * 3]{
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
const int CHUNK_SIZE_MINUS_ONE = (CHUNK_SIZE - 1);
const int CHUNK_CUBIC_SIZE = (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
const int OCTREE_LENGTH = OCTREE_MAX_LEVEL * OCTREE_MAX_LEVEL;
const int OCTREE_FRUSTUM_CULLING = CHUNK_SIZE * 2;