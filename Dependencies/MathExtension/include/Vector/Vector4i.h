#pragma once

#include <string>

struct Vector4i {
    int x, y, z, w;

    Vector4i();
    Vector4i(int value);
    Vector4i(int x, int y, int z, int w);

    friend Vector4i operator+(const Vector4i& left, const Vector4i& right);
    friend Vector4i operator+(const Vector4i& left, int value);
    friend Vector4i operator-(const Vector4i& left, const Vector4i& right);
    friend Vector4i operator-(const Vector4i& left, int value);
    friend Vector4i operator*(const Vector4i& left, const Vector4i& right);
    friend Vector4i operator*(const Vector4i& left, int value);
    friend Vector4i operator/(const Vector4i& left, const Vector4i& right);
    friend Vector4i operator/(const Vector4i& left, int value);

    bool operator==(const Vector4i& other);
    bool operator!=(const Vector4i& other);

    Vector4i& operator+=(const Vector4i& other);
    Vector4i& operator-=(const Vector4i& other);
    Vector4i& operator*=(const Vector4i& other);
    Vector4i& operator/=(const Vector4i& other);

    const char* toString();
};