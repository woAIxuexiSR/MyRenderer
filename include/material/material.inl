#include "material.hpp"

bool diffuse::scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const
{
    direction out = random_hemisphere(rec.normal);
    while(srm::dot(out, out) < srm::EPS)
        out = random_hemisphere(rec.normal);

    scattered = ray(rec.p, out);
    attenuation = albedo;
    return true;
}

bool specular::scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const
{
    direction rdir = r.get_dir().normalize();
    direction out = rdir - rec.normal * (srm::dot(rdir, rec.normal) * 2);

    scattered = ray(rec.p, out);
    attenuation = albedo;
    return srm::dot(out, rec.normal) > 0;
}

bool glossy::scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const
{
    direction rdir = r.get_dir();
    direction out = rdir - rec.normal * (srm::dot(rdir, rec.normal) * 2);
    out = out.normalize() + random_sphere() * radius;

    if(srm::dot(out, rec.normal) < 0)
        return false;
    
    scattered = ray(rec.p, out);
    attenuation = albedo;
    return true;
}

bool refract::scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const
{
    return true;
}