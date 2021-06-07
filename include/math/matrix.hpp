#pragma once

#include <cmath>
#include <iostream>
#include "utility.hpp"
#include "vector.hpp"

/* matrix definition */

#pragma region mat

/* 
    所有函数均返回class T
    在使用mat<int>的inverse等时会发生问题
*/

template <class T>
class mat2
{
    /* 
        d[0] d[1]
        d[2] d[3]
    */
private:
    T d[4];

public:
    mat2() { d[0] = d[3] = 1; d[1] = d[2] = 0; }
    mat2(const vec2<T>& _a, const vec2<T>& _b) { d[0] = _a.x, d[2] = _a.y, d[1] = _b.x, d[3] = _b.y; } // col vec
    mat2(T _a, T _b, T _c, T _d) { d[0] = _a, d[1] = _b, d[2] = _c, d[3] = _d; }
    mat2(T* _a) { d[0] = _a[0], d[1] = _a[1], d[2] = _a[2], d[3] = _a[3]; }

    /* unary operations */
    vec2<T> row(int r) const;
    vec2<T> col(int c) const;
    T determinant() const;
    mat2<T> inverse() const;
    mat2<T> transpose() const;
    mat2<T> operator-() const;
    T* operator[](int row);


    /* binary opeartions */
    mat2<T> operator+(const mat2<T>& _a) const;
    mat2<T> operator-(const mat2<T>& _a) const;
    mat2<T> operator*(T _a) const;
    mat2<T> operator/(T _a) const;
    vec2<T> operator*(const vec2<T>& _a) const;
    mat2<T> operator*(const mat2<T>& _a) const;

    /* transform */
    mat2<T> rotate(double _d) const;
};

template <class T>
class mat3
{
    /* 
        d[0] d[1] d[2]
        d[3] d[4] d[5]
        d[6] d[7] d[8]
    */
private:
    T d[9];

public:
    mat3() { d[0] = d[4] = d[8] = 1; d[1] = d[2] = d[3] = d[5] = d[6] = d[7] = 0; }
    mat3(const vec3<T>& _a, const vec3<T>& _b, const vec3<T>& _c); // col vec
    mat3(T* _a);
    mat3(T a0, T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8);

    /* unary operations */
    vec3<T> row(int r) const;
    vec3<T> col(int c) const;
    T determinant() const;
    mat3<T> inverse() const;
    mat3<T> transpose() const;
    mat3<T> operator-() const;
    T* operator[](int row);


    /* binary opeartions */
    mat3<T> operator+(const mat3<T>& _a) const;
    mat3<T> operator-(const mat3<T>& _a) const;
    mat3<T> operator*(T _a) const;
    mat3<T> operator/(T _a) const;
    vec3<T> operator*(const vec3<T>& _a) const;
    mat3<T> operator*(const mat3<T>& _a) const;

    /* transform */
    mat3<T> rotate_y(double _d) const;
    mat3<T> rotate_x(double _d) const;
    mat3<T> rotate_z(double _d) const;
};

template <class T>
class mat4
{
    /* 
        d[0] d[1] d[2] d[3]
        d[4] d[5] d[6] d[7]
        d[8] d[9] d[10] d[11]
        d[12] d[13] d[14] d[15]
    */
private:
    T d[16];

public:
    mat4();
    mat4(const vec4<T>& _a, const vec4<T>& _b, const vec4<T>& _c, const vec4<T> _d); // col vec
    mat4(T* _a);
    mat4(T a0, T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9, T a10, T a11, T a12, T a13, T a14, T a15);

    /* unary operations */
    vec4<T> row(int r) const;
    vec4<T> col(int c) const;
    T determinant() const;
    mat4<T> inverse() const;
    mat4<T> transpose() const;
    mat4<T> operator-() const;
    T* operator[](int row);


    /* binary opeartions */
    mat4<T> operator+(const mat4<T>& _a) const;
    mat4<T> operator-(const mat4<T>& _a) const;
    mat4<T> operator*(T _a) const;
    mat4<T> operator/(T _a) const;
    vec4<T> operator*(const vec4<T>& _a) const;
    mat4<T> operator*(const mat4<T>& _a) const;

    /* transform */
    mat4<T> translate(const vec3<T>& offset) const;
    mat4<T> rotate_y(double _d) const;
    mat4<T> rotate_x(double _d) const;
    mat4<T> rotate_z(double _d) const;
};

#pragma endregion mat

#pragma region utils

template <class T>
std::ostream& operator<<(std::ostream& os, mat2<T> _a);

template <class T>
std::ostream& operator<<(std::ostream& os, mat3<T> _a);

template <class T>
std::ostream& operator<<(std::ostream& os, mat4<T> _a);

#pragma endregion utils


#include "matrix.inl"