#include "material.hpp"

bool diffuse::scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const
{
    direction out = random_hemisphere(rec.normal);

    scattered = ray(rec.p, out);
    attenuation = albedo;
    return true;
}