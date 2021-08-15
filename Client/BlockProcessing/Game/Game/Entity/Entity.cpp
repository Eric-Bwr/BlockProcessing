#include "Entity.h"

void Entity::initEntity(double x, double y, double z, double yaw, double pitch) {
    position.x = x;
    position.y = y;
    position.z = z;
    this->yaw = yaw;
    this->pitch = pitch;
}

Vec3 Entity::convertMovement(double strafe, double forward, float slipperiness) {
    double speed = std::sqrt(strafe * strafe + forward * forward);
    if (speed < 0.01)
        return Vec3(0);
    speed = slipperiness / std::max(speed, 1.0);

    strafe = strafe * speed;
    forward = forward * speed;

    double yawYComponent = std::sin((yaw - 90) * radians);
    double yawXComponent = std::cos((yaw - 90) * radians);

    double xComponent = strafe * yawXComponent - forward * yawYComponent;
    double zComponent = forward * yawXComponent + strafe * yawYComponent;

    return Vec3(xComponent, 0, zComponent);
}