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
    rec.hit_mat = mat;
    direction normal = (rec.p - center).normalize();
    rec.set_normal(rdir, normal);
    rec.uv = get_sphere_uv(normal);

    return true;
}

AABB sphere::bounding_box() const
{
    return AABB(center - point(radius), center + point(radius));
}

coord sphere::get_sphere_uv(const point& p)
{
    double theta = acos(-p.y);
    double phi = atan2(-p.z, p.x) + pi;

    return coord(phi / (2 * pi), theta / pi);
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
    rec.hit_mat = mat;
    rec.set_normal(r.get_dir(), normal);
    rec.uv = textureCoord[0] * txy.y + textureCoord[1] * txy.z + textureCoord[2] * (1 - txy.y - txy.z);

    return true;
}

AABB triangle::bounding_box() const
{
    point m(fmin(fmin(vertex[0].x, vertex[1].x), vertex[2].x),
            fmin(fmin(vertex[0].y, vertex[1].y), vertex[2].y),
            fmin(fmin(vertex[0].z, vertex[1].z), vertex[2].z));
    point M(fmax(fmax(vertex[0].x, vertex[1].x), vertex[2].x),
            fmax(fmax(vertex[0].y, vertex[1].y), vertex[2].y),
            fmax(fmax(vertex[0].z, vertex[1].z), vertex[2].z));

    if(fabs(M.x - m.x) < srm::EPS) M.x += 0.01, m.x -= 0.01;
    if(fabs(M.y - m.y) < srm::EPS) M.y += 0.01, m.x -= 0.01;
    if(fabs(M.z - m.z) < srm::EPS) M.z += 0.01, m.x -= 0.01;

    return AABB(m, M);
}

bool yz_rect::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    point rori = r.get_ori();
    direction rdir = r.get_dir();

    double t = fabs(rdir.x) < srm::EPS ? -1 : (x - rori.x) / rdir.x;
    if(!t_interval.in_interval(t))
        return false;
    
    point p = r.at(t);
    if(p.y < y0 || p.y > y1 || p.z < z0 || p.z > z1)
        return false;

    rec.t = t;
    rec.p = p;
    rec.hit_mat = mat;
    rec.set_normal(rdir, direction(1, 0, 0));
    rec.uv = coord((p.z - z0) / (z1 - z0), (p.y - y0) / (y1 - y0));
    
    return true;
}

AABB yz_rect::bounding_box() const
{
    return AABB(point(x - 0.001, y0, z0), point(x + 0.001, y1, z1));
}

bool xy_rect::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    point rori = r.get_ori();
    direction rdir = r.get_dir();

    double t = fabs(rdir.z) < srm::EPS ? -1 : (z - rori.z) / rdir.z;
    if(!t_interval.in_interval(t))
        return false;
    
    point p = r.at(t);
    if(p.x < x0 || p.x > x1 || p.y < y0 || p.y > y1)
        return false;

    rec.t = t;
    rec.p = p;
    rec.hit_mat = mat;
    rec.set_normal(rdir, direction(0, 0, 1));
    rec.uv = coord((p.x - x0) / (x1 - x0), (p.y - y0) / (y1 - y0));
    
    return true;
}

AABB xy_rect::bounding_box() const
{
    return AABB(point(x0, y0, z - 0.001), point(x1, y1, z + 0.001));
}

bool xz_rect::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    point rori = r.get_ori();
    direction rdir = r.get_dir();

    double t = fabs(rdir.y) < srm::EPS ? -1 : (y - rori.y) / rdir.y;
    if(!t_interval.in_interval(t))
        return false;
    
    point p = r.at(t);
    if(p.x < x0 || p.x > x1 || p.z < z0 || p.z > z1)
        return false;

    rec.t = t;
    rec.p = p;
    rec.hit_mat = mat;
    rec.set_normal(rdir, direction(0, 1, 0));
    rec.uv = coord((p.x - x0) / (x1 - x0), (p.z - z0) / (z1 - z0));
    
    return true;
}

AABB xz_rect::bounding_box() const
{
    return AABB(point(x0, y - 0.001, z0), point(x1, y + 0.001, z1));
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
        first = false;
    }
    
    return ans;
}

box::box(point _m, point _M, std::shared_ptr<material> mat) : m(_m), M(_M)
{
    faces.add(std::make_shared<xy_rect>(_m.z, _m.x, _M.x, _m.y, _M.y, mat));
    faces.add(std::make_shared<xy_rect>(_M.z, _m.x, _M.x, _m.y, _M.y, mat));
    
    faces.add(std::make_shared<xz_rect>(_m.y, _m.x, _M.x, _m.z, _M.z, mat));
    faces.add(std::make_shared<xz_rect>(_M.y, _m.x, _M.x, _m.z, _M.z, mat));

    faces.add(std::make_shared<yz_rect>(_m.x, _m.y, _M.y, _m.z, _M.z, mat));
    faces.add(std::make_shared<yz_rect>(_M.x, _m.y, _M.y, _m.z, _M.z, mat));
}

