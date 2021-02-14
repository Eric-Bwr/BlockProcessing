#include "Frustum.h"

enum Planes {
    Near, Far, Left, Right, Top, Bottom
};

float Plane::distanceToPoint(Coord point) const {
    return (point.tileX * normal.tileX + point.tileY * normal.tileY + point.tileZ * normal.tileZ) + distanceToOrigin;
}

void Frustum::update(Matrix4f *projectionView) {
    planes[Planes::Left].normal.tileX = projectionView->m03 + projectionView->m00;
    planes[Planes::Left].normal.tileY = projectionView->m13 + projectionView->m10;
    planes[Planes::Left].normal.tileZ = projectionView->m23 + projectionView->m20;
    planes[Planes::Left].distanceToOrigin = projectionView->m33 + projectionView->m30;

    planes[Planes::Right].normal.tileX = projectionView->m03 - projectionView->m00;
    planes[Planes::Right].normal.tileY = projectionView->m13 - projectionView->m10;
    planes[Planes::Right].normal.tileZ = projectionView->m23 - projectionView->m20;
    planes[Planes::Right].distanceToOrigin = projectionView->m33 - projectionView->m30;

    planes[Planes::Bottom].normal.tileX = projectionView->m03 + projectionView->m01;
    planes[Planes::Bottom].normal.tileY = projectionView->m13 + projectionView->m11;
    planes[Planes::Bottom].normal.tileZ = projectionView->m23 + projectionView->m21;
    planes[Planes::Bottom].distanceToOrigin = projectionView->m33 + projectionView->m31;

    planes[Planes::Top].normal.tileX = projectionView->m03 - projectionView->m01;
    planes[Planes::Top].normal.tileY = projectionView->m13 - projectionView->m11;
    planes[Planes::Top].normal.tileZ = projectionView->m23 - projectionView->m21;
    planes[Planes::Top].distanceToOrigin = projectionView->m33 - projectionView->m31;

    planes[Planes::Near].normal.tileX = projectionView->m03 + projectionView->m02;
    planes[Planes::Near].normal.tileY = projectionView->m13 + projectionView->m12;
    planes[Planes::Near].normal.tileZ = projectionView->m23 + projectionView->m22;
    planes[Planes::Near].distanceToOrigin = projectionView->m33 + projectionView->m32;

    planes[Planes::Far].normal.tileX = projectionView->m03 - projectionView->m02;
    planes[Planes::Far].normal.tileY = projectionView->m13 - projectionView->m12;
    planes[Planes::Far].normal.tileZ = projectionView->m23 - projectionView->m22;
    planes[Planes::Far].distanceToOrigin = projectionView->m33 - projectionView->m32;

    for (auto &plane : planes) {
        float length = std::sqrt(plane.normal.tileX * plane.normal.tileX + plane.normal.tileY * plane.normal.tileY + plane.normal.tileZ * plane.normal.tileZ);
        plane.normal.tileX /= length;
        plane.normal.tileY /= length;
        plane.normal.tileZ /= length;
        plane.distanceToOrigin /= length;
    }
}

bool Frustum::isInside(const AABB &box) {
    bool result = true;
    for (auto &plane : planes) {
        if (plane.distanceToPoint(box.getVP(plane.normal)) < 0) {
            return false;
        } else if (plane.distanceToPoint(box.getVN(plane.normal)) < 0) {
            result = true;
        }
    }
    return result;
}