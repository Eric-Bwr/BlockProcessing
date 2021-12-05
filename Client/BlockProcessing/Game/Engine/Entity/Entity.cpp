#include "Entity.h"

void Entity::initEntity(double x, double y, double z, double yaw, double pitch) {
    position.x = x;
    position.y = y;
    position.z = z;
    this->yaw = yaw;
    this->pitch = pitch;
}

void Entity::calculateMovement(double strafe, double forward, float slipperiness) {
    double speed = std::sqrt(strafe * strafe + forward * forward);
    if (speed < 0.01)
        return;
    speed = slipperiness / std::max(speed, 1.0);

    strafe = strafe * speed;
    forward = forward * speed;

    double yawYComponent = std::sin((yaw - 90) * RADIANS);
    double yawXComponent = std::cos((yaw - 90) * RADIANS);

    velocityX += strafe * yawXComponent - forward * yawYComponent;
    velocityZ += forward * yawXComponent + strafe * yawYComponent;
}