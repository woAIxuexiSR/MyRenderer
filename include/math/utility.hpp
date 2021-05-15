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