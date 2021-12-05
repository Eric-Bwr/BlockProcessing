#pragma once

#include "Matrix.h"

template<typename T = float>
class Quat {
public:
    T x = 0, y = 0, z = 0, w = 0;

    Quat() = default;
    explicit Quat<T>(T v){
        this->z = v;
        this->x = v;
        this->y = v;
        this->w = v;
    }
    Quat(T x, T y, T z, T w){
        this->z = z;
        this->x = x;
        this->y = y;
        this->w = w;
    }
    explicit Quat(const Mat3<T>& m){
        T s;
        T tr = m.m00 + m.m11 + m.m22;
        if(tr >= 0.0){
            s = std::sqrt(tr + 1.0);
            w = s * 0.5f;
            s = 0.5f / s;
            x = (m.m21 - m.m12) * s;
            y = (m.m02 - m.m20) * s;
            z = (m.m10 - m.m01) * s;
        }else{
            T max = std::max(std::max(m.m00, m.m11), m.m22);
            if (max == m.m00) {
                s = std::sqrt(m.m00 - (m.m11 + m.m22) + 1.0);
                x = s * 0.5f;
                s = 0.5f / s;
                y = (m.m01 + m.m10) * s;
                z = (m.m20 + m.m02) * s;
                w = (m.m21 - m.m12) * s;
            } else if (max == m.m11) {
                s = std::sqrt(m.m11 - (m.m22 + m.m00) + 1.0);
                y = s * 0.5f;
                s = 0.5f / s;
                z = (m.m12 + m.m21) * s;
                x = (m.m01 + m.m10) * s;
                w = (m.m02 - m.m20) * s;
            } else {
                s = std::sqrt(m.m22 - (m.m00 + m.m11) + 1.0);
                z = s * 0.5f;
                s = 0.5f / s;
                x = (m.m20 + m.m02) * s;
                y = (m.m12 + m.m21) * s;
                w = (m.m10 - m.m01) * s;
            }
        }
    }
    explicit Quat(const Mat4<T>& m){
        Quat(m.toMat3());
    }

    Quat<T> identity(){
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
        this->w = 1.0;
        return *this;
    }
    Quat<T> norm() {
        T len2 = (x * x + y * y + z * z + w * w);
        if(len2 == 0)
            return *this;
        T len = 1.0 / std::sqrt(len2);
        return Quat<T>(x * len, y * len, z * len, w * len);
    }
    Quat<T> dot(Quat<T> const &a) const {
        return x * a.x + y * a.y + z * a.z + w * a.w;
    }
    T len() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    T len2() const {
        return x * x + y * y + z * z + w * w;
    }
    Quat<T> negate() const {
        return Quat<T>(-x, -y, -z, w);
    }
    Quat<T> scale(T scale) const {
        return Quat<T>(x * scale, y * scale, z * scale, w * scale);
    }
    Quat<T> convertAxisAngles() const{
        T len = std::sqrt(x * x + y * y + z * z);
        if(len == 0.0)
            return *this;
        T halfW = 0.5 * w;
        T s = std::sin(halfW) / len;
        x *= s;
        y *= s;
        w *= s;
        w = std::cos(halfW);
    }
    Quat<T> lerp(const Quat<T> &a, T blend){
        Quat<T> result;
        T dot = x * a.x + y * a.y + z * a.z + w * a.w;
        T oneMinusBlend = 1.0 - blend;
        if(dot < 0){
            result.x = oneMinusBlend * x + blend * -a.x;
            result.y = oneMinusBlend * y + blend * -a.y;
            result.z = oneMinusBlend * z + blend * -a.z;
            result.w = oneMinusBlend * w + blend * -a.w;
        }else{
            result.x = oneMinusBlend * x + blend * a.x;
            result.y = oneMinusBlend * y + blend * a.y;
            result.z = oneMinusBlend * z + blend * a.z;
            result.w = oneMinusBlend * w + blend * a.w;
        }
        return result;
    }
    Quat<float> slerp(Quat<float> const &a, float blend){
        Quat<float> result = a;
        float dot = x * a.x + y * a.y + z * a.z + w * a.w;
        float oneMinusBlend = 1.0f - blend;
        if(dot < 0.0f){
            result = a.negate();
            dot = -dot;
        }
        if(dot > 1.0f - std::numeric_limits<float>::epsilon()){
           result.x = x * oneMinusBlend + result.x * blend;
           result.y = y * oneMinusBlend + result.y * blend;
           result.z = z * oneMinusBlend + result.z * blend;
           result.w = w * oneMinusBlend + result.w * blend;
        }else{
            float angle = std::acos(dot);
            float angleSin = std::sin(angle);
            float angleOneMinus = std::sin(oneMinusBlend) * angle;
            float angleBlend = std::sin(blend * angle);
            result.x = (angleOneMinus * x + angleBlend * result.x) / angleSin;
            result.y = (angleOneMinus * y + angleBlend * result.y) / angleSin;
            result.z = (angleOneMinus * z + angleBlend * result.z) / angleSin;
            result.w = (angleOneMinus * w + angleBlend * result.w) / angleSin;
        }
        return result;
    }
    Mat4<T> toMat4(){
        Mat4<T> result;
        T xy = x * y;
        T xz = x * z;
        T xw = x * w;
        T yz = y * z;
        T yw = y * w;
        T zw = z * w;
        T xSquared = x * x;
        T ySquared = y * y;
        T zSquared = z * z;
        result.m00 = 1 - 2 * (ySquared + zSquared);
        result.m01 = 2 * (xy + zw);
        result.m02 = 2 * (xz - yw);
        result.m03 = 0;
        result.m10 = 2 * (xy - zw);
        result.m11 = 1 - 2 * (xSquared + zSquared);
        result.m12 = 2 * (yz + xw);
        result.m13 = 0;
        result.m20 = 2 * (xz + yw);
        result.m21 = 2 * (yz - xw);
        result.m22 = 1 - 2 * (xSquared + ySquared);
        result.m23 = 0;
        result.m30 = 0;
        result.m31 = 0;
        result.m32 = 0;
        result.m33 = 1;
        return result;
    }

    friend Quat<T> operator* (const Quat<T> &a, const Quat<T> &b){
        return Quat<T>(a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y,
                       a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z,
                       a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x,
                       a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
    }

    bool operator== (Quat<T> &a) const{
        return x == a.x && y == a.y && z == a.z && w == a.w;
    }
    bool operator!= (Quat<T> & a) const{
        return x != a.x || y != a.y || z != a.z || w != a.w;
    }

    char *toString() {
        const char *mt = "%+6.2f %+6.2f %+6.2f %+6.2f\n";
        char *s = (char *) malloc(sizeof(char) * 32);
        std::sprintf(s, mt, x, y, z, w);
        return s;
    }
};

typedef Quat<float> Quatf;
typedef Quat<double> Quatd;