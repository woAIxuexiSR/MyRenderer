#include "matrix.hpp"

namespace srm
{
    /* matrix definition */
#pragma region mat

    // mat2
    template <class T>
    vec2<T> mat2<T>::row(int r) const
    {
        return vec2<T>(d[2 * r], d[2 * r + 1]);
    }

    template <class T>
    vec2<T> mat2<T>::col(int c) const
    {
        return vec2<T>(d[c], d[c + 2]);
    }

    template <class T>
    T mat2<T>::determinant() const
    {
        return d[0] * d[3] - d[1] * d[2];
    }

    template <class T>
    mat2<T> mat2<T>::inverse() const
    {
        T det = determinant();
        if(abs((double)det) < EPS)
        {
            std::cout << "mat2 doesn't have an inverse" << std::endl;
            exit(-1);
        }
        return mat2<T>(d[3], -d[1], -d[2], d[0]) / det;
    }

    template <class T>
    mat2<T> mat2<T>::operator-() const
    {
        return mat2<T>(-d[0], -d[1], -d[2], -d[3]);
    }

    template <class T>
    T* mat2<T>::operator[](int row)
    {
        return d + row * 2;
    }

    template <class T>
    mat2<T> mat2<T>::operator+(const mat2<T>& _a) const
    {
        return mat2<T>(d[0] + _a.d[0], d[1] + _a.d[1], d[2] + _a.d[2], d[3] + _a.d[3]);
    }

    template <class T>
    mat2<T> mat2<T>::operator-(const mat2<T>& _a) const
    {
        return mat2<T>(d[0] - _a.d[0], d[1] - _a.d[1], d[2] - _a.d[2], d[3] - _a.d[3]);
    }

    template <class T>
    mat2<T> mat2<T>::operator*(T _a) const
    {
        return mat2<T>(d[0] * _a, d[1] * _a, d[2] * _a, d[3] * _a);
    }

    template <class T>
    mat2<T> mat2<T>::operator/(T _a) const
    {
        return mat2<T>(d[0] / _a, d[1] / _a, d[2] / _a, d[3] / _a);
    }

    template <class T>
    vec2<T> mat2<T>::operator*(const vec2<T>& _a) const
    {
        return vec2<T>(d[0] * _a.x + d[1] * _a.y, d[2] * _a.x + d[3] * _a.y);
    }

    template <class T>
    mat2<T> mat2<T>::operator*(const mat2<T>& _a) const
    {
        return mat2<T>(d[0] * _a.d[0] + d[1] * _a.d[2], d[0] * _a.d[1] + d[1] * _a.d[3],
                    d[2] * _a.d[0] + d[3] * _a.d[2], d[2] * _a.d[1] + d[3] * _a.d[3]);
    }

    // mat3
    template <class T>
    mat3<T>::mat3(const vec3<T>& _a, const vec3<T>& _b, const vec3<T>& _c)
    {
        d[0] = _a.x, d[1] = _b.x, d[2] = _c.x;
        d[3] = _a.y, d[4] = _b.y, d[5] = _c.y;
        d[6] = _a.z, d[7] = _b.z, d[8] = _c.z;
    }

    template <class T>
    mat3<T>::mat3(T* _a)
    {
        d[0] = _a[0], d[1] = _a[1], d[2] = _a[2];
        d[3] = _a[3], d[4] = _a[4], d[5] = _a[5];
        d[6] = _a[6], d[7] = _a[7], d[8] = _a[8];
    }

    template <class T>
    mat3<T>::mat3(T a0, T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8)
    {
        d[0] = a0, d[1] = a1, d[2] = a2;
        d[3] = a3, d[4] = a4, d[5] = a5;
        d[6] = a6, d[7] = a7, d[8] = a8;
    }

    template <class T>
    vec3<T> mat3<T>::row(int r) const
    {
        return vec3<T>(d[3 * r], d[3 * r + 1], d[3 * r + 2]);
    }

    template <class T>
    vec3<T> mat3<T>::col(int c) const
    {
        return vec3<T>(d[c], d[c + 3], d[c + 6]);
    }

