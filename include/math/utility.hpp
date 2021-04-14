#pragma once

#include <cmath>
#include <limits>
#include <memory>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = std::acos(-1);

inline double degree_to_radius(double degree)
{
    return degree / 180.0 * pi;
}

template <class T>
inline T clamp(T x, T x_min, T x_max)
{
    if(x < x_min) return x_min;
    if(x > x_max) return x_max;
    return x;
}