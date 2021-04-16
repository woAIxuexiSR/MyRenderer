#pragma once

#include <cmath>
#include "math/utility.hpp"
#include "geometry/ray.hpp"

class Camera
{
private:
    point origin;
    point low_left;
    direction horizontal;
    direction vertical;

public:
    Camera(point lookfrom = point(0, 0, 1), 
           point lookat = point(0, 0, -1), 
           direction up = point(0, 1, 0), 
           double vfov = 90, 
           double aspect_ratio = 1.25 )
    {
        const double focal_lenth = 1.0;
        double height = 2 * tan(degree_to_radius(vfov) / 2);
        double width = height * aspect_ratio;

        direction forward = lookat - lookfrom;

        origin = lookfrom;
        horizontal = srm::cross(forward, up).normalize() * width;
        vertical = srm::cross(horizontal, forward).normalize() * height;
        low_left = lookfrom + forward.normalize() * focal_lenth - horizontal * 0.5 - vertical * 0.5;
    }

    inline ray get_ray(double w, double h)
    {
        return ray(origin, low_left - origin + horizontal * w + vertical * h);
    }
};