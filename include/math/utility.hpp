#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <time.h>
#include "vector.hpp"

const double INF = std::numeric_limits<double>::infinity();
const double PI = std::acos(-1);
const double EPS = 1e-7;

enum class AXIS { AXIS_X, AXIS_Y, AXIS_Z };

inline AXIS next_axis(AXIS a) { return (AXIS) (((int)a + 1) % 3); }

inline double degree_to_radius(double degree)
{
    return degree / 180.0 * PI;
}

template <class T>
inline T myclamp(T x, T x_min, T x_max)
{
    if(x < x_min) return x_min;
    if(x > x_max) return x_max;
    return x;
}

// [0, 1)
inline double random_double()
{
    static std::uniform_real_distribution<double> dis(0.0, 1.0);
    static std::default_random_engine e(time(NULL));

    return dis(e);
}

// [min, max)
inline double random_double(double min, double max)
{
    return (max - min) * random_double() + min;
}

// [min, max]
inline int random_int(int min, int max)
{
    // can not declare static when using "uniform_int_distribution"
    double t = static_cast<int>(random_double(min, max + 1));
    return t;
}

// [0, 1)
inline vec3<double> random_v3()
{
    return vec3<double>(random_double(), random_double(), random_double());
}

// [min, max)
inline vec3<double> random_v3(double min, double max)
{
    return vec3<double>(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline vec3<double> random_sphere()
{
    vec3<double> p = random_v3(-1.0, 1.0);
    while(p.length_square() > 1)
        p = random_v3(-1.0, 1.0);
    return p;
}

inline vec3<double> random_hemisphere(const vec3<double>& up)
{
    vec3<double> p = random_sphere();
    if(p.dot(up) < 0) p = -p;
    return p;
}

inline vec3<double> random_sphere_surface()
{
    vec3<double> p = random_sphere();
    return p.normalize();
}

inline vec3<double> random_hemisphere_surface(const vec3<double>& up)
{
    vec3<double> p = random_sphere_surface();
    if(p.dot(up) < 0) p = -p;
    return p;
}

// Square [-1, 1] to Unit Disk r <= 1
inline vec2<double> square_to_disk(const vec2<double>& square)
{
    double a = square.x, b = square.y;
    double r, phi;

    if(a + b > 0)
    {
        if(a > b) r = a, phi = (PI / 4) * (b / a);
        else r = b, phi = (PI / 4) * (2 - a / b);
    }
    else
    {
        if(a < b) r = -a, phi = (PI / 4) * (4 + b / a);
        else r = -b, phi = (b == 0) ? 0 : (PI / 4) * (6 - a / b); 
    }

    return vec2<double>(r * cos(phi), r * sin(phi));
}

// Unit Disk r <= 1 to Square [-1, 1]
inline vec2<double> disk_to_square(const vec2<double>& disk)
{
    double r = disk.length();
    double phi = atan2(disk.y, disk.x);

    if(phi < -PI / 4) phi += 2 * PI;

    double a, b;
    if(phi < PI / 4)
        a = r, b = phi * a / (PI / 4);
    else if(phi < 3 * PI / 4)
        b = r, a = -(phi - PI / 2) * b / (PI / 4);
    else if(phi < 5 * PI / 4)
        a = -r, b = (phi - PI) * a / (PI / 4);
    else
        b = -r, a = -(phi - 3 * PI / 2) * b / (PI / 4);

    return vec2<double>(a, b);
}

// Unit Disk r <= 1 to Unit Hemisphere r <= 1, up = (0, 0, 1)
inline vec3<double> disk_to_hemisphere(const vec2<double>& disk)
{
    double r = disk.length();
    double z = 1 - r * r;
    double rr = sqrt(1 - z * z) / r;

    return vec3<double>(disk.x * rr, disk.y * rr, z);
}