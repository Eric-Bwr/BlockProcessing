#pragma once

#include <Matrix/Matrix4f.h>
#include <Vector/Vector3f.h>

class Transform3D {
public:
    Transform3D();
    Transform3D(float x, float y, float z, float angle, float axisX, float axisY, float axisZ, float size);
    Transform3D(float x, float y, float z, float angle, const Vector3f& axis, float size);
    Transform3D(float x, float y, float z, float sizeX, float sizeY, float sizeZ);
    Transform3D(float x, float y, float z, float size);
    Transform3D(const Vector3f& position, float angle, float axisX, float axisY, float axisZ, float size);
    Transform3D(const Vector3f& position, float angle, const Vector3f& axis, float size);
    Transform3D(const Vector3f& position, float size);
    Transform3D(const Vector3f& position, float sizeX, float sizeY, float sizeZ);
    void setPosition(float x, float y, float z);
    void setPosition(const Vector3f& position);
    void setRotation(float angle, float axisX, float axisY, float axisZ);
    void setRotation(float angle, const Vector3f& axis);
    void setSizes(float sizeX, float sizeY, float sizeZ);
    void setSize(float size);
    inline Vector3f getPosition() const { return Vector3f(x, y, z); }
    inline float getX() const { return x; }
    inline float getY() { return y; }
    inline float getZ() const { return z; }
    inline float getAngle() const { return angle; }
    inline Matrix4f& getMatrix() { return matrix; }
    ~Transform3D();
private:
    Matrix4f matrix;
    void updateMatrix();
    float x, y, z, sizeX, sizeY, sizeZ, angle, axisX, axisY, axisZ;
};