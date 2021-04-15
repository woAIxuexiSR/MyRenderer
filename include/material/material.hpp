#pragma once

#include "geometry/geometry.hpp"

class material
{
public:
    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class diffuse : public material
{
private:
    color albedo;

public:
    diffuse() {}
    diffuse(color _a) : albedo(_a) {}

    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

#include "material.inl"