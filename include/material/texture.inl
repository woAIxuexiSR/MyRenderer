#include "texture.hpp"

color solid_color::get_color(double u, double v) const
{
    return c;
}

color checker::get_color(double u, double v) const
{
    double weight = pi / frequency;
    double sines = sin(u * weight) * sin(v * weight);
    
    if(sines < 0) return odd->get_color(u, v);
    return even->get_color(u, v);
}