    template <class T>
    T mat3<T>::determinant() const
    {
        return d[0] * (d[4] * d[8] - d[5] * d[7]) - d[1] * (d[3] * d[8] - d[5] * d[6]) + d[2] * (d[3] * d[7] - d[4] * d[6]);
    }

    template <class T>
    mat3<T> mat3<T>::inverse() const
    {
        T det = determinant();
        if(fabs((double)det) < EPS)
        {
            std::cout << "mat3 doesn't have an inverse" << std::endl;
            exit(-1);
        }
        return mat3<T>(d[4] * d[8] - d[5] * d[7], -(d[1] * d[8] - d[2] * d[7]), d[1] * d[5] - d[2] * d[4],
                    -(d[3] * d[8] - d[5] * d[6]), d[0] * d[8] - d[2] * d[6], -(d[0] * d[5] - d[2] * d[3]),
                    d[3] * d[7] - d[4] * d[6], -(d[0] * d[7] - d[1] * d[6]), d[0] * d[4] - d[1] * d[3]) / det;
    }

    template <class T>
    mat3<T> mat3<T>::operator-() const
    {
        return mat3<T>(-d[0], -d[1], -d[2], -d[3], -d[4], -d[5], -d[6], -d[7], -d[8]);
    }

    template <class T>
    T* mat3<T>::operator[](int row)
    {
        return d + row * 3;
    }

    template <class T>
    mat3<T> mat3<T>::operator+(const mat3<T>& _a) const
    {
        return mat3<T>(d[0] + _a.d[0], d[1] + _a.d[1], d[2] + _a.d[2], 
                    d[3] + _a.d[3], d[4] + _a.d[4], d[5] + _a.d[5],
                    d[6] + _a.d[6], d[7] + _a.d[7], d[8] + _a.d[8]);
    }

    template <class T>
    mat3<T> mat3<T>::operator-(const mat3<T>& _a) const
    {
        return mat3<T>(d[0] - _a.d[0], d[1] - _a.d[1], d[2] - _a.d[2], 
                    d[3] - _a.d[3], d[4] - _a.d[4], d[5] - _a.d[5],
                    d[6] - _a.d[6], d[7] - _a.d[7], d[8] - _a.d[8]);
    }

    template <class T>
    mat3<T> mat3<T>::operator*(T _a) const
    {
        return mat3<T>(d[0] * _a, d[1] * _a, d[2] * _a, 
                    d[3] * _a, d[4] * _a, d[5] * _a,
                    d[6] * _a, d[7] * _a, d[8] * _a);
    }

    template <class T>
    mat3<T> mat3<T>::operator/(T _a) const
    {
        return mat3<T>(d[0] / _a, d[1] / _a, d[2] / _a, 
                    d[3] / _a, d[4] / _a, d[5] / _a,
                    d[6] / _a, d[7] / _a, d[8] / _a);
    }

    template <class T>
    vec3<T> mat3<T>::operator*(const vec3<T>& _a) const
    {
        return vec3<T>(d[0] * _a.x + d[1] * _a.y + d[2] * _a.z, 
                    d[3] * _a.x + d[4] * _a.y + d[5] * _a.z,
                    d[6] * _a.x + d[7] * _a.y + d[8] * _a.z);
    }

    template <class T>
    mat3<T> mat3<T>::operator*(const mat3<T>& _a) const
    {
        return mat3<T>(dot(row(0), _a.col(0)), dot(row(0), _a.col(1)), dot(row(0), _a.col(2)),
                    dot(row(1), _a.col(0)), dot(row(1), _a.col(1)), dot(row(1), _a.col(2)),
                    dot(row(2), _a.col(0)), dot(row(2), _a.col(1)), dot(row(2), _a.col(2)));
    }

    // mat4
    template <class T>
    mat4<T>::mat4()
    {
        d[0] = d[5] = d[10] = d[15] = 1;
        d[1] = d[2] = d[3] = d[4] = d[6] = d[7] = 
        d[8] = d[9] = d[11] = d[12] = d[13] = d[14] = 0;
    }

