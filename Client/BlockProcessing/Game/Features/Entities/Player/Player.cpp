#include "Player.h"

void Player::init(WorldManager *worldManager, double x, double y, double z, float yaw, float pitch) {
    initEntity(x, y, z, yaw, pitch);
    this->worldManager = worldManager;
    worldUp.y = 1;
    front.z = -1;
    view.identity();
    calculateCamera();
    playerBlockOutline.init();
}

void Player::render(Mat4d &view) {
    playerBlockOutline.render(view);
}

void Player::setProjection(Mat4f &projection) {
    playerBlockOutline.setProjection(projection);
}

void Player::update(double deltaTime) {
    calculateMove(deltaTime);
    block.x = blockX = getBlockFromCamera(position.x);
    block.y = blockY = getBlockFromCamera(position.y);
    block.z = blockZ = getBlockFromCamera(position.z);
    chunk.x = chunkX = getChunkFromBlock(blockX);
    chunk.y = chunkY = getChunkFromBlock(blockY);
    chunk.z = chunkZ = getChunkFromBlock(blockZ);
    octree.x = octreeX = getOctreeFromChunk(chunkX);
    octree.y = octreeY = getOctreeFromChunk(chunkY);
    octree.z = octreeZ = getOctreeFromChunk(chunkZ);
    castRay();
}

void Player::dig() {
    if (lookedBlockID != BLOCK_AIR)
        worldManager->setBlock(BLOCK_AIR, lookedBlockX, lookedBlockY, lookedBlockZ);
}

void Player::place() {
    if (lookedBlockID != BLOCK_AIR)
        worldManager->setBlock(BLOCK_PLANKS, prevLookedBlockX, prevLookedBlockY, prevLookedBlockZ);
}

bool Player::colliding() {
    int minX = position.x - PLAYER_WIDTH_HALF;
    int minY = position.y - PLAYER_HEIGHT;
    int minZ = position.z - PLAYER_WIDTH_HALF;

    int maxX = position.x + PLAYER_WIDTH_HALF;
    int maxY = position.y + PLAYER_HEIGHT + 1;
    int maxZ = position.z + PLAYER_WIDTH_HALF;

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            for (int z = minZ; z <= maxZ; z++) {
                collisionBlockID = worldManager->getBlock(x, y, z);
                if (collisionBlockID != 0)
                    return true;
            }
        }
    }
    return false;
}

bool Player::isOnGround() {
    return true;
}
//https://github.com/ddevault/TrueCraft/wiki/Entity-Movement-And-Physics

//https://github.com/kpreid/cubes/blob/c5e61fa22cb7f9ba03cd9f22e5327d738ec93969/world.js#L307
//https://gist.github.com/aadnk/7123926
//https://gamedev.stackexchange.com/questions/88298/aabb-swept-collision-response-with-voxel-world#88315
//https://stackoverflow.com/questions/8978491/player-to-voxel-collision-detection-response
void Player::calculateMove(double deltaTime) {
    if (!(shouldMoveForward && shouldMoveBackward)) {
        if (!shouldMoveForward && !shouldMoveBackward)
            moveForward = 0.0f;
        if (shouldMoveForward)
            moveForward = 1.0f;
        if (shouldMoveBackward)
            moveForward = -1.0f;
    } else
        moveForward = 0.0f;
    if (!(shouldMoveLeft && shouldMoveRight)) {
        if (!shouldMoveLeft && !shouldMoveRight)
            moveStrafe = 0.0f;
        if (shouldMoveLeft)
            moveStrafe = 1.0f;
        if (shouldMoveRight)
            moveStrafe = -1.0f;
    } else
        moveStrafe = 0.0f;

    calculateMovement(moveStrafe, moveForward, 0.06);

    //velocityY -= 0.08;
    //velocityY *= 0.98;
    // if(onGround){
    velocityX *= 0.7;
    velocityZ *= 0.7;
    // }else{
    //     velocityX *= 0.91;
    //     velocityZ *= 0.91;
    // }


    velocityY *= 0.7;
    if (shouldMoveDown)
        velocityY -= 0.06;
    if (shouldMoveUp)
        velocityY += 0.06;
    position.x += velocityX * speed * deltaTime;
    position.y += velocityY * speed * deltaTime;
    position.z += velocityZ * speed * deltaTime;
}

