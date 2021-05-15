#pragma once

#include "math/vector.hpp"
#include "math/ray.hpp"
#include "math/utility.hpp"

inline bool check_cross(double& t_min, double& t_max, double t0, double t1)
{
    if(t0 > t1)
        std::swap(t0, t1);

    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;

    if(t_min >= t_max) return false;
    return true;
}

class AABB
{
public:
    point minimum;
    point maximum;

public:
    AABB() {}
    AABB(point _s, point _l) : minimum(_s), maximum(_l) {}
    AABB(const AABB& _a, const AABB& _b)
    {
        minimum = point(fmin(_a.minimum.x, _b.minimum.x), fmin(_a.minimum.y, _b.minimum.y), fmin(_a.minimum.z, _b.minimum.z));
        maximum = point(fmax(_a.maximum.x, _b.maximum.x), fmax(_a.maximum.y, _b.maximum.y), fmax(_a.maximum.z, _b.maximum.z));
    }

    inline bool hit(const ray& r, interval t_interval) const
    {
        double t_min = t_interval.x, t_max = t_interval.y;
        point rori = r.get_ori(), rdir = r.get_dir();

        if(fabs(rdir.x) < EPS)
        {
            if(rori.x < minimum.x || rori.x > maximum.x)
                return false;
        }
        else if(!check_cross(t_min, t_max, (minimum.x - rori.x) / rdir.x, (maximum.x - rori.x) / rdir.x))
            return false;


        if(fabs(rdir.y) < EPS)
        {
            if(rori.y < minimum.y || rori.y > maximum.y)
                return false;
        }
        else if(!check_cross(t_min, t_max, (minimum.y - rori.y) / rdir.y, (maximum.y - rori.y) / rdir.y))
            return false;


        if(fabs(rdir.z) < EPS)
        {
            if(rori.z < minimum.z || rori.z > maximum.z)
                return false;
        }
        else if(!check_cross(t_min, t_max, (minimum.z - rori.z) / rdir.z, (maximum.z - rori.z) / rdir.z))
            return false;


        return true;
    }
};