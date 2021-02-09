#include "Transform2D.h"

Transform2D::Transform2D()
        : x(0.0f), y(0.0f), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(0.0f), sizeY(0.0f) {
    updateMatrix();
}

Transform2D::Transform2D(float x, float y, float angle, float axisX, float axisY, float axisZ, float size)
        : x(x), y(y), angle(angle), axisX(axisX), axisY(axisY), axisZ(axisZ), sizeX(size), sizeY(size) {
    updateMatrix();
}

Transform2D::Transform2D(float x, float y, float angle, const Vector3f &axis, float size)
        : x(x), y(y), angle(angle), axisX(axis.x), axisY(axis.y), axisZ(axis.z), sizeX(size), sizeY(size) {
    updateMatrix();
}

Transform2D::Transform2D(float x, float y, float sizeX, float sizeY)
        : x(x), y(y), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(sizeX), sizeY(sizeY) {
    updateMatrix();
}

Transform2D::Transform2D(float x, float y, float size)
        : x(x), y(y), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(size), sizeY(size) {
    updateMatrix();
}

Transform2D::Transform2D(const Vector2f &position, float angle, float axisX, float axisY, float axisZ, float size)
        : x(position.x), y(position.y), angle(angle), axisX(axisX), axisY(axisY), axisZ(axisZ), sizeX(size), sizeY(size) {
    updateMatrix();
}

Transform2D::Transform2D(const Vector2f &position, float angle, const Vector3f &axis, float size)
        : x(position.x), y(position.y), angle(angle), axisX(axis.x), axisY(axis.y), axisZ(axis.z), sizeX(size), sizeY(size) {
    updateMatrix();
}

Transform2D::Transform2D(const Vector2f &position, float size)
        : x(position.x), y(position.y), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(size), sizeY(size) {
    updateMatrix();
}

Transform2D::Transform2D(const Vector2f &position, float sizeX, float sizeY)
        : x(position.x), y(position.y), angle(0.0f), axisX(0.0f), axisY(0.0f), axisZ(0.0f), sizeX(sizeX), sizeY(sizeY) {
    updateMatrix();
}

void Transform2D::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    updateMatrix();
}

void Transform2D::setPosition(const Vector2f &position) {
    this->x = position.x;
    this->y = position.y;
    updateMatrix();
}

void Transform2D::setRotation(float angle, float axisX, float axisY, float axisZ) {
    this->angle = angle;
    this->axisX = axisX;
    this->axisY = axisY;
    this->axisZ = axisZ;
    updateMatrix();
}

void Transform2D::setRotation(float angle, const Vector3f &axis) {
    this->angle = angle;
    this->axisX = axis.x;
    this->axisY = axis.y;
    this->axisZ = axis.z;
    updateMatrix();
}

void Transform2D::setSizes(float sizeX, float sizeY) {
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    updateMatrix();
}

void Transform2D::setSize(float size) {
    this->sizeX = size;
    this->sizeY = size;
    updateMatrix();
}

void Transform2D::updateMatrix() {
    matrix.identity();
    matrix.translate(x, y, 0.0);
    matrix.rotate(angle, axisX, axisY, axisZ);
    matrix.scale(sizeX, sizeY, 1.0f);
}

Transform2D::~Transform2D() = default;