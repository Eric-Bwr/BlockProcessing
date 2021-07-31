#pragma once

#include <cmath>

class Vec2 {
public:
	double x = 0, y = 0;

	Vec2() = default;
	explicit Vec2(double v);
	Vec2(double x, double y);
	Vec2(const Vec2 &v);

	friend Vec2 operator+ (const Vec2 &a, const Vec2 &b);
	friend Vec2 operator+ (const Vec2 &a, double b);
	friend Vec2 operator- (const Vec2 &a, const Vec2 &b);
	friend Vec2 operator- (const Vec2 &a, double b);
	friend Vec2 operator* (const Vec2 &a, const Vec2 &b);
	friend Vec2 operator* (const Vec2 &a, double b);
	friend Vec2 operator/ (const Vec2 &a, const Vec2 &b);
	friend Vec2 operator/ (const Vec2 &a, double b);

	Vec2& operator+= (Vec2 const &a);
	Vec2& operator-= (Vec2 const &a);
	Vec2& operator*= (double v);
	
	bool operator== (Vec2 &a) const;
	bool operator!= (Vec2 &a) const;

	Vec2 norm() const;
	double dot(Vec2 const &a) const;
	double len() const;
	double len2() const;
};

class Vec3 {
public:
	double x = 0, y = 0, z = 0;

	Vec3() = default;
	explicit Vec3(double v);
	Vec3(double x, double y, double z);
	Vec3(const Vec2& xy, double z);
	Vec3(const Vec3& v);

	Vec2 xy() const;
	Vec2 xz() const;
	Vec2 yz() const;

	friend Vec3 operator+ (const Vec3 &a, const Vec3 &b);
	friend Vec3 operator+ (const Vec3 &a, double b);
	friend Vec3 operator- (const Vec3 &a, const Vec3 &b);
	friend Vec3 operator- (const Vec3 &a, double b);
	friend Vec3 operator* (const Vec3 &a, const Vec3 &b);
	friend Vec3 operator* (const Vec3 &a, double b);
	friend Vec3 operator/ (const Vec3 &a, const Vec3 &b);
	friend Vec3 operator/ (const Vec3 &a, double b);

	Vec3 &operator+= (const Vec3 &a);
	Vec3 &operator-= (const Vec3 &a);
	Vec3 &operator*= (double v);
	
	bool operator== (Vec3 &a) const;
	bool operator!= (Vec3 &a) const;

	Vec3 cross(Vec3 const &a) const;
	Vec3 norm() const;
	double dot(Vec3 const &a) const;
	double len() const;
	double len2() const;
};

class Vec4 {
public:
    double x = 0, y = 0, z = 0, w = 0;

	Vec4() = default;
    explicit Vec4(double v);
    Vec4(double x, double y, double z, double w);

    friend Vec4 operator+ (const Vec4 &a, const Vec4 &b);
    friend Vec4 operator+ (const Vec4 &a, double b);
    friend Vec4 operator- (const Vec4 &a, const Vec4 &b);
    friend Vec4 operator- (const Vec4 &a, double b);
    friend Vec4 operator* (const Vec4 &a, const Vec4 &b);
    friend Vec4 operator* (const Vec4 &a, double b);
    friend Vec4 operator/ (const Vec4 &a, const Vec4 &b);
    friend Vec4 operator/ (const Vec4 &a, double b);

    Vec4 &operator+= (const Vec4 &a);
    Vec4 &operator-= (const Vec4 &a);
    Vec4 &operator*= (double v);

    bool operator== (Vec4 &a) const;
    bool operator!= (Vec4 &a) const;
};