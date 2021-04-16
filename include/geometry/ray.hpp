#pragma once

#include "math/vector.hpp"

class ray
{
private:
    point ori;
    direction dir;

public:
    ray() {}
    ray(const point& _o, const direction _d) : ori(_o), dir(_d.normalize()) {}

    point get_ori() const { return ori; }
    direction get_dir() const { return dir; }
    point at(double t) const { return ori + dir * t; }
};