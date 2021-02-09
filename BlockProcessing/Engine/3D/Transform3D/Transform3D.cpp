#include "Transform3D.h"

Transform3D::Transform3D()
: x(0.0f), y(0.0f), z(0.0f), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(0.0f), sizeY(0.0f), sizeZ(0.0f) {
    updateMatrix();
}

Transform3D::Transform3D(float x, float y, float z, float angle, float axisX, float axisY, float axisZ, float size)
: x(x), y(y), z(z), angle(angle), axisX(axisX), axisY(axisY), axisZ(axisZ), sizeX(size), sizeY(size), sizeZ(size) {
    updateMatrix();
}

Transform3D::Transform3D(float x, float y, float z, float angle, const Vector3f &axis, float size)
        : x(x), y(y), z(z), angle(angle), axisX(axis.x), axisY(axis.y), axisZ(axis.z), sizeX(size), sizeY(size), sizeZ(size) {
    updateMatrix();
}

Transform3D::Transform3D(float x, float y, float z, float sizeX, float sizeY, float sizeZ)
        : x(x), y(y), z(z), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ){
    updateMatrix();
}

Transform3D::Transform3D(float x, float y, float z, float size)
: x(x), y(y), z(z), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(size), sizeY(size), sizeZ(size) {
    updateMatrix();
}

Transform3D::Transform3D(const Vector3f &position, float angle, float axisX, float axisY, float axisZ, float size)
: x(position.x), y(position.y), z(position.z), angle(angle), axisX(axisX), axisY(axisY), axisZ(axisZ), sizeX(size), sizeY(size), sizeZ(size) {
    updateMatrix();
}

Transform3D::Transform3D(const Vector3f &position, float angle, const Vector3f &axis, float size)
: x(position.x), y(position.y), z(position.z), angle(angle), axisX(axis.x), axisY(axis.y), axisZ(axis.z), sizeX(size), sizeY(size), sizeZ(size){
    updateMatrix();
}

Transform3D::Transform3D(const Vector3f &position, float size)
: x(position.x), y(position.y), z(position.z), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(size), sizeY(size), sizeZ(size) {
    updateMatrix();
}

Transform3D::Transform3D(const Vector3f &position, float sizeX, float sizeY, float sizeZ)
: x(position.x), y(position.y), z(position.z), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {
    updateMatrix();
}

void Transform3D::setPosition(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    updateMatrix();
}

void Transform3D::setPosition(const Vector3f &position) {
    this->x = position.x;
    this->y = position.y;
    this->z = position.z;
    updateMatrix();
}

void Transform3D::setRotation(float angle, float axisX, float axisY, float axisZ) {
    this->angle = angle;
    this->axisX = axisX;
    this->axisY = axisY;
    this->axisZ = axisZ;
    updateMatrix();
}

void Transform3D::setRotation(float angle, const Vector3f &axis) {
    this->angle = angle;
    this->axisX = axis.x;
    this->axisY = axis.y;
    this->axisZ = axis.z;
    updateMatrix();
}

void Transform3D::setSizes(float sizeX, float sizeY, float sizeZ) {
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->sizeZ = sizeZ;
    updateMatrix();
}

void Transform3D::setSize(float size) {
    this->sizeX = size;
    this->sizeY = size;
    this->sizeZ = size;
    updateMatrix();
}

void Transform3D::updateMatrix() {
    matrix.identity();
    matrix.translate(x, y, z);
    matrix.rotate(angle, axisX, axisY, axisZ);
    matrix.scale(sizeX, sizeY, sizeZ);
}

Transform3D::~Transform3D() = default;