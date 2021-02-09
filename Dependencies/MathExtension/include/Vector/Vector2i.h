#pragma once

#include <string>

struct Vector2i {
    int x, y;

    Vector2i();
    Vector2i(int value);
    Vector2i(int x, int y);

    friend Vector2i operator+(const Vector2i& left, const Vector2i& right);
    friend Vector2i operator+(const Vector2i& left, int value);
    friend Vector2i operator-(const Vector2i& left, const Vector2i& right);
    friend Vector2i operator-(const Vector2i& left, int value);
    friend Vector2i operator*(const Vector2i& left, const Vector2i& right);
    friend Vector2i operator*(const Vector2i& left, int value);
    friend Vector2i operator/(const Vector2i& left, const Vector2i& right);
    friend Vector2i operator/(const Vector2i& left, int value);

    bool operator==(const Vector2i& other);
    bool operator!=(const Vector2i& other);

    Vector2i& operator+=(const Vector2i& other);
    Vector2i& operator-=(const Vector2i& other);
    Vector2i& operator*=(const Vector2i& other);
    Vector2i& operator/=(const Vector2i& other);

    const char* toString();
};