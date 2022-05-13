#pragma once
#include "directxmath/include/DirectXMath.h"

template <typename T>
struct Vector2 {
    T x;
    T y;

    Vector2(T vx, T vy)
        : x(vx), y(vy)
    {}

    Vector2()
        : x(0), y(0)
    {}

    inline Vector2<T>& Inverse()
    {
        x = (T)0 - x;
        y = (T)0 - y;
        return Vector2<T>(x, y);
    }

    static inline Vector2<T> Inverse(const Vector2<T>& vec)
    {
        return vec.Inverse();
    }
};

typedef Vector2<unsigned int> ScreenResolution;
typedef Vector2<float> Vector2f;

template <typename T>
struct Vector3 {
    T x;
    T y;
    T z;

    Vector3(T vx, T vy, T vz)
        : x(vx), y(vy), z(vz)
    {}

    Vector3()
        : x(0), y(0), z(0)
    {}

    const float Magnitude() const
    {
        return sqrtf( (x * x) + (y * y) + (z * z) );
    }

    inline const float Magnitude(const Vector3<T>& rvec) const
    {
        return ( ((x - rvec.x) * (x - rvec.x)) + ((y - rvec.y) * (y - rvec.y)) + ((z - rvec.z) * (z - rvec.z)) );
    }

    inline Vector3<T>& Inverse()
    {
        x = (T)0 - x;
        y = (T)0 - y;
        z = (T)0 - z;
        return Vector3<T>(x, y, z);
    }

    static inline Vector3<T> Inverse(const Vector3<T>& vec)
    {
        return vec.Inverse();
    }
};

typedef Vector3<float> Vector3f;

template <typename T>
struct Vector4 {
    T x;
    T y;
    T z;
    T a;

    Vector4(T vx, T vy, T vz, T va)
        : x(vx), y(vy), z(vz), a(va)
    {}

    Vector4()
        : x(0), y(0), z(0), a(0)
    {}

    explicit operator Vector3<T>() const
    {
        return { x, y, z };
    }

    Vector4(const Vector3<T>& rhs)
        : x(rhs.x), y(rhs.y), z(rhs.z), a(0)
    {}

    const bool operator==(const Vector4<T>& rhs) const
    {
        return
            (x == rhs.x) &&
            (y == rhs.y) &&
            (z == rhs.z) &&
            (a == rhs.a);
    }

    const bool operator==(Vector4<T>& rhs) const
    {
        return
            (x == rhs.x) &&
            (y == rhs.y) &&
            (z == rhs.z) &&
            (a == rhs.a);
    }

    const float Magnitude() const
    {
        return sqrtf((x * x) + (y * y) + (z * z) + (a * a));
    }

    inline const float Magnitude(const Vector4<T>& rvec) const
    {
        return ( ((x - rvec.x) * (x - rvec.x)) + ((y - rvec.y) * (y - rvec.y)) + ((z - rvec.z) * (z - rvec.z)) + ((a - rvec.a) * (a - rvec.a)) );
    }

    inline Vector4<T>& Inverse()
    {
        x = (T)0 - x;
        y = (T)0 - y;
        z = (T)0 - z;
        a = (T)0 - a;
        return Vector4<T>(x, y, z, a);
    }

    static inline Vector4<T> Inverse(const Vector4<T>& vec)
    {
        return vec.Inverse();
    }
};

struct ColorRGB {
    float r;
    float g;
    float b;
    float a;

    ColorRGB(float vr, float vg, float vb, float va)
        : r(vr), g(vg), b(vb), a(va)
    {}

    ColorRGB(float vr, float vg, float vb)
        : r(vr), g(vg), b(vb), a(0)
    {}

    ColorRGB()
        : r(0), g(0), b(0), a(0)
    {}
};

typedef Vector4<float> Vector4f;

struct Vector4fDouble
{
    Vector4f    Vec1;
    Vector4f    Vec2;
};

template<typename T>
struct Quaternion {
    T w;
    T x;
    T y;
    T z;

    Quaternion(T vw, T vx, T vy, T vz)
        : w(vw), x(vx), y(vy), z(vz)
    {}

    Quaternion(T vx, T vy, T vz)
        : w(0), x(vx), y(vy), z(vz)
    {}

    Quaternion()
        : w(0), x(0), y(0), z(0)
    {}

    explicit operator Vector4<T>() const
    {
        return { x, y, z, w };
    }

    Quaternion(const DirectX::XMVECTOR& rhs)
    {
        w = rhs.m128_f32[0];
        x = rhs.m128_f32[1];
        y = rhs.m128_f32[2];
        z = rhs.m128_f32[3];
    }

    inline Quaternion<T>& Inverse()
    {
        x = (T)0 - x;
        y = (T)0 - y;
        z = (T)0 - z;
        a = (T)0 - a;
        return Quaternion<T>(x, y, z, a);
    }

    static inline Quaternion<T> Inverse(const Quaternion<T>& vec)
    {
        return vec.Inverse();
    }
};

typedef Quaternion<float> Orientation;