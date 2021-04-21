#include "texture.hpp"

color solid_color::get_color(coord uv) const
{
    return c;
}

color checker::get_color(coord uv) const
{
    double weight = 2 * pi * cycle;
    double sines = sin(uv.x * weight) * sin(uv.y * weight);
    
    if(sines < 0) return odd->get_color(uv);
    return even->get_color(uv);
}