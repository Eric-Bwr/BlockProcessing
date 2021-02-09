#pragma once

#include <Matrix/Matrix4f.h>
#include <Vector/Vector2f.h>

class Transform2D {
public:
    Transform2D();
    Transform2D(float x, float y, float angle, float axisX, float axisY, float axisZ, float size);
    Transform2D(float x, float y, float angle, const Vector3f& axis, float size);
    Transform2D(float x, float y, float sizeX, float sizeY);
    Transform2D(float x, float y, float size);
    Transform2D(const Vector2f& position, float angle, float axisX, float axisY, float axisZ, float size);
    Transform2D(const Vector2f& position, float angle, const Vector3f& axis, float size);
    Transform2D(const Vector2f& position, float size);
    Transform2D(const Vector2f& position, float sizeX, float sizeY);
    void setPosition(float x, float y);
    void setPosition(const Vector2f& position);
    void setRotation(float angle, float axisX, float axisY, float axisZ);
    void setRotation(float angle, const Vector3f& axis);
    void setSizes(float sizeX, float sizeY);
    void setSize(float size);
    inline Vector2f getPosition() const { return Vector2f(x, y); }
    inline float getX() const { return x; }
    inline float getY() { return y; }
    inline float getAngle() const { return angle; }
    inline Matrix4f& getMatrix() { return matrix; }
    ~Transform2D();
private:
    Matrix4f matrix;
    void updateMatrix();
    float x, y, sizeX, sizeY, angle, axisX, axisY, axisZ;
};