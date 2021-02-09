#include "Matrix4f.h"
#include <sstream>

Matrix4f& Matrix4f::identity() {
    m00 = 1.0f;
    m01 = 0.0f;
    m02 = 0.0f;
    m03 = 0.0f;
    m10 = 0.0f;
    m11 = 1.0f;
    m12 = 0.0f;
    m13 = 0.0f;
    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
    m23 = 0.0f;
    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;
    updateBuffer();
    return *this;
}

Matrix4f matrix::identity(){
    Matrix4f matrix;
    matrix.m00 = 1.0f;
    matrix.m01 = 0.0f;
    matrix.m02 = 0.0f;
    matrix.m03 = 0.0f;
    matrix.m10 = 0.0f;
    matrix.m11 = 1.0f;
    matrix.m12 = 0.0f;
    matrix.m13 = 0.0f;
    matrix.m20 = 0.0f;
    matrix.m21 = 0.0f;
    matrix.m22 = 1.0f;
    matrix.m23 = 0.0f;
    matrix.m30 = 0.0f;
    matrix.m31 = 0.0f;
    matrix.m32 = 0.0f;
    matrix.m33 = 1.0f;
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& Matrix4f::perspective(float fov, float width, float height, float near, float far){
    float aspectRatio = width / height;
    float yScale = std::tan(fov / 2.0f * radians);

    m00 = 1.0f / (aspectRatio * yScale);
    m11 = 1.0f / (yScale);
    m22 = far / (near - far);
    m23 = -1.0f;
    m32 = -(far * near) / (far - near);
    updateBuffer();
    return *this;
}

Matrix4f& matrix::perspective(Matrix4f& matrix, float fov, float width, float height, float near, float far){
    float aspectRatio = width / height;
    float yScale = 1.0f / (std::tan(fov * 0.5f * radians) * aspectRatio);
    float frustum_length = far - near;

    matrix.m00 = yScale / aspectRatio;
    matrix.m11 = yScale;
    matrix.m22 = -((far + near) / frustum_length);
    matrix.m23 = -1;
    matrix.m32 = -((2 * near * far) / frustum_length);
    matrix.m33 = 0;
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& Matrix4f::rotate(float angle, const Vector3f& axis, const Matrix4f& from) {
    float angleRadians = angle * radians;
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    float oneminusc = 1.0f - c;
    float xy = axis.x * axis.y;
    float yz = axis.y * axis.z;
    float xz = axis.x * axis.z;
    float xs = axis.x * s;
    float ys = axis.y * s;
    float zs = axis.z * s;
    float f00 = axis.x * axis.x * oneminusc + c;
    float f2 = xy * oneminusc + zs;
    float f3 = xz * oneminusc - ys;
    float f4 = xy * oneminusc - zs;
    float f5 = axis.y * axis.y * oneminusc + c;
    float f6 = yz * oneminusc + xs;
    float f7 = xz * oneminusc + ys;
    float f8 = yz * oneminusc - xs;
    float f9 = axis.z * axis.z * oneminusc + c;
    float e1 = from.m00 * f00 + from.m10 * f2 + from.m20 * f3;
    float e2 = from.m01 * f00 + from.m11 * f2 + from.m21 * f3;
    float e3 = from.m02 * f00 + from.m12 * f2 + from.m22 * f3;
    float e4 = from.m03 * f00 + from.m13 * f2 + from.m23 * f3;
    float e5 = from.m00 * f4 + from.m10 * f5 + from.m20 * f6;
    float e6 = from.m01 * f4 + from.m11 * f5 + from.m21 * f6;
    float e7 = from.m02 * f4 + from.m12 * f5 + from.m22 * f6;
    float e8 = from.m03 * f4 + from.m13 * f5 + from.m23 * f6;
    float e9 = from.m00 * f7 + from.m10 * f8 + from.m20 * f9;
    float e10 = from.m01 * f7 + from.m11 * f8 + from.m21 * f9;
    float e11 = from.m02 * f7 + from.m12 * f8 + from.m22 * f9;
    float e12 = from.m03 * f7 + from.m13 * f8 + from.m23 * f9;
    m00 = e1;
    m01 = e2;
    m02 = e3;
    m03 = e4;
    m10 = e5;
    m11 = e6;
    m12 = e7;
    m13 = e8;
    m20 = e9;
    m21 = e10;
    m22 = e11;
    m23 = e12;
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::rotate(float angle, float x, float y, float z, const Matrix4f& from) {
    float angleRadians = angle * radians;
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    float oneminusc = 1.0f - c;
    float xy = x * y;
    float yz = y * z;
    float xz = x * z;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;
    float f00 = x * x * oneminusc + c;
    float f2 = xy * oneminusc + zs;
    float f3 = xz * oneminusc - ys;
    float f4 = xy * oneminusc - zs;
    float f5 = y * y * oneminusc + c;
    float f6 = yz * oneminusc + xs;
    float f7 = xz * oneminusc + ys;
    float f8 = yz * oneminusc - xs;
    float f9 = z * z * oneminusc + c;
    float e1 = from.m00 * f00 + from.m10 * f2 + from.m20 * f3;
    float e2 = from.m01 * f00 + from.m11 * f2 + from.m21 * f3;
    float e3 = from.m02 * f00 + from.m12 * f2 + from.m22 * f3;
    float e4 = from.m03 * f00 + from.m13 * f2 + from.m23 * f3;
    float e5 = from.m00 * f4 + from.m10 * f5 + from.m20 * f6;
    float e6 = from.m01 * f4 + from.m11 * f5 + from.m21 * f6;
    float e7 = from.m02 * f4 + from.m12 * f5 + from.m22 * f6;
    float e8 = from.m03 * f4 + from.m13 * f5 + from.m23 * f6;
    float e9 = from.m00 * f7 + from.m10 * f8 + from.m20 * f9;
    float e10 = from.m01 * f7 + from.m11 * f8 + from.m21 * f9;
    float e11 = from.m02 * f7 + from.m12 * f8 + from.m22 * f9;
    float e12 = from.m03 * f7 + from.m13 * f8 + from.m23 * f9;
    m00 = e1;
    m01 = e2;
    m02 = e3;
    m03 = e4;
    m10 = e5;
    m11 = e6;
    m12 = e7;
    m13 = e8;
    m20 = e9;
    m21 = e10;
    m22 = e11;
    m23 = e12;
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::rotate(float angle, float x, float y, float z) {
    float angleRadians = angle * radians;
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    float oneminusc = 1.0f - c;
    float xy = x * y;
    float yz = y * z;
    float xz = x * z;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;
    float f00 = x * x * oneminusc + c;
    float f2 = xy * oneminusc + zs;
    float f3 = xz * oneminusc - ys;
    float f4 = xy * oneminusc - zs;
    float f5 = y * y * oneminusc + c;
    float f6 = yz * oneminusc + xs;
    float f7 = xz * oneminusc + ys;
    float f8 = yz * oneminusc - xs;
    float f9 = z * z * oneminusc + c;
    float e1 = m00 * f00 + m10 * f2 + m20 * f3;
    float e2 = m01 * f00 + m11 * f2 + m21 * f3;
    float e3 = m02 * f00 + m12 * f2 + m22 * f3;
    float e4 = m03 * f00 + m13 * f2 + m23 * f3;
    float e5 = m00 * f4 + m10 * f5 + m20 * f6;
    float e6 = m01 * f4 + m11 * f5 + m21 * f6;
    float e7 = m02 * f4 + m12 * f5 + m22 * f6;
    float e8 = m03 * f4 + m13 * f5 + m23 * f6;
    float e9 = m00 * f7 + m10 * f8 + m20 * f9;
    float e10 = m01 * f7 + m11 * f8 + m21 * f9;
    float e11 = m02 * f7 + m12 * f8 + m22 * f9;
    float e12 = m03 * f7 + m13 * f8 + m23 * f9;
    m00 = e1;
    m01 = e2;
    m02 = e3;
    m03 = e4;
    m10 = e5;
    m11 = e6;
    m12 = e7;
    m13 = e8;
    m20 = e9;
    m21 = e10;
    m22 = e11;
    m23 = e12;
    updateBuffer();
    return *this;
}

Matrix4f& matrix::rotate(Matrix4f& matrix, float angle, const Vector3f& axis, const Matrix4f& from) {
    float angleRadians = angle * radians;
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    float oneminusc = 1.0f - c;
    float xy = axis.x * axis.y;
    float yz = axis.y * axis.z;
    float xz = axis.x * axis.z;
    float xs = axis.x * s;
    float ys = axis.y * s;
    float zs = axis.z * s;
    float f00 = axis.x * axis.x * oneminusc + c;
    float f2 = xy * oneminusc + zs;
    float f3 = xz * oneminusc - ys;
    float f4 = xy * oneminusc - zs;
    float f5 = axis.y * axis.y * oneminusc + c;
    float f6 = yz * oneminusc + xs;
    float f7 = xz * oneminusc + ys;
    float f8 = yz * oneminusc - xs;
    float f9 = axis.z * axis.z * oneminusc + c;
    float e1 = from.m00 * f00 + from.m10 * f2 + from.m20 * f3;
    float e2 = from.m01 * f00 + from.m11 * f2 + from.m21 * f3;
    float e3 = from.m02 * f00 + from.m12 * f2 + from.m22 * f3;
    float e4 = from.m03 * f00 + from.m13 * f2 + from.m23 * f3;
    float e5 = from.m00 * f4 + from.m10 * f5 + from.m20 * f6;
    float e6 = from.m01 * f4 + from.m11 * f5 + from.m21 * f6;
    float e7 = from.m02 * f4 + from.m12 * f5 + from.m22 * f6;
    float e8 = from.m03 * f4 + from.m13 * f5 + from.m23 * f6;
    float e9 = from.m00 * f7 + from.m10 * f8 + from.m20 * f9;
    float e10 = from.m01 * f7 + from.m11 * f8 + from.m21 * f9;
    float e11 = from.m02 * f7 + from.m12 * f8 + from.m22 * f9;
    float e12 = from.m03 * f7 + from.m13 * f8 + from.m23 * f9;
    matrix.m00 = e1;
    matrix.m01 = e2;
    matrix.m02 = e3;
    matrix.m03 = e4;
    matrix.m10 = e5;
    matrix.m11 = e6;
    matrix.m12 = e7;
    matrix.m13 = e8;
    matrix.m20 = e9;
    matrix.m21 = e10;
    matrix.m22 = e11;
    matrix.m23 = e12;
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& Matrix4f::scale(float scale) {
    m00 *= scale;
    m01 *= scale;
    m02 *= scale;
    m03 *= scale;
    m10 *= scale;
    m11 *= scale;
    m12 *= scale;
    m13 *= scale;
    m20 *= scale;
    m21 *= scale;
    m22 *= scale;
    m23 *= scale;
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::scale(const Vector3f& scale) {
    m00 *= scale.x;
    m01 *= scale.x;
    m02 *= scale.x;
    m03 *= scale.x;
    m10 *= scale.y;
    m11 *= scale.y;
    m12 *= scale.y;
    m13 *= scale.y;
    m20 *= scale.z;
    m21 *= scale.z;
    m22 *= scale.z;
    m23 *= scale.z;
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::scale(float x, float y, float z) {
    m00 *= x;
    m01 *= x;
    m02 *= x;
    m03 *= x;
    m10 *= y;
    m11 *= y;
    m12 *= y;
    m13 *= y;
    m20 *= z;
    m21 *= z;
    m22 *= z;
    m23 *= z;
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::scale(const Vector3f& scale, const Matrix4f& from) {
    m00 = from.m00 * scale.x;
    m01 = from.m01 * scale.x;
    m02 = from.m02 * scale.x;
    m03 = from.m03 * scale.x;
    m10 = from.m10 * scale.y;
    m11 = from.m11 * scale.y;
    m12 = from.m12 * scale.y;
    m13 = from.m13 * scale.y;
    m20 = from.m20 * scale.z;
    m21 = from.m21 * scale.z;
    m22 = from.m22 * scale.z;
    m23 = from.m23 * scale.z;
    updateBuffer();
    return *this;
}

Matrix4f& matrix::scale(Matrix4f& matrix, const Vector3f& scale, const Matrix4f& from) {
    matrix.m00 = from.m00 * scale.x;
    matrix.m01 = from.m01 * scale.x;
    matrix.m02 = from.m02 * scale.x;
    matrix.m03 = from.m03 * scale.x;
    matrix.m10 = from.m10 * scale.y;
    matrix.m11 = from.m11 * scale.y;
    matrix.m12 = from.m12 * scale.y;
    matrix.m13 = from.m13 * scale.y;
    matrix.m20 = from.m20 * scale.z;
    matrix.m21 = from.m21 * scale.z;
    matrix.m22 = from.m22 * scale.z;
    matrix.m23 = from.m23 * scale.z;
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& matrix::scale(Matrix4f& matrix, float scale, const Matrix4f& from) {
    matrix.m00 = from.m00 * scale;
    matrix.m01 = from.m01 * scale;
    matrix.m02 = from.m02 * scale;
    matrix.m03 = from.m03 * scale;
    matrix.m10 = from.m10 * scale;
    matrix.m11 = from.m11 * scale;
    matrix.m12 = from.m12 * scale;
    matrix.m13 = from.m13 * scale;
    matrix.m20 = from.m20 * scale;
    matrix.m21 = from.m21 * scale;
    matrix.m22 = from.m22 * scale;
    matrix.m23 = from.m23 * scale;
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& Matrix4f::translate(const Vector3f& vector, const Matrix4f& from) {
    m30 = from.m00 * vector.x + from.m10 * vector.y + from.m20 * vector.z + from.m30;
    m31 = from.m01 * vector.x + from.m11 * vector.y + from.m21 * vector.z + from.m31;
    m32 = from.m02 * vector.x + from.m12 * vector.y + from.m22 * vector.z + from.m32;
    m33 = from.m03 * vector.x + from.m13 * vector.y + from.m23 * vector.z + from.m33;
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::translate(float x, float y, float z, const Matrix4f& from) {
    m30 += from.m00 * x + from.m10 * y + from.m20 * z + from.m30;
    m31 += from.m01 * x + from.m11 * y + from.m21 * z + from.m31;
    m32 += from.m02 * x + from.m12 * y + from.m22 * z + from.m32;
    m33 += from.m03 * x + from.m13 * y + from.m23 * z + from.m33;
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::translate(float x, float y, float z) {
    m30 += m00 * x + m10 * y + m20 * z;
    m31 += m01 * x + m11 * y + m21 * z;
    m32 += m02 * x + m12 * y + m22 * z;
    m33 += m03 * x + m13 * y + m23 * z;
    updateBuffer();
    return *this;
}

Matrix4f& matrix::translate(Matrix4f& matrix, const Vector3f& vector, const Matrix4f& from) {
    matrix.m30 += from.m00 * vector.x + from.m10 * vector.y + from.m20 * vector.z + from.m30;
    matrix.m31 += from.m01 * vector.x + from.m11 * vector.y + from.m21 * vector.z + from.m31;
    matrix.m32 += from.m02 * vector.x + from.m12 * vector.y + from.m22 * vector.z + from.m32;
    matrix.m33 += from.m03 * vector.x + from.m13 * vector.y + from.m23 * vector.z + from.m33;
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& matrix::translate(Matrix4f& matrix, float x, float y, float z, const Matrix4f& from) {
    matrix.m30 += from.m00 * x + from.m10 * y + from.m20 * z + from.m30;
    matrix.m31 += from.m01 * x + from.m11 * y + from.m21 * z + from.m31;
    matrix.m32 += from.m02 * x + from.m12 * y + from.m22 * z + from.m32;
    matrix.m33 += from.m03 * x + from.m13 * y + from.m23 * z + from.m33;
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& Matrix4f::lookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) {
    Vector3f f((center - eye).normalize());
    Vector3f s;
    s.cross(f, up).normalize();
    Vector3f u;
    u.cross(s, f);
    m00 = s.x;
    m10 = s.y;
    m20 = s.z;
    m01 = u.x;
    m11 = u.y;
    m21 = u.z;
    m02 =-f.x;
    m12 =-f.y;
    m22 =-f.z;
    m30 =-dot(s, eye);
    m31 =-dot(u, eye);
    m32 = dot(f, eye);
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::orthographic(float left, float right, float bottom, float top, float near, float far){
    m00 = 2.0f / (right - left);
    m11 = 2.0f / (top - bottom);
    m22 = 2.0f / (near - far);

    m30 = (left + right) / (left - right);
    m31 = (bottom + top) / (bottom - top);
    m32 = (far + near) / (far - near);
    updateBuffer();
    return *this;
}

Matrix4f& matrix::orthographic(Matrix4f& matrix, float left, float right, float bottom, float top, float near, float far){
    matrix.m00 = 2.0F / (right - left);
    matrix.m11 = 2.0F / (top - bottom);
    matrix.m22 = 2.0F / (far - near);

    matrix.m30 = (right + left) / (right - left);
    matrix.m31 = (top + bottom) / (top - bottom);
    matrix.m32 = (far + near) / (far - near);
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& Matrix4f::multiply(const Matrix4f& one, const Matrix4f& two) {
    m00 = one.m00 * two.m00 + one.m10 * two.m01 + one.m20 * two.m02 + one.m30 * two.m03;
    m01 = one.m01 * two.m00 + one.m11 * two.m01 + one.m21 * two.m02 + one.m31 * two.m03;
    m02 = one.m02 * two.m00 + one.m12 * two.m01 + one.m22 * two.m02 + one.m32 * two.m03;
    m03 = one.m03 * two.m00 + one.m13 * two.m01 + one.m23 * two.m02 + one.m33 * two.m03;
    m10 = one.m00 * two.m10 + one.m10 * two.m11 + one.m20 * two.m12 + one.m30 * two.m13;
    m11 = one.m01 * two.m10 + one.m11 * two.m11 + one.m21 * two.m12 + one.m31 * two.m13;
    m12 = one.m02 * two.m10 + one.m12 * two.m11 + one.m22 * two.m12 + one.m32 * two.m13;
    m13 = one.m03 * two.m10 + one.m13 * two.m11 + one.m23 * two.m12 + one.m33 * two.m13;
    m20 = one.m00 * two.m20 + one.m10 * two.m21 + one.m20 * two.m22 + one.m30 * two.m23;
    m21 = one.m01 * two.m20 + one.m11 * two.m21 + one.m21 * two.m22 + one.m31 * two.m23;
    m22 = one.m02 * two.m20 + one.m12 * two.m21 + one.m22 * two.m22 + one.m32 * two.m23;
    m23 = one.m03 * two.m20 + one.m13 * two.m21 + one.m23 * two.m22 + one.m33 * two.m23;
    m30 = one.m00 * two.m30 + one.m10 * two.m31 + one.m20 * two.m32 + one.m30 * two.m33;
    m31 = one.m01 * two.m30 + one.m11 * two.m31 + one.m21 * two.m32 + one.m31 * two.m33;
    m32 = one.m02 * two.m30 + one.m12 * two.m31 + one.m22 * two.m32 + one.m32 * two.m33;
    m33 = one.m03 * two.m30 + one.m13 * two.m31 + one.m23 * two.m32 + one.m33 * two.m33;
    updateBuffer();
    return *this;
}

Matrix4f matrix::multiply(const Matrix4f& one, const Matrix4f& two) {
    Matrix4f matrix;
    matrix.m00 = one.m00 * two.m00 + one.m10 * two.m01 + one.m20 * two.m02 + one.m30 * two.m03;
    matrix.m01 = one.m01 * two.m00 + one.m11 * two.m01 + one.m21 * two.m02 + one.m31 * two.m03;
    matrix.m02 = one.m02 * two.m00 + one.m12 * two.m01 + one.m22 * two.m02 + one.m32 * two.m03;
    matrix.m03 = one.m03 * two.m00 + one.m13 * two.m01 + one.m23 * two.m02 + one.m33 * two.m03;
    matrix.m10 = one.m00 * two.m10 + one.m10 * two.m11 + one.m20 * two.m12 + one.m30 * two.m13;
    matrix.m11 = one.m01 * two.m10 + one.m11 * two.m11 + one.m21 * two.m12 + one.m31 * two.m13;
    matrix.m12 = one.m02 * two.m10 + one.m12 * two.m11 + one.m22 * two.m12 + one.m32 * two.m13;
    matrix.m13 = one.m03 * two.m10 + one.m13 * two.m11 + one.m23 * two.m12 + one.m33 * two.m13;
    matrix.m20 = one.m00 * two.m20 + one.m10 * two.m21 + one.m20 * two.m22 + one.m30 * two.m23;
    matrix.m21 = one.m01 * two.m20 + one.m11 * two.m21 + one.m21 * two.m22 + one.m31 * two.m23;
    matrix.m22 = one.m02 * two.m20 + one.m12 * two.m21 + one.m22 * two.m22 + one.m32 * two.m23;
    matrix.m23 = one.m03 * two.m20 + one.m13 * two.m21 + one.m23 * two.m22 + one.m33 * two.m23;
    matrix.m30 = one.m00 * two.m30 + one.m10 * two.m31 + one.m20 * two.m32 + one.m30 * two.m33;
    matrix.m31 = one.m01 * two.m30 + one.m11 * two.m31 + one.m21 * two.m32 + one.m31 * two.m33;
    matrix.m32 = one.m02 * two.m30 + one.m12 * two.m31 + one.m22 * two.m32 + one.m32 * two.m33;
    matrix.m33 = one.m03 * two.m30 + one.m13 * two.m31 + one.m23 * two.m32 + one.m33 * two.m33;
    matrix.updateBuffer();
    return matrix;
}

Matrix4f& matrix::multiply(Matrix4f& matrix, const Matrix4f& one, const Matrix4f& two) {
    matrix.m00 = one.m00 * two.m00 + one.m10 * two.m01 + one.m20 * two.m02 + one.m30 * two.m03;
    matrix.m01 = one.m01 * two.m00 + one.m11 * two.m01 + one.m21 * two.m02 + one.m31 * two.m03;
    matrix.m02 = one.m02 * two.m00 + one.m12 * two.m01 + one.m22 * two.m02 + one.m32 * two.m03;
    matrix.m03 = one.m03 * two.m00 + one.m13 * two.m01 + one.m23 * two.m02 + one.m33 * two.m03;
    matrix.m10 = one.m00 * two.m10 + one.m10 * two.m11 + one.m20 * two.m12 + one.m30 * two.m13;
    matrix.m11 = one.m01 * two.m10 + one.m11 * two.m11 + one.m21 * two.m12 + one.m31 * two.m13;
    matrix.m12 = one.m02 * two.m10 + one.m12 * two.m11 + one.m22 * two.m12 + one.m32 * two.m13;
    matrix.m13 = one.m03 * two.m10 + one.m13 * two.m11 + one.m23 * two.m12 + one.m33 * two.m13;
    matrix.m20 = one.m00 * two.m20 + one.m10 * two.m21 + one.m20 * two.m22 + one.m30 * two.m23;
    matrix.m21 = one.m01 * two.m20 + one.m11 * two.m21 + one.m21 * two.m22 + one.m31 * two.m23;
    matrix.m22 = one.m02 * two.m20 + one.m12 * two.m21 + one.m22 * two.m22 + one.m32 * two.m23;
    matrix.m23 = one.m03 * two.m20 + one.m13 * two.m21 + one.m23 * two.m22 + one.m33 * two.m23;
    matrix.m30 = one.m00 * two.m30 + one.m10 * two.m31 + one.m20 * two.m32 + one.m30 * two.m33;
    matrix.m31 = one.m01 * two.m30 + one.m11 * two.m31 + one.m21 * two.m32 + one.m31 * two.m33;
    matrix.m32 = one.m02 * two.m30 + one.m12 * two.m31 + one.m22 * two.m32 + one.m32 * two.m33;
    matrix.m33 = one.m03 * two.m30 + one.m13 * two.m31 + one.m23 * two.m32 + one.m33 * two.m33;
    matrix.updateBuffer();
    return matrix;
}

float determinant3x3(float t00, float t01, float t02, float t10, float t11, float t12, float t20, float t21, float t22) {
    return   t00 * (t11 * t22 - t12 * t21)
             + t01 * (t12 * t20 - t10 * t22)
             + t02 * (t10 * t21 - t11 * t20);
}

Matrix4f& matrix::invert(Matrix4f &matrix, const Matrix4f& from) {
    float f = from.m00 * ((from.m11 * from.m22 * from.m33 + from.m12 * from.m23 * from.m31 + from.m13 * from.m21 * from.m32)
            - from.m13 * from.m22 * from.m31- from.m11 * from.m23 * from.m32- from.m12 * from.m21 * from.m33);
    f -= from.m01 * ((from.m10 * from.m22 * from.m33 + from.m12 * from.m23 * from.m30 + from.m13 * from.m20 * from.m32)
            - from.m13 * from.m22 * from.m30- from.m10 * from.m23 * from.m32- from.m12 * from.m20 * from.m33);
    f += from.m02 * ((from.m10 * from.m21 * from.m33 + from.m11 * from.m23 * from.m30 + from.m13 * from.m20 * from.m31)
            - from.m13 * from.m21 * from.m30- from.m10 * from.m23 * from.m31- from.m11 * from.m20 * from.m33);
    f -= from.m03 * ((from.m10 * from.m21 * from.m32 + from.m11 * from.m22 * from.m30 + from.m12 * from.m20 * from.m31)
            - from.m12 * from.m21 * from.m30- from.m10 * from.m22 * from.m31- from.m11 * from.m20 * from.m32);
    float determinant_inv = 1.0f / f;

    float t00 =  determinant3x3(from.m11, from.m12, from.m13, from.m21, from.m22, from.m23, from.m31, from.m32, from.m33);
    float t01 = -determinant3x3(from.m10, from.m12, from.m13, from.m20, from.m22, from.m23, from.m30, from.m32, from.m33);
    float t02 =  determinant3x3(from.m10, from.m11, from.m13, from.m20, from.m21, from.m23, from.m30, from.m31, from.m33);
    float t03 = -determinant3x3(from.m10, from.m11, from.m12, from.m20, from.m21, from.m22, from.m30, from.m31, from.m32);

    float t10 = -determinant3x3(from.m01, from.m02, from.m03, from.m21, from.m22, from.m23, from.m31, from.m32, from.m33);
    float t11 =  determinant3x3(from.m00, from.m02, from.m03, from.m20, from.m22, from.m23, from.m30, from.m32, from.m33);
    float t12 = -determinant3x3(from.m00, from.m01, from.m03, from.m20, from.m21, from.m23, from.m30, from.m31, from.m33);
    float t13 =  determinant3x3(from.m00, from.m01, from.m02, from.m20, from.m21, from.m22, from.m30, from.m31, from.m32);

    float t20 =  determinant3x3(from.m01, from.m02, from.m03, from.m11, from.m12, from.m13, from.m31, from.m32, from.m33);
    float t21 = -determinant3x3(from.m00, from.m02, from.m03, from.m10, from.m12, from.m13, from.m30, from.m32, from.m33);
    float t22 =  determinant3x3(from.m00, from.m01, from.m03, from.m10, from.m11, from.m13, from.m30, from.m31, from.m33);
    float t23 = -determinant3x3(from.m00, from.m01, from.m02, from.m10, from.m11, from.m12, from.m30, from.m31, from.m32);

    float t30 = -determinant3x3(from.m01, from.m02, from.m03, from.m11, from.m12, from.m13, from.m21, from.m22, from.m23);
    float t31 =  determinant3x3(from.m00, from.m02, from.m03, from.m10, from.m12, from.m13, from.m20, from.m22, from.m23);
    float t32 = -determinant3x3(from.m00, from.m01, from.m03, from.m10, from.m11, from.m13, from.m20, from.m21, from.m23);
    float t33 =  determinant3x3(from.m00, from.m01, from.m02, from.m10, from.m11, from.m12, from.m20, from.m21, from.m22);

    matrix.m00 = t00 * determinant_inv;
    matrix.m11 = t11 * determinant_inv;
    matrix.m22 = t22 * determinant_inv;
    matrix.m33 = t33 * determinant_inv;
    matrix.m01 = t10 * determinant_inv;
    matrix.m10 = t01 * determinant_inv;
    matrix.m20 = t02 * determinant_inv;
    matrix.m02 = t20 * determinant_inv;
    matrix.m12 = t21 * determinant_inv;
    matrix.m21 = t12 * determinant_inv;
    matrix.m03 = t30 * determinant_inv;
    matrix.m30 = t03 * determinant_inv;
    matrix.m13 = t31 * determinant_inv;
    matrix.m31 = t13 * determinant_inv;
    matrix.m32 = t23 * determinant_inv;
    matrix.m23 = t32 * determinant_inv;
    matrix.updateBuffer();
    return matrix;
}


Matrix4f& Matrix4f::invert(Matrix4f &matrix, const Matrix4f& from) {
    float f = from.m00 * ((from.m11 * from.m22 * from.m33 + from.m12 * from.m23 * from.m31 + from.m13 * from.m21 * from.m32)
                          - from.m13 * from.m22 * from.m31- from.m11 * from.m23 * from.m32- from.m12 * from.m21 * from.m33);
    f -= from.m01 * ((from.m10 * from.m22 * from.m33 + from.m12 * from.m23 * from.m30 + from.m13 * from.m20 * from.m32)
                     - from.m13 * from.m22 * from.m30- from.m10 * from.m23 * from.m32- from.m12 * from.m20 * from.m33);
    f += from.m02 * ((from.m10 * from.m21 * from.m33 + from.m11 * from.m23 * from.m30 + from.m13 * from.m20 * from.m31)
                     - from.m13 * from.m21 * from.m30- from.m10 * from.m23 * from.m31- from.m11 * from.m20 * from.m33);
    f -= from.m03 * ((from.m10 * from.m21 * from.m32 + from.m11 * from.m22 * from.m30 + from.m12 * from.m20 * from.m31)
                     - from.m12 * from.m21 * from.m30- from.m10 * from.m22 * from.m31- from.m11 * from.m20 * from.m32);
    float determinant_inv = 1.0f / f;

    float t00 =  determinant3x3(from.m11, from.m12, from.m13, from.m21, from.m22, from.m23, from.m31, from.m32, from.m33);
    float t01 = -determinant3x3(from.m10, from.m12, from.m13, from.m20, from.m22, from.m23, from.m30, from.m32, from.m33);
    float t02 =  determinant3x3(from.m10, from.m11, from.m13, from.m20, from.m21, from.m23, from.m30, from.m31, from.m33);
    float t03 = -determinant3x3(from.m10, from.m11, from.m12, from.m20, from.m21, from.m22, from.m30, from.m31, from.m32);

    float t10 = -determinant3x3(from.m01, from.m02, from.m03, from.m21, from.m22, from.m23, from.m31, from.m32, from.m33);
    float t11 =  determinant3x3(from.m00, from.m02, from.m03, from.m20, from.m22, from.m23, from.m30, from.m32, from.m33);
    float t12 = -determinant3x3(from.m00, from.m01, from.m03, from.m20, from.m21, from.m23, from.m30, from.m31, from.m33);
    float t13 =  determinant3x3(from.m00, from.m01, from.m02, from.m20, from.m21, from.m22, from.m30, from.m31, from.m32);

    float t20 =  determinant3x3(from.m01, from.m02, from.m03, from.m11, from.m12, from.m13, from.m31, from.m32, from.m33);
    float t21 = -determinant3x3(from.m00, from.m02, from.m03, from.m10, from.m12, from.m13, from.m30, from.m32, from.m33);
    float t22 =  determinant3x3(from.m00, from.m01, from.m03, from.m10, from.m11, from.m13, from.m30, from.m31, from.m33);
    float t23 = -determinant3x3(from.m00, from.m01, from.m02, from.m10, from.m11, from.m12, from.m30, from.m31, from.m32);

    float t30 = -determinant3x3(from.m01, from.m02, from.m03, from.m11, from.m12, from.m13, from.m21, from.m22, from.m23);
    float t31 =  determinant3x3(from.m00, from.m02, from.m03, from.m10, from.m12, from.m13, from.m20, from.m22, from.m23);
    float t32 = -determinant3x3(from.m00, from.m01, from.m03, from.m10, from.m11, from.m13, from.m20, from.m21, from.m23);
    float t33 =  determinant3x3(from.m00, from.m01, from.m02, from.m10, from.m11, from.m12, from.m20, from.m21, from.m22);

    matrix.m00 = t00 * determinant_inv;
    matrix.m11 = t11 * determinant_inv;
    matrix.m22 = t22 * determinant_inv;
    matrix.m33 = t33 * determinant_inv;
    matrix.m01 = t10 * determinant_inv;
    matrix.m10 = t01 * determinant_inv;
    matrix.m20 = t02 * determinant_inv;
    matrix.m02 = t20 * determinant_inv;
    matrix.m12 = t21 * determinant_inv;
    matrix.m21 = t12 * determinant_inv;
    matrix.m03 = t30 * determinant_inv;
    matrix.m30 = t03 * determinant_inv;
    matrix.m13 = t31 * determinant_inv;
    matrix.m31 = t13 * determinant_inv;
    matrix.m32 = t23 * determinant_inv;
    matrix.m23 = t32 * determinant_inv;
    updateBuffer();
    return matrix;
}

Matrix4f& Matrix4f::invert(Matrix4f &matrix) {
    float f = matrix.m00 * ((matrix.m11 * matrix.m22 * matrix.m33 + matrix.m12 * matrix.m23 * matrix.m31 + matrix.m13 * matrix.m21 * matrix.m32)
                          - matrix.m13 * matrix.m22 * matrix.m31- matrix.m11 * matrix.m23 * matrix.m32- matrix.m12 * matrix.m21 * matrix.m33);
    f -= matrix.m01 * ((matrix.m10 * matrix.m22 * matrix.m33 + matrix.m12 * matrix.m23 * matrix.m30 + matrix.m13 * matrix.m20 * matrix.m32)
                     - matrix.m13 * matrix.m22 * matrix.m30- matrix.m10 * matrix.m23 * matrix.m32- matrix.m12 * matrix.m20 * matrix.m33);
    f += matrix.m02 * ((matrix.m10 * matrix.m21 * matrix.m33 + matrix.m11 * matrix.m23 * matrix.m30 + matrix.m13 * matrix.m20 * matrix.m31)
                     - matrix.m13 * matrix.m21 * matrix.m30- matrix.m10 * matrix.m23 * matrix.m31- matrix.m11 * matrix.m20 * matrix.m33);
    f -= matrix.m03 * ((matrix.m10 * matrix.m21 * matrix.m32 + matrix.m11 * matrix.m22 * matrix.m30 + matrix.m12 * matrix.m20 * matrix.m31)
                     - matrix.m12 * matrix.m21 * matrix.m30- matrix.m10 * matrix.m22 * matrix.m31- matrix.m11 * matrix.m20 * matrix.m32);
    float determinant_inv = 1.0f / f;

    float t00 =  determinant3x3(matrix.m11, matrix.m12, matrix.m13, matrix.m21, matrix.m22, matrix.m23, matrix.m31, matrix.m32, matrix.m33);
    float t01 = -determinant3x3(matrix.m10, matrix.m12, matrix.m13, matrix.m20, matrix.m22, matrix.m23, matrix.m30, matrix.m32, matrix.m33);
    float t02 =  determinant3x3(matrix.m10, matrix.m11, matrix.m13, matrix.m20, matrix.m21, matrix.m23, matrix.m30, matrix.m31, matrix.m33);
    float t03 = -determinant3x3(matrix.m10, matrix.m11, matrix.m12, matrix.m20, matrix.m21, matrix.m22, matrix.m30, matrix.m31, matrix.m32);

    float t10 = -determinant3x3(matrix.m01, matrix.m02, matrix.m03, matrix.m21, matrix.m22, matrix.m23, matrix.m31, matrix.m32, matrix.m33);
    float t11 =  determinant3x3(matrix.m00, matrix.m02, matrix.m03, matrix.m20, matrix.m22, matrix.m23, matrix.m30, matrix.m32, matrix.m33);
    float t12 = -determinant3x3(matrix.m00, matrix.m01, matrix.m03, matrix.m20, matrix.m21, matrix.m23, matrix.m30, matrix.m31, matrix.m33);
    float t13 =  determinant3x3(matrix.m00, matrix.m01, matrix.m02, matrix.m20, matrix.m21, matrix.m22, matrix.m30, matrix.m31, matrix.m32);

    float t20 =  determinant3x3(matrix.m01, matrix.m02, matrix.m03, matrix.m11, matrix.m12, matrix.m13, matrix.m31, matrix.m32, matrix.m33);
    float t21 = -determinant3x3(matrix.m00, matrix.m02, matrix.m03, matrix.m10, matrix.m12, matrix.m13, matrix.m30, matrix.m32, matrix.m33);
    float t22 =  determinant3x3(matrix.m00, matrix.m01, matrix.m03, matrix.m10, matrix.m11, matrix.m13, matrix.m30, matrix.m31, matrix.m33);
    float t23 = -determinant3x3(matrix.m00, matrix.m01, matrix.m02, matrix.m10, matrix.m11, matrix.m12, matrix.m30, matrix.m31, matrix.m32);

    float t30 = -determinant3x3(matrix.m01, matrix.m02, matrix.m03, matrix.m11, matrix.m12, matrix.m13, matrix.m21, matrix.m22, matrix.m23);
    float t31 =  determinant3x3(matrix.m00, matrix.m02, matrix.m03, matrix.m10, matrix.m12, matrix.m13, matrix.m20, matrix.m22, matrix.m23);
    float t32 = -determinant3x3(matrix.m00, matrix.m01, matrix.m03, matrix.m10, matrix.m11, matrix.m13, matrix.m20, matrix.m21, matrix.m23);
    float t33 =  determinant3x3(matrix.m00, matrix.m01, matrix.m02, matrix.m10, matrix.m11, matrix.m12, matrix.m20, matrix.m21, matrix.m22);

    matrix.m00 = t00 * determinant_inv;
    matrix.m11 = t11 * determinant_inv;
    matrix.m22 = t22 * determinant_inv;
    matrix.m33 = t33 * determinant_inv;
    matrix.m01 = t10 * determinant_inv;
    matrix.m10 = t01 * determinant_inv;
    matrix.m20 = t02 * determinant_inv;
    matrix.m02 = t20 * determinant_inv;
    matrix.m12 = t21 * determinant_inv;
    matrix.m21 = t12 * determinant_inv;
    matrix.m03 = t30 * determinant_inv;
    matrix.m30 = t03 * determinant_inv;
    matrix.m13 = t31 * determinant_inv;
    matrix.m31 = t13 * determinant_inv;
    matrix.m32 = t23 * determinant_inv;
    matrix.m23 = t32 * determinant_inv;
    updateBuffer();
    return matrix;
}

Matrix4f& Matrix4f::invert() {
    float f = m00 * ((m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32)
                            - m13 * m22 * m31- m11 * m23 * m32- m12 * m21 * m33);
    f -= m01 * ((m10 * m22 * m33 + m12 * m23 * m30 + m13 * m20 * m32)
                       - m13 * m22 * m30- m10 * m23 * m32- m12 * m20 * m33);
    f += m02 * ((m10 * m21 * m33 + m11 * m23 * m30 + m13 * m20 * m31)
                       - m13 * m21 * m30- m10 * m23 * m31- m11 * m20 * m33);
    f -= m03 * ((m10 * m21 * m32 + m11 * m22 * m30 + m12 * m20 * m31)
                       - m12 * m21 * m30- m10 * m22 * m31- m11 * m20 * m32);
    float determinant_inv = 1.0f / f;

    float t00 =  determinant3x3(m11, m12, m13, m21, m22, m23, m31, m32, m33);
    float t01 = -determinant3x3(m10, m12, m13, m20, m22, m23, m30, m32, m33);
    float t02 =  determinant3x3(m10, m11, m13, m20, m21, m23, m30, m31, m33);
    float t03 = -determinant3x3(m10, m11, m12, m20, m21, m22, m30, m31, m32);

    float t10 = -determinant3x3(m01, m02, m03, m21, m22, m23, m31, m32, m33);
    float t11 =  determinant3x3(m00, m02, m03, m20, m22, m23, m30, m32, m33);
    float t12 = -determinant3x3(m00, m01, m03, m20, m21, m23, m30, m31, m33);
    float t13 =  determinant3x3(m00, m01, m02, m20, m21, m22, m30, m31, m32);

    float t20 =  determinant3x3(m01, m02, m03, m11, m12, m13, m31, m32, m33);
    float t21 = -determinant3x3(m00, m02, m03, m10, m12, m13, m30, m32, m33);
    float t22 =  determinant3x3(m00, m01, m03, m10, m11, m13, m30, m31, m33);
    float t23 = -determinant3x3(m00, m01, m02, m10, m11, m12, m30, m31, m32);

    float t30 = -determinant3x3(m01, m02, m03, m11, m12, m13, m21, m22, m23);
    float t31 =  determinant3x3(m00, m02, m03, m10, m12, m13, m20, m22, m23);
    float t32 = -determinant3x3(m00, m01, m03, m10, m11, m13, m20, m21, m23);
    float t33 =  determinant3x3(m00, m01, m02, m10, m11, m12, m20, m21, m22);

    m00 = t00 * determinant_inv;
    m11 = t11 * determinant_inv;
    m22 = t22 * determinant_inv;
    m33 = t33 * determinant_inv;
    m01 = t10 * determinant_inv;
    m10 = t01 * determinant_inv;
    m20 = t02 * determinant_inv;
    m02 = t20 * determinant_inv;
    m12 = t21 * determinant_inv;
    m21 = t12 * determinant_inv;
    m03 = t30 * determinant_inv;
    m30 = t03 * determinant_inv;
    m13 = t31 * determinant_inv;
    m31 = t13 * determinant_inv;
    m32 = t23 * determinant_inv;
    m23 = t32 * determinant_inv;
    updateBuffer();
    return *this;
}

Matrix4f& Matrix4f::removeTranslation(){
    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    return *this;
}

float * Matrix4f::getBuffer() const {
    return buffer;
}

void Matrix4f::updateBuffer() {
    if(buffer != nullptr)
        delete[] buffer;
    buffer = new float[16] {m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33};
}

std::string Matrix4f::toString(){
    std::stringstream ss;
    ss << m00 << " : " << m10 << " : " << m20 << " : " << m30 << "\n"
       << m01 << " : " << m11 << " : " << m21 << " : " << m31 << "\n"
       << m02 << " : " << m12 << " : " << m22 << " : " << m32 << "\n"
       << m03 << " : " << m13 << " : " << m23 << " : " << m33 << "";
    return ss.str();
}