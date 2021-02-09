#pragma once

#include <string>

struct Vector2f {
    float x, y;

    Vector2f();
    Vector2f(float value);
    Vector2f(float x, float y);

    friend Vector2f operator+(const Vector2f& left, const Vector2f& right);
    friend Vector2f operator+(const Vector2f& left, float value);
    friend Vector2f operator-(const Vector2f& left, const Vector2f& right);
    friend Vector2f operator-(const Vector2f& left, float value);
    friend Vector2f operator*(const Vector2f& left, const Vector2f& right);
    friend Vector2f operator*(const Vector2f& left, float value);
    friend Vector2f operator/(const Vector2f& left, const Vector2f& right);
    friend Vector2f operator/(const Vector2f& left, float value);

    bool operator==(const Vector2f& other);
    bool operator!=(const Vector2f& other);

    Vector2f& operator+=(const Vector2f& other);
    Vector2f& operator+=(const float& other);
    Vector2f& operator-=(const Vector2f& other);
    Vector2f& operator-=(const float& other);
    Vector2f& operator*=(const Vector2f& other);
    Vector2f& operator*=(const float& other);
    Vector2f& operator/=(const Vector2f& other);
    Vector2f& operator/=(const float& other);

    const char* toString();
};