    template <class T>
    mat4<T>::mat4(const vec4<T>& _a, const vec4<T>& _b, const vec4<T>& _c, const vec4<T> _d)
    {
        d[0] = _a.x; d[1] = _b.x; d[2] = _c.x; d[3] = _d.x;
        d[4] = _a.y; d[5] = _b.y; d[6] = _c.y; d[7] = _d.y;
        d[8] = _a.z; d[9] = _b.z; d[10] = _c.z; d[11] = _d.z;
        d[12] = _a.w; d[13] = _b.w; d[14] = _c.w; d[15] = _d.w;
    }

    template <class T>
    mat4<T>::mat4(T* _a)
    {
        d[0] = _a[0]; d[1] = _a[1]; d[2] = _a[2]; d[3] = _a[3];
        d[4] = _a[4]; d[5] = _a[5]; d[6] = _a[6]; d[7] = _a[7];
        d[8] = _a[8]; d[9] = _a[9]; d[10] = _a[10]; d[11] = _a[11];
        d[12] = _a[12]; d[13] = _a[13]; d[14] = _a[14]; d[15] = _a[15];
    }

    template <class T>
    mat4<T>::mat4(T a0, T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9, T a10, T a11, T a12, T a13, T a14, T a15)
    {
        d[0] = a0; d[1] = a1; d[2] = a2; d[3] = a3;
        d[4] = a4; d[5] = a5; d[6] = a6; d[7] = a7;
        d[8] = a8; d[9] = a9; d[10] = a10; d[11] = a11;
        d[12] = a12; d[13] = a13; d[14] = a14; d[15] = a15;
    }

    template <class T>
    vec4<T> mat4<T>::row(int r) const
    {
        return vec4<T>(d[4 * r], d[4 * r + 1], d[4 * r + 2], d[4 * r + 3]);
    }

    template <class T>
    vec4<T> mat4<T>::col(int c) const
    {
        return vec4<T>(d[c], d[c + 4], d[c + 8], d[c + 12]);
    }

    template <class T>
    T mat4<T>::determinant() const
    {
        return d[0] * ( d[5] * (d[10] * d[15] - d[11] * d[14]) - d[6] * (d[9] * d[15] - d[11] * d[13]) + d[7] * (d[9] * d[14] - d[10] * d[13]) )
            - d[1] * ( d[4] * (d[10] * d[15] - d[11] * d[14]) - d[6] * (d[8] * d[15] - d[11] * d[12]) + d[7] * (d[8] * d[14] - d[10] * d[12]) )
            + d[2] * ( d[4] * (d[9] * d[15] - d[11] * d[13]) - d[5] * (d[8] * d[15] - d[11] * d[12]) + d[7] * (d[8] * d[13] - d[9] * d[12]) )
            - d[3] * ( d[4] * (d[9] * d[14] - d[10] * d[13]) - d[5] * (d[8] * d[14] - d[10] * d[12]) + d[6] * (d[8] * d[13] - d[9] * d[12]) );
    }

