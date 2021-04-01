#pragma once

#include <cmath>
#include <iostream>

namespace srm
{
    /* vector definition */

#pragma region vec
    
    // 除了length 返回 double, 其余所有函数均返回class T
    
    template <class T>
    class vec2
    {
    public:
        T x, y;

        vec2(): x(0), y(0) {}
        vec2(T _x): x(_x), y(_x) {}
        vec2(T _x, T _y): x(_x), y(_y) {}

        /* unary operations */
        double length() const;
        vec2<T> normalize() const;
        vec2<T> operator-() const;

        /* binary operations */
        T dot(const vec2<T>& _a) const;
        T cross(const vec2<T>& _a) const;
        vec2<T> operator+(const vec2<T>& _a) const;
        vec2<T> operator-(const vec2<T>& _a) const;
        vec2<T> operator*(T _a) const;
        vec2<T> operator/(T _a) const;
    };

    template <class T>
    class vec3
    {
    public:
        T x, y, z;

        vec3(): x(0), y(0), z(0) {}
        vec3(T _x): x(_x), y(_x), z(_x) {}
        vec3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {}

        /* unary operations */
        double length() const;
        vec3<T> normalize() const;
        vec3<T> operator-() const;

        /* binary operations */
        T dot(const vec3<T>& _a) const;
        vec3<T> cross(const vec3<T>& _a) const;
        vec3<T> operator+(const vec3<T>& _a) const;
        vec3<T> operator-(const vec3<T>& _a) const;
        vec3<T> operator*(T _a) const;
        vec3<T> operator/(T _a) const;
    };

    template <class T>
    class vec4
    {
    public:
        T x, y, z, w;

        vec4(): x(0), y(0), z(0), w(0) {}
        vec4(T _x): x(_x), y(_x), z(_x), w(_x) {}
        vec4(T _x, T _y, T _z, T _w): x(_x), y(_y), z(_z), w(_w) {}

        /* unary operations */
        double length() const;
        vec4<T> normalize() const;
        vec4<T> operator-() const;

        /* binary operations */
        T dot(const vec4<T>& _a) const;
        vec4<T> cross(const vec4<T>& _a, const vec4<T>& _b) const;
        vec4<T> operator+(const vec4<T>& _a) const;
        vec4<T> operator-(const vec4<T>& _a) const;
        vec4<T> operator*(T _a) const;
        vec4<T> operator/(T _a) const;
    };

#pragma endregion vec

    /* matrix definition */

#pragma region mat



#pragma endregion mat

    /* useful functions */

#pragma region utils

    template <class V>
    auto dot(const V& _a, const V& _b);

    template <class V>
    auto cross(const V& _a, const V& _b);

    template <class T>
    vec4<T> cross(const vec4<T>& _a, const vec4<T>& _b, const vec4<T>& _c);

    template <class T>
    std::ostream& operator<<(std::ostream& os, const vec2<T>& _a);

    template <class T>
    std::ostream& operator<<(std::ostream& os, const vec3<T>& _a);

    template <class T>
    std::ostream& operator<<(std::ostream& os, const vec4<T>& _a);

#pragma endregion utils

}

#include "vector.inl"