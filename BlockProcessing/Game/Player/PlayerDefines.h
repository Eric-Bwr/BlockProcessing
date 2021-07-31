#pragma once

const double PLAYER_BLOCK_DISTANCE = 15.0;
const double PLAYER_STEP_SIZE = 0.03;
const double PLAYER_GRAVITY = 1.5;
const double PLAYER_MOVE_SPEED = 18;
const int PLAYER_DOUBLE_PRESS_IGNORE_SPAN = 5;
const int PLAYER_DOUBLE_PRESS_SPAN = 30;
const float PLAYER_BLOCK_OUTLINE_WIDTH = 10;
const float PLAYER_BLOCK_OUTLINE_COLOR[3]{
        1.0, 0.0, 0.0
};
//----------------------------------------[CAMERA]----------------------------------------
const double PLAYER_CAMERA_PITCH_MAX = 89.0, PLAYER_CAMERA_PITCH_MIN = -89.0;
const double PLAYER_CAMERA_MOUSE_SENSITIVITY = 0.025;
//----------------------------------------[UNNECESSARY]----------------------------------------
const int GAMEMODE_CREATIVE = 1;
const int GAMEMODE_SURVIVAL = 0;