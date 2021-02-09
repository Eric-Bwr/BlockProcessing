#pragma once

#include <string>

struct Vector4f {
    float x, y, z, w;

    Vector4f();
    Vector4f(float value);
    Vector4f(float x, float y, float z, float w);

    friend Vector4f operator+(const Vector4f& left, const Vector4f& right);
    friend Vector4f operator+(const Vector4f& left, float value);
    friend Vector4f operator-(const Vector4f& left, const Vector4f& right);
    friend Vector4f operator-(const Vector4f& left, float value);
    friend Vector4f operator*(const Vector4f& left, const Vector4f& right);
    friend Vector4f operator*(const Vector4f& left, float value);
    friend Vector4f operator/(const Vector4f& left, const Vector4f& right);
    friend Vector4f operator/(const Vector4f& left, float value);

    bool operator==(const Vector4f& other);
    bool operator!=(const Vector4f& other);

    Vector4f& operator+=(const Vector4f& other);
    Vector4f& operator-=(const Vector4f& other);
    Vector4f& operator*=(const Vector4f& other);
    Vector4f& operator/=(const Vector4f& other);

    const char* toString();
};