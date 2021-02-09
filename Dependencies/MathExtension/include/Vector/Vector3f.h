#pragma once

#include <string>
#include <cmath>

struct Vector3f {
    float x, y, z;

    Vector3f();
    Vector3f(float value);
    Vector3f(float x, float y, float z);

    Vector3f& cross(const Vector3f& left, const Vector3f &right);
    Vector3f& cross(const Vector3f& other);
    Vector3f& normalize();

    friend Vector3f operator+(const Vector3f& left, const Vector3f& right);
    friend Vector3f operator+(const Vector3f& left, float value);
    friend Vector3f operator-(const Vector3f& left, const Vector3f& right);
    friend Vector3f operator-(const Vector3f& left, float value);
    friend Vector3f operator*(const Vector3f& left, const Vector3f& right);
    friend Vector3f operator*(const Vector3f& left, float value);
    friend Vector3f operator/(const Vector3f& left, const Vector3f& right);
    friend Vector3f operator/(const Vector3f& left, float value);

    float length();

    bool operator==(const Vector3f& other);
    bool operator!=(const Vector3f& other);

    Vector3f& operator+=(const Vector3f& other);
    Vector3f& operator-=(const Vector3f& other);
    Vector3f& operator*=(const Vector3f& other);
    Vector3f& operator/=(const Vector3f& other);

    const char* toString();
};

Vector3f& cross(Vector3f& left, const Vector3f& right);
float dot(Vector3f& left, const Vector3f& right);
Vector3f& normalize(Vector3f& vector);