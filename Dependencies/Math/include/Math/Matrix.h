#pragma once

#include <cstdio>
#include <cmath>
#include "Vector.h"

class Mat4 {
public:
    double m00 = 0.0;
    double m01 = 0.0;
    double m02 = 0.0;
    double m03 = 0.0;
    double m10 = 0.0;
    double m11 = 0.0;
    double m12 = 0.0;
    double m13 = 0.0;
    double m20 = 0.0;
    double m21 = 0.0;
    double m22 = 0.0;
    double m23 = 0.0;
    double m30 = 0.0;
    double m31 = 0.0;
    double m32 = 0.0;
    double m33 = 0.0;

    Mat4() = default;
    explicit Mat4(double v);

    Mat4 &identity();
    Mat4 &translate(const Vec3 &vector);
    Mat4 &translate(double x, double y, double z);
    Mat4 &rotate(double angle, const Vec3 &axis);
    Mat4 &rotate(double angle, double x, double y, double z);
    Mat4 &scale(double scale);
    Mat4 &scale(double x, double y, double z);
    Mat4 &scale(const Vec3 &scale);
    Mat4 &multiply(const Mat4& matrix);
    Mat4 &multiply(const Mat4& one, const Mat4& two);
    Mat4 &invert();
    Mat4 &removeTranslation();
    Mat4 &orthographic(double left, double right, double bottom, double top, double near, double far);
    Mat4 &perspective(double fov, double width, double height, double near, double far);
    Mat4 &lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);

    char *toString() const;
    const double* getBuffer() const;
    double* getBuffer();

    ~Mat4();
};

Mat4 identityMatrix();
Mat4 perspectiveMatrix(double fov, double width, double height, double near, double far);
Mat4 orthographicMatrix(double left, double right, double bottom, double top, double near, double far);
Mat4 multiplyMatrix(const Mat4& one, const Mat4& two);
Mat4 lookAtMatrix(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
