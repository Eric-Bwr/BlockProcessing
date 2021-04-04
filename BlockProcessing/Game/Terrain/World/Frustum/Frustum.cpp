#include "Frustum.h"

enum Planes {
    Near, Far, Left, Right, Top, Bottom
};

float Plane::distanceToPoint(Coord point) const {
    return (point.tileX * normal.x + point.tileY * normal.y + point.tileZ * normal.z) + distanceToOrigin;
}

void Frustum::update(Mat4f& projectionView) {
    planes[Planes::Left].normal.x = projectionView.m03 + projectionView.m00;
    planes[Planes::Left].normal.y = projectionView.m13 + projectionView.m10;
    planes[Planes::Left].normal.z = projectionView.m23 + projectionView.m20;
    planes[Planes::Left].distanceToOrigin = projectionView.m33 + projectionView.m30;

    planes[Planes::Right].normal.x = projectionView.m03 - projectionView.m00;
    planes[Planes::Right].normal.y = projectionView.m13 - projectionView.m10;
    planes[Planes::Right].normal.z = projectionView.m23 - projectionView.m20;
    planes[Planes::Right].distanceToOrigin = projectionView.m33 - projectionView.m30;

    planes[Planes::Bottom].normal.x = projectionView.m03 + projectionView.m01;
    planes[Planes::Bottom].normal.y = projectionView.m13 + projectionView.m11;
    planes[Planes::Bottom].normal.z = projectionView.m23 + projectionView.m21;
    planes[Planes::Bottom].distanceToOrigin = projectionView.m33 + projectionView.m31;

    planes[Planes::Top].normal.x = projectionView.m03 - projectionView.m01;
    planes[Planes::Top].normal.y = projectionView.m13 - projectionView.m11;
    planes[Planes::Top].normal.z = projectionView.m23 - projectionView.m21;
    planes[Planes::Top].distanceToOrigin = projectionView.m33 - projectionView.m31;

    planes[Planes::Near].normal.x = projectionView.m03 + projectionView.m02;
    planes[Planes::Near].normal.y = projectionView.m13 + projectionView.m12;
    planes[Planes::Near].normal.z = projectionView.m23 + projectionView.m22;
    planes[Planes::Near].distanceToOrigin = projectionView.m33 + projectionView.m32;

    planes[Planes::Far].normal.x = projectionView.m03 - projectionView.m02;
    planes[Planes::Far].normal.y = projectionView.m13 - projectionView.m12;
    planes[Planes::Far].normal.z = projectionView.m23 - projectionView.m22;
    planes[Planes::Far].distanceToOrigin = projectionView.m33 - projectionView.m32;

    for (auto &plane : planes) {
        float length = plane.normal.len();
        plane.normal.x /= length;
        plane.normal.y /= length;
        plane.normal.z /= length;
        plane.distanceToOrigin /= length;
    }
}

bool Frustum::isInside(const Coord& coord) {
    bool result = true;
    for (auto &plane : planes) {
        if (plane.distanceToPoint(getVP(plane.normal, coord)) < 0) {
            return false;
        } else if (plane.distanceToPoint(getVN(plane.normal, coord)) < 0) {
            result = true;
        }
    }
    return result;
}

Coord Frustum::getVN(Vec3f &normal, const Coord& coord) {
    Coord copyCoord = coord;
    copyCoord.tileX *= WORLD_SIZE;
    copyCoord.tileY *= WORLD_SIZE;
    copyCoord.tileZ *= WORLD_SIZE;
    if (normal.x < 0) {
        copyCoord.tileX += WORLD_SIZE;
    }
    if (normal.y < 0) {
        copyCoord.tileY += WORLD_SIZE;
    }
    if (normal.z < 0) {
        copyCoord.tileZ += WORLD_SIZE;
    }
    return coord;
}

Coord Frustum::getVP(Vec3f &normal, const Coord& coord) {
    Coord copyCoord = coord;
    copyCoord.tileX *= WORLD_SIZE;
    copyCoord.tileY *= WORLD_SIZE;
    copyCoord.tileZ *= WORLD_SIZE;
    if (normal.x > 0) {
        copyCoord.tileX += WORLD_SIZE;
    }
    if (normal.y > 0) {
        copyCoord.tileY += WORLD_SIZE;
    }
    if (normal.z > 0) {
        copyCoord.tileZ += WORLD_SIZE;
    }
    return copyCoord;
}