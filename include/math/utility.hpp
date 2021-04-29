#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <time.h>
#include "vector.hpp"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = std::acos(-1);

inline double degree_to_radius(double degree)
{
    return degree / 180.0 * pi;
}

template <class T>
inline T myclamp(T x, T x_min, T x_max)
{
    if(x < x_min) return x_min;
    if(x > x_max) return x_max;
    return x;
}

inline double random_double()
{
    static std::uniform_real_distribution<double> dis(0.0, 1.0);
    static std::default_random_engine e(time(NULL));

    return dis(e);
}

inline double random_double(double min, double max)
{
    return (max - min) * random_double() + min;
}

// [min, max]
inline int random_int(int min, int max)
{
    // can not declare static when using "uniform_int_distribution"
    double t = static_cast<int>(random_double(min, max + 1));
    while(t == max + 1) 
        t = static_cast<int>(random_double(min, max + 1));
    return t;
}

inline srm::vec3<double> random_v3()
{
    return srm::vec3<double>(random_double(), random_double(), random_double());
}

inline srm::vec3<double> random_v3(double min, double max)
{
    return srm::vec3<double>(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline srm::vec3<double> random_sphere()
{
    srm::vec3<double> p = random_v3(-1.0, 1.0);
    while(srm::dot(p, p) > 1)
        p = random_v3(-1.0, 1.0);
    return p;
}

inline srm::vec3<double> random_hemisphere(const srm::vec3<double>& up)
{
    srm::vec3<double> p = random_sphere();
    if(srm::dot(p, up) < 0) p = -p;
    return p;
}

inline srm::vec3<double> random_sphere_surface()
{
    auto p = random_sphere();
    return p.normalize();
}

inline srm::vec3<double> random_hemisphere_surface(const srm::vec3<double>& up)
{
    auto p = random_sphere_surface();
    if(srm::dot(p, up) < 0) p = -p;
    return p;
}