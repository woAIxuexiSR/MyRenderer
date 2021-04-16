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

class specular : public material
{
private:
    color albedo;

public:
    specular() {}
    specular(color _a) : albedo(_a) {}

    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

class glossy : public material
{
    // sample from a distant sphere, distance is set to 1
private:
    color albedo;
    double radius;

public:
    glossy() {}
    glossy(color _a, double _r) : albedo(_a), radius(_r) {}

    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

class refract : public material
{
private:
    double index;

public:
    refract() {}
    refract(double _i): index(_i) {}

    virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

#include "material.inl"