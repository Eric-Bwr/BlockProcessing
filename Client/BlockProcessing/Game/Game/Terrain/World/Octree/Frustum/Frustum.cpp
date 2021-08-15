#include "Frustum.h"
#include "BlockProcessing/Game/Game/Terrain/Util/Coordinate.h"

enum Planes {
    Near, Far, Left, Right, Top, Bottom
};

void Frustum::update(Mat4& projectionView) {
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
        double length = plane.normal.len();
        plane.normal.x /= length;
        plane.normal.y /= length;
        plane.normal.z /= length;
        plane.distanceToOrigin /= length;
    }
}

float Frustum::distanceToPlane(const Plane& plane, const Coord& point) {
    return (point.x * CHUNK_SIZE) * plane.normal.x + (point.y * CHUNK_SIZE) * plane.normal.y + (point.z * CHUNK_SIZE) * plane.normal.z + plane.distanceToOrigin;
}

bool Frustum::isInside(const Coord& center, float radius){
    float dist01 = std::min(distanceToPlane(planes[0], center), distanceToPlane(planes[1], center));
    float dist23 = std::min(distanceToPlane(planes[2], center), distanceToPlane(planes[3], center));
    float dist45 = std::min(distanceToPlane(planes[4], center), distanceToPlane(planes[5], center));

    return std::min(std::min(dist01, dist23), dist45) + radius > 0;
}