void Player::castRay() {
    auto direction = Vec3d(front);
    direction.norm();
	
	auto currentPos = Vec3d(position);
    while ((currentPos - position).dot(currentPos - position) <= PLAYER_BLOCK_DISTANCE * PLAYER_BLOCK_DISTANCE) {

    	prevLookedBlockX = lookedBlockX;
        prevLookedBlockY = lookedBlockY;
        prevLookedBlockZ = lookedBlockZ;
		
		const auto& nextWholeCoord = [](int64_t lookedBlock, double currentPos, double direction){
			double t = INFINITY;
		
			//lookedBlock <= currentPos < lookedBlock + 1.0 //by definition of floor

			lookedBlock = floor(currentPos);
			if(direction > 0.0){
				//currentPos + t * direction == lookedBlockX+1;
				t = (lookedBlock + 1.0 - currentPos) / direction;
			}else if(direction < 0.0){
				if(currentPos > lookedBlock){
					//currentPos + t * direction == lookedBlockX;
					t = (lookedBlock - currentPos) / direction;
				}else{
					//currentPos + t * direction == lookedBlockX-1;
					t = (lookedBlock - 1.0 - currentPos) / direction;
				}
			}


			return t;
		};
		
		double tx = nextWholeCoord(lookedBlockX, currentPos.x, direction.x);
		double ty = nextWholeCoord(lookedBlockY, currentPos.y, direction.y);
		double tz = nextWholeCoord(lookedBlockZ, currentPos.z, direction.z);
		
		double t = std::min(tx, std::min(ty, tz));

        currentPos += direction * t;

        if(t == tx){
        	currentPos.x = round(currentPos.x);
        }else if(t == ty){
        	currentPos.y = round(currentPos.y);
        }else if(t == tz){
        	currentPos.z = round(currentPos.z);
        }

		lookedBlockX = floor(currentPos.x);
		lookedBlockY = floor(currentPos.y);
		lookedBlockZ = floor(currentPos.z);

		if(t == tx && direction.x < 0){
			lookedBlockX--;
		}else if(t == ty && direction.y < 0){
			lookedBlockY--;
		}else if(t == tz && direction.z < 0){
			lookedBlockZ--;
		}

		//     \/
		// X ____ y=1   <--- good
		// X   /\
		//

        lookedBlockID = worldManager->getBlock(lookedBlockX, lookedBlockY, lookedBlockZ);
        if (lookedBlockID != BLOCK_AIR) {
            lookedBlock.x = lookedBlockX;
            lookedBlock.y = lookedBlockY;
            lookedBlock.z = lookedBlockZ;
            playerBlockOutline.update(lookedBlockX, lookedBlockY, lookedBlockZ);
            return;
        }
    }
    playerBlockOutline.update(0, -99999999, 0);
}

void Player::calculateCamera() {
    front.x = std::cos(yaw * RADIANS) * std::cos(pitch * RADIANS);
    front.y = std::sin(pitch * RADIANS);
    front.z = std::sin(yaw * RADIANS) * std::cos(pitch * RADIANS);
    front.norm();
    right = front.cross(worldUp).norm();
    up = (right.cross(front)).norm();
}

void Player::moveMouse(double xPos, double yPos) {
    if (!hasLastPos) {
        lastPosX = xPos;
        lastPosY = (yPos * -1);
        hasLastPos = true;
    }
    xOffset = (lastPosX - xPos) * -1;
    yOffset = (yPos * -1) - lastPosY;
    lastPosX = xPos;
    lastPosY = (yPos * -1);

    xOffset *= PLAYER_CAMERA_MOUSE_SENSITIVITY;
    yOffset *= PLAYER_CAMERA_MOUSE_SENSITIVITY;

    yaw += xOffset;
    pitch += yOffset;
    if (pitch >= PLAYER_CAMERA_PITCH_MAX)
        pitch = PLAYER_CAMERA_PITCH_MAX;
    if (pitch <= PLAYER_CAMERA_PITCH_MIN)
        pitch = PLAYER_CAMERA_PITCH_MIN;
    calculateCamera();
}

Mat4d Player::getViewMatrix() {
    return view.lookAtModified(position, front, up);
}

Player::~Player() = default;
