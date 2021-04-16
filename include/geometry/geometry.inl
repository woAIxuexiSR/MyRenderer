#include "geometry.hpp"

bool sphere::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    // | (ori + t * dir) - center | = r ^ 2

    direction rdir = r.get_dir(), dis = r.get_ori() - center;

    double a = srm::dot(rdir, rdir);
    double half_b = srm::dot(rdir, dis);
    double c = srm::dot(dis, dis) - radius * radius;

    double delta = half_b * half_b - a * c;
    if(delta < 0) return false;
    delta = sqrt(delta);

    double ans = (-half_b - delta) / a;
    if(!t_interval.in_interval(ans))
    {
        ans = (-half_b + delta) / a;
        if(!t_interval.in_interval(ans))
            return false;
    }

    rec.t = ans;
    rec.p = r.at(ans);
    rec.normal = (rec.p - center).normalize();
    rec.hit_mat = mat;

    return true;
}

AABB sphere::bounding_box() const
{
    return AABB(center - point(radius), center + point(radius));
}

bool triangle::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    // ori + t * dir = A * x + B * y + C * (1 - x - y)

    srm::mat3<double> m(r.get_dir(), vertex[2] - vertex[0], vertex[2] - vertex[1]);
    srm::vec3<double> rg = vertex[2] - r.get_ori();

    double t = m.determinant();
    if(fabs(t) < srm::EPS)
        return false;

    srm::vec3<double> txy = m.inverse() * rg;
    
    interval xy_interval(0, 1);
    if( !t_interval.in_interval(txy.x) || !xy_interval.in_interval(txy.y) || !xy_interval.in_interval(txy.z) || !xy_interval.in_interval(1 - txy.y - txy.z) )
        return false;

    rec.t = txy.x;
    rec.p = r.at(rec.t);
    rec.normal = normal;
    rec.hit_mat = mat;

    return true;
}

AABB triangle::bounding_box() const
{
    point m = (fmin(fmin(vertex[0].x, vertex[1].x), vertex[2].x),
                fmin(fmin(vertex[0].y, vertex[1].y), vertex[2].y),
                fmin(fmin(vertex[0].z, vertex[1].z), vertex[2].z));
    point M = (fmax(fmax(vertex[0].x, vertex[1].x), vertex[2].x),
                fmax(fmax(vertex[0].y, vertex[1].y), vertex[2].y),
                fmax(fmax(vertex[0].z, vertex[1].z), vertex[2].z));

    return AABB(m, M);
}

bool geometry_list::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    hit_record tmp_rec;
    bool is_hit = false;
    double t_min = t_interval.y;

    for(const auto& object : objects)
    {
        if(object->hit(r, tmp_rec, interval(t_interval.x, t_min)))
        {
            is_hit = true;
            t_min = tmp_rec.t;
            rec = tmp_rec;
        }
    }
    return is_hit;
}

AABB geometry_list::bounding_box() const
{
    bool first = true;
    AABB ans;

    for(const auto& object : objects)
    {
        AABB tmp = object->bounding_box();
        ans = first ? tmp : AABB(ans, tmp);
        first = true;
    }
    
    return ans;
}