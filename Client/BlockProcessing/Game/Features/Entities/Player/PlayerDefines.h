#pragma once

const float PLAYER_WIDTH = 0.7f;
const float PLAYER_HEIGHT = 1.9f;
const float PLAYER_Y_OFFSET = 1.65f;

const float PLAYER_BLOCK_DISTANCE = 5.0;
const float PLAYER_STEP_SIZE = 0.03;
const float PLAYER_GRAVITY = 1.5;
const float PLAYER_MOVE_SPEED = 18;
const int PLAYER_DOUBLE_PRESS_IGNORE_SPAN = 5;
const int PLAYER_DOUBLE_PRESS_SPAN = 30;
const float PLAYER_BLOCK_OUTLINE_WIDTH = 2;
//----------------------------------------[CAMERA]----------------------------------------
const double PLAYER_CAMERA_PITCH_MAX = 89.0, PLAYER_CAMERA_PITCH_MIN = -89.0;
const double PLAYER_CAMERA_MOUSE_SENSITIVITY = 0.025;
//----------------------------------------[UNNECESSARY]----------------------------------------
const int GAMEMODE_CREATIVE = 1;
const int GAMEMODE_SURVIVAL = 0;

const float PLAYER_WIDTH_HALF = PLAYER_WIDTH / 2;
const float PLAYER_HEIGHT_MINUS_Y_OFFSET = PLAYER_HEIGHT - PLAYER_Y_OFFSET;
const float PLAYER_BLOCK_DISTANCE_SQUARED = PLAYER_BLOCK_DISTANCE * PLAYER_BLOCK_DISTANCE;