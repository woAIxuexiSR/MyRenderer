#include "material.hpp"

bool diffuse::scatter(const ray& r, const hit_record& rec, scatter_record& srec) const
{
    srec.attenuation = albedo->get_color(rec.uv);
    srec.brdf_pdf = std::make_shared<cosine_pdf>(rec.normal);
    srec.is_specular = false;

    return true;
}

double diffuse::brdf_cos(const ray& r, const hit_record& rec, const ray& scattered) const
{
    double brdf = 1.0 / PI;
    double cosine = dot(rec.normal, scattered.get_dir());
    return (cosine <= 0) ? 0 : brdf * cosine;
}

bool specular::scatter(const ray& r, const hit_record& rec, scatter_record& srec) const
{
    direction rdir = r.get_dir();
    direction out = rdir - rec.normal * (dot(rdir, rec.normal) * 2 / rec.normal.length());

    srec.specular_ray = ray(rec.p, out);
    srec.is_specular = true;
    srec.attenuation = albedo->get_color(rec.uv);
    srec.brdf_pdf = nullptr;
    return dot(out, rec.normal) > 0;
}

bool glossy::scatter(const ray& r, const hit_record& rec, scatter_record& srec) const
{
    direction rdir = r.get_dir();
    direction out = rdir - rec.normal * (dot(rdir, rec.normal) * 2 / rec.normal.length());
    out = out.normalize() + random_sphere() * radius;

    if(dot(out, rec.normal) < 0)
        return false;
    
    srec.specular_ray = ray(rec.p, out);
    srec.is_specular = true;
    srec.attenuation = albedo->get_color(rec.uv);
    srec.brdf_pdf = nullptr;
    return true;
}

bool dielectric::scatter(const ray& r, const hit_record& rec, scatter_record& srec) const
{
    double refract_ratio = rec.front_face ? (1.0 / index) : index;

    direction rdir = r.get_dir().normalize(), normal = rec.normal.normalize();
    double cosine_theta = -dot(rdir, normal);
    
    direction out_perpendicular = (rdir + normal * cosine_theta) * refract_ratio;
    double sine_theta_prime = dot(out_perpendicular, out_perpendicular);

    ray scattered;
    if(sine_theta_prime > 1.0 || reflectance(cosine_theta, refract_ratio) > random_double())          // reflect
        scattered = ray(rec.p, rdir + normal * (cosine_theta * 2));
    else        // refract
    {
        direction out_parallel = normal * (-sqrt(1.0 - sine_theta_prime));
        scattered = ray(rec.p, out_perpendicular + out_parallel);
    }

    srec.specular_ray = scattered;
    srec.is_specular = true;
    srec.attenuation = color(1.0, 1.0, 1.0);
    srec.brdf_pdf = nullptr;

    return true;
}

double dielectric::reflectance(double cosine, double _i)
{
    double r0 = (1 - _i) / (1 + _i);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

// bool isotropic::scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const
// {
//     attenuation = albedo->get_color(rec.uv);
//     scattered = ray(rec.p, random_sphere_surface());
//     return true;
// }