bool box::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    return faces.hit(r, rec, t_interval);
}

AABB box::bounding_box() const
{
    return AABB(m, M);
}

bool translate::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    ray moved(r.get_ori() - offset, r.get_dir());

    if(!object->hit(moved, rec, t_interval))
        return false;

    rec.p = rec.p + offset;
    return true;
}

AABB translate::bounding_box() const
{
    AABB aabb = object->bounding_box();
    return AABB(aabb.minimum + offset, aabb.maximum + offset);
}

rotate_y::rotate_y(std::shared_ptr<geometry> _o, double degree) : object(_o)
{
    double radius = degree_to_radius(degree);
    
    sine = sin(radius);
    cosine = cos(radius);

    AABB aabb = _o->bounding_box();

    double x0 = aabb.minimum.x, x1 = aabb.maximum.x, tmpx;
    double z0 = aabb.minimum.z, z1 = aabb.maximum.z, tmpz;

    point m(cosine * x0 + sine * z0, aabb.minimum.y, -sine * x0 + cosine * z0);
    point M(cosine * x0 + sine * z0, aabb.maximum.y, -sine * x0 + cosine * z0);

    tmpx = cosine * x0 + sine * z1, tmpz = -sine * x0 + cosine * z1;
    m.x = m.x < tmpx ? m.x : tmpx; m.z = m.z < tmpz ? m.z : tmpz;
    M.x = M.x > tmpx ? M.x : tmpx; M.z = M.z > tmpz ? M.z : tmpz;

    tmpx = cosine * x1 + sine * z0, tmpz = -sine * x1 + cosine * z0;
    m.x = m.x < tmpx ? m.x : tmpx; m.z = m.z < tmpz ? m.z : tmpz;
    M.x = M.x > tmpx ? M.x : tmpx; M.z = M.z > tmpz ? M.z : tmpz;

    tmpx = cosine * x1 + sine * z1, tmpz = -sine * x1 + cosine * z1;
    m.x = m.x < tmpx ? m.x : tmpx; m.z = m.z < tmpz ? m.z : tmpz;
    M.x = M.x > tmpx ? M.x : tmpx; M.z = M.z > tmpz ? M.z : tmpz;

    box = AABB(m, M);
}

bool rotate_y::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    point rori = r.get_ori(); direction rdir = r.get_dir();
    
    double tmpx, tmpz;

    // ray rotate reversely
    tmpx = cosine * rori.x - sine * rori.z; tmpz = sine * rori.x + cosine * rori.z;
    point newori(tmpx, rori.y, tmpz);

    tmpx = cosine * rdir.x - sine * rdir.z; tmpz = sine * rdir.x + cosine * rdir.z;
    direction newdir(tmpx, rdir.y, tmpz);

    ray rotated(newori, newdir);

    if(!object->hit(rotated, rec,t_interval))
        return false;

    point p = rec.p;
    tmpx = cosine * p.x + sine * p.z; tmpz = -sine * p.x + cosine * p.z;
    rec.p = point(tmpx, p.y, tmpz);

    direction normal = rec.normal;
    tmpx = cosine * normal.x + sine * normal.z; tmpz = -sine * normal.x + cosine * normal.z;
    rec.set_normal(rdir, direction(tmpx, normal.y, tmpz));

    return true;
}

AABB rotate_y::bounding_box() const
{
    return box;
}

bool constant_medium::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    hit_record rec1, rec2;
    if(!boundary->hit(r, rec1, interval(-infinity, infinity)))
        return false;
    if(!boundary->hit(r, rec2, interval(rec1.t + 0.1, infinity)))
        return false;

    rec1.t = rec1.t > t_interval.x ? rec1.t : t_interval.x;
    rec2.t = rec2.t < t_interval.y ? rec2.t : t_interval.y;

    if(rec1.t >= rec2.t) return false;

    const double rdir_len = r.get_dir().length();
    const double dis = (rec2.t - rec1.t) * rdir_len;
    const double hit_dis = neg_inv_density * log(random_double());

    if(hit_dis > dis) return false;

    rec.t = rec1.t + hit_dis / rdir_len;
    rec.p = r.at(rec.t);
    rec.hit_mat = phase_function;
    rec.uv = (rec1.uv + rec2.uv) * 0.5;
    rec.set_normal(r.get_dir(), random_sphere_surface());

    return true;
}

AABB constant_medium::bounding_box() const
{
    return boundary->bounding_box();
}