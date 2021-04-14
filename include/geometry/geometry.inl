#include "geometry.hpp"

bool sphere::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    direction rdir = r.get_dir(), dis = r.get_ori() - center;

    double a = srm::dot(rdir, rdir);
    double half_b = srm::dot(rdir, dis);
    double c = srm::dot(dis, dis) - radius * radius;

    double delta = half_b * half_b - a * c;
    if(delta < 0) return false;

    double ans = (-half_b - sqrt(delta)) / a;
    if(!t_interval.in_interval(ans))
    {
        ans = (-half_b + sqrt(delta)) / a;
        if(!t_interval.in_interval(ans))
            return false;
    }

    rec.t = ans;
    rec.p = r.at(ans);
    rec.normal = (rec.p - center).normalize();

    return true;
}

bool triangle::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    return true;
}

bool geometry_list::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    return true;
}