    template <class T>
    mat4<T> mat4<T>::inverse() const
    {
        T det = determinant();
        if(abs((double)det) < EPS)
        {
            std::cout << "mat4 doesn't have an inverse" << std::endl;
            exit(-1);
        }
        return mat4<T>( d[5] * (d[10] * d[15] - d[11] * d[14]) - d[6] * (d[9] * d[15] - d[11] * d[13]) + d[7] * (d[9] * d[14] - d[10] * d[13]),
                    -( d[1] * (d[10] * d[15] - d[11] * d[14]) - d[2] * (d[9] * d[15] - d[11] * d[13]) + d[3] * (d[9] * d[14] - d[10] * d[13]) ),
                    d[1] * (d[6] * d[15] - d[7] * d[14]) - d[2] * (d[5] * d[15] - d[7] * d[13]) + d[3] * (d[5] * d[14] - d[6] * d[13]),
                    -( d[1] * (d[6] * d[11] - d[7] * d[10]) - d[2] * (d[5] * d[11] - d[7] * d[9]) + d[3] * (d[5] * d[10] - d[6] * d[9]) ),
                    -( d[4] * (d[10] * d[15] - d[11] * d[14]) - d[6] * (d[8] * d[15] - d[11] * d[12]) + d[7] * (d[8] * d[14] - d[10] * d[12]) ),
                    d[0] * (d[10] * d[15] - d[11] * d[14]) - d[2] * (d[8] * d[15] - d[11] * d[12]) + d[3] * (d[8] * d[14] - d[10] * d[12]),
                    -( d[0] * (d[6] * d[15] - d[7] * d[14]) - d[2] * (d[4] * d[15] - d[7] * d[12]) + d[3] * (d[4] * d[14] - d[6] * d[12]) ),
                    d[0] * (d[6] * d[11] - d[7] * d[10]) - d[2] * (d[4] * d[11] - d[7] * d[8]) + d[3] * (d[4] * d[10] - d[6] * d[8]),
                    d[4] * (d[9] * d[15] - d[11] * d[13]) - d[5] * (d[8] * d[15] - d[11] * d[12]) + d[7] * (d[8] * d[13] - d[9] * d[12]),
                    -( d[0] * (d[9] * d[15] - d[11] * d[13]) - d[1] * (d[8] * d[15] - d[11] * d[12]) + d[3] * (d[8] * d[13] - d[9] * d[12]) ),
                    d[0] * (d[5] * d[15] - d[7] * d[13]) - d[1] * (d[4] * d[15] - d[7] * d[12]) + d[3] * (d[4] * d[13] - d[5] * d[12]),
                    -( d[0] * (d[5] * d[11] - d[7] * d[9]) - d[1] * (d[4] * d[11] - d[7] * d[8]) + d[3] * (d[4] * d[9] - d[5] * d[8]) ),
                    -( d[4] * (d[9] * d[14] - d[10] * d[13]) - d[5] * (d[8] * d[14] - d[10] * d[12]) + d[6] * (d[8] * d[13] - d[9] * d[12]) ),
                    d[0] * (d[9] * d[14] - d[10] * d[13]) - d[1] * (d[8] * d[14] - d[10] * d[12]) + d[2] * (d[8] * d[13] - d[9] * d[12]),
                    -( d[0] * (d[5] * d[14] - d[6] * d[13]) - d[1] * (d[4] * d[14] - d[6] * d[12]) + d[2] * (d[4] * d[13] - d[5] * d[12]) ),
                    d[0] * (d[5] * d[10] - d[6] * d[9]) - d[1] * (d[4] * d[10] - d[6] * d[8]) + d[2] * (d[4] * d[9] - d[5] * d[8])
                    ) / det;
    }

    template <class T>
    mat4<T> mat4<T>::operator-() const
    {
        return mat4<T>(-d[0], -d[1], -d[2], -d[3], -d[4], -d[5], -d[6], -d[7],
                    -d[8], -d[9], -d[10], -d[11], -d[12], -d[13], -d[14], -d[15]);
    }

    template <class T>
    T* mat4<T>::operator[](int row)
    {
        return d + row * 4;
    }

    template <class T>
    mat4<T> mat4<T>::operator+(const mat4<T>& _a) const
    {
        return mat4<T>(d[0] + _a.d[0], d[1] + _a.d[1], d[2] + _a.d[2], d[3] + _a.d[3],
                    d[4] + _a.d[4], d[5] + _a.d[5], d[6] + _a.d[6], d[7] + _a.d[7],
                    d[8] + _a.d[8], d[9] + _a.d[9], d[10] + _a.d[10], d[11] + _a.d[11],
                    d[12] + _a.d[12], d[13] + _a.d[13], d[14] + _a.d[14], d[15] + _a.d[15]);
    }

    template <class T>
    mat4<T> mat4<T>::operator-(const mat4<T>& _a) const
    {
        return mat4<T>(d[0] - _a.d[0], d[1] - _a.d[1], d[2] - _a.d[2], d[3] - _a.d[3],
                    d[4] - _a.d[4], d[5] - _a.d[5], d[6] - _a.d[6], d[7] - _a.d[7],
                    d[8] - _a.d[8], d[9] - _a.d[9], d[10] - _a.d[10], d[11] - _a.d[11],
                    d[12] - _a.d[12], d[13] - _a.d[13], d[14] - _a.d[14], d[15] - _a.d[15]);
    }

    template <class T>
    mat4<T> mat4<T>::operator*(T _a) const
    {
        return mat4<T>(d[0] * _a, d[1] * _a, d[2] * _a, d[3] * _a,
                    d[4] * _a, d[5] * _a, d[6] * _a, d[7] * _a,
                    d[8] * _a, d[9] * _a, d[10] * _a, d[11] * _a,
                    d[12] * _a, d[13] * _a, d[14] * _a, d[15] * _a);
    }

    template <class T>
    mat4<T> mat4<T>::operator/(T _a) const
    {
        return mat4<T>(d[0] / _a, d[1] / _a, d[2] / _a, d[3] / _a,
                    d[4] / _a, d[5] / _a, d[6] / _a, d[7] / _a,
                    d[8] / _a, d[9] / _a, d[10] / _a, d[11] / _a,
                    d[12] / _a, d[13] / _a, d[14] / _a, d[15] / _a);
    }

    template <class T>
    vec4<T> mat4<T>::operator*(const vec4<T>& _a) const
    {
        return vec4<T>(d[0] * _a.x + d[1] * _a.y + d[2] * _a.z + d[3] * _a.w,
                    d[4] * _a.x + d[5] * _a.y + d[6] * _a.z + d[7] * _a.w,
                    d[8] * _a.x + d[9] * _a.y + d[10] * _a.z + d[11] * _a.w,
                    d[12] * _a.x + d[13] * _a.y + d[14] * _a.z + d[15] * _a.w);
    }

    template <class T>
    mat4<T> mat4<T>::operator*(const mat4<T>& _a) const
    {
        return mat4<T>(dot(row(0), _a.col(0)), dot(row(0), _a.col(1)), dot(row(0), _a.col(2)), dot(row(0), _a.col(3)),
                    dot(row(1), _a.col(0)), dot(row(1), _a.col(1)), dot(row(1), _a.col(2)), dot(row(1), _a.col(3)),
                    dot(row(2), _a.col(0)), dot(row(2), _a.col(1)), dot(row(2), _a.col(2)), dot(row(2), _a.col(3)),
                    dot(row(3), _a.col(0)), dot(row(3), _a.col(1)), dot(row(3), _a.col(2)), dot(row(3), _a.col(3)) );
    }

#pragma endregion mat

#pragma region utils

    template <class T>
    std::ostream& operator<<(std::ostream& os, mat2<T> _a)
    {
        os << "mat2(" << _a[0][0] << "  " << _a[0][1] << std::endl <<
            "     " << _a[1][0] << "  " << _a[1][1] << ")" << std::endl;
        return os;
    }

    template <class T>
    std::ostream& operator<<(std::ostream& os, mat3<T> _a)
    {
        os << "mat3(" << _a[0][0] << "  " << _a[0][1] << "  " << _a[0][2] << std::endl <<
            "     " << _a[1][0] << "  " << _a[1][1] << "  " << _a[1][2] << std::endl <<
            "     " << _a[2][0] << "  " << _a[2][1] << "  " << _a[2][2] << ")" << std::endl;
        return os;
    }

    template <class T>
    std::ostream& operator<<(std::ostream& os, mat4<T> _a)
    {
        os << "mat4(" << _a[0][0] << "  " << _a[0][1] << "  " << _a[0][2] << "  " << _a[0][3] << std::endl <<
            "     " << _a[1][0] << "  " << _a[1][1] << "  " << _a[1][2] << "  " << _a[1][3] << std::endl <<
            "     " << _a[2][0] << "  " << _a[2][1] << "  " << _a[2][2] << "  " << _a[2][3] << std::endl <<
            "     " << _a[3][0] << "  " << _a[3][1] << "  " << _a[3][2] << "  " << _a[3][3] << ")" << std::endl;
        return os;
    }

#pragma endregion utils

}