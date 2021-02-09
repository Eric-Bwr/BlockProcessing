#pragma once

#include "../Vector/Vector3f.h"
#include "../Vector/Vector4f.h"

const float radians = 0.01745329251994329576923690768489f;

class Matrix4f {
public:
    Matrix4f() = default;
    Matrix4f& identity();
    Matrix4f& perspective(float fov, float width, float height, float near, float far);
    Matrix4f& rotate(float angle, const Vector3f& axis, const Matrix4f& from);
    Matrix4f& rotate(float angle, float x, float y, float z, const Matrix4f& from);
    Matrix4f& rotate(float angle, float x, float y, float z);
    Matrix4f& scale(float scale);
    Matrix4f& scale(float x, float y, float z);
    Matrix4f& scale(const Vector3f& scale);
    Matrix4f& scale(const Vector3f& scale, const Matrix4f& from);
    Matrix4f& translate(const Vector3f& vector, const Matrix4f& from);
    Matrix4f& translate(float x, float y, float z, const Matrix4f& from);
    Matrix4f& translate(float x, float y, float z);
    Matrix4f& lookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
    Matrix4f& orthographic(float left, float right, float bottom, float top, float near, float far);
    Matrix4f& multiply(const Matrix4f& one, const Matrix4f& two);
    Matrix4f& invert(Matrix4f &matrix, const Matrix4f& from);
    Matrix4f& invert(Matrix4f &matrix);
    Matrix4f& invert();
    Matrix4f& removeTranslation();
    float* getBuffer() const;
    void updateBuffer();
    std::string toString();
public:
    float m00 = 0.0f;
    float m01 = 0.0f;
    float m02 = 0.0f;
    float m03 = 0.0f;
    float m10 = 0.0f;
    float m11 = 0.0f;
    float m12 = 0.0f;
    float m13 = 0.0f;
    float m20 = 0.0f;
    float m21 = 0.0f;
    float m22 = 0.0f;
    float m23 = 0.0f;
    float m30 = 0.0f;
    float m31 = 0.0f;
    float m32 = 0.0f;
    float m33 = 0.0f;
    float* buffer = nullptr;
};

namespace matrix {
    Matrix4f identity();
    Matrix4f &perspective(Matrix4f &matrix, float fov, float width, float height, float near, float far);
    Matrix4f &rotate(Matrix4f &matrix, float angle, const Vector3f &axis, const Matrix4f &from);
    Matrix4f &scale(Matrix4f &matrix, float scale, const Matrix4f &from);
    Matrix4f &scale(Matrix4f &matrix, const Vector3f &scale, const Matrix4f &from);
    Matrix4f &translate(Matrix4f &matrix, const Vector3f &vector, const Matrix4f &from);
    Matrix4f &translate(Matrix4f &matrix, float x, float y, float z, const Matrix4f &from);
    Matrix4f &orthographic(Matrix4f &matrix, float left, float right, float bottom, float top, float near, float far);
    Matrix4f &multiply(Matrix4f &matrix, const Matrix4f &one, const Matrix4f &two);
    Matrix4f &invert(Matrix4f &matrix, const Matrix4f& from);
    Matrix4f multiply(const Matrix4f &one, const Matrix4f &two);
}