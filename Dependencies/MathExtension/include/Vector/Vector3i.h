#pragma once

#include <string>
#include <cmath>

struct Vector3i {
    int x, y, z;

    Vector3i();
    Vector3i(int value);
    Vector3i(int x, int y, int z);

    friend Vector3i operator+(const Vector3i& left, const Vector3i& right);
    friend Vector3i operator+(const Vector3i& left, int value);
    friend Vector3i operator-(const Vector3i& left, const Vector3i& right);
    friend Vector3i operator-(const Vector3i& left, int value);
    friend Vector3i operator*(const Vector3i& left, const Vector3i& right);
    friend Vector3i operator*(const Vector3i& left, int value);
    friend Vector3i operator/(const Vector3i& left, const Vector3i& right);
    friend Vector3i operator/(const Vector3i& left, int value);

    bool operator==(const Vector3i& other);
    bool operator!=(const Vector3i& other);

    Vector3i& operator+=(const Vector3i& other);
    Vector3i& operator-=(const Vector3i& other);
    Vector3i& operator*=(const Vector3i& other);
    Vector3i& operator/=(const Vector3i& other);

    const char* toString();
};