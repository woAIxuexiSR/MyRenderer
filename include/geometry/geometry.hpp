#pragma once

#include <cmath>
#include <vector>
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "math/utility.hpp"
#include "ray.hpp"
#include "aabb.hpp"

class material;

class hit_record
{
public:
    point p;
    direction normal;
    double t;
    std::shared_ptr<material> hit_mat;
    bool front_face;
    coord uv;

    void set_normal(const direction& rdir, const direction& norm)
    {
        front_face = dot(rdir, norm) < 0;
        normal = front_face ? norm : -norm;
    }
};

class geometry
{
public:
    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const = 0;
    virtual AABB bounding_box() const = 0;
};

class sphere : public geometry
{
private:
    point center;
    double radius;
    std::shared_ptr<material> mat;

public:
    sphere() {}
    sphere(const point& _c, double _r, std::shared_ptr<material> _m) : center(_c), radius(_r), mat(_m) {}

    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const override;
    virtual AABB bounding_box() const override;

private:
    static coord get_sphere_uv(const point& p);
};

class triangle : public geometry
{
private:
    point vertex[3];
    direction normal;
    std::shared_ptr<material> mat;
    coord textureCoord[3];

public:
    triangle() {}
    triangle(const point& _a, const point& _b, const point& _c, std::shared_ptr<material> _m,
            const coord& _tc1 = coord(0, 0), const coord& _tc2 = coord(0, 0), const coord& _tc3 = coord(0, 0))
            : vertex{_a, _b, _c}, mat(_m), textureCoord{_tc1, _tc2, _tc3} { normal = srm::cross(_a - _b, _a - _c).normalize(); }

    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const override;
    virtual AABB bounding_box() const override;
};

class yz_rect : public geometry
{
private:
    double x;
    double y0, y1, z0, z1;
    std::shared_ptr<material> mat;

public:
    yz_rect() {}
    yz_rect(double _x, double _y0, double _y1, double _z0, double _z1, std::shared_ptr<material> _m)
            : x(_x), y0(_y0), y1(_y1), z0(_z0), z1(_z1), mat(_m) {}

    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const override;
    virtual AABB bounding_box() const override;
};

class xy_rect : public geometry
{
private:
    double z;
    double x0, x1, y0, y1;
    std::shared_ptr<material> mat;

public:
    xy_rect() {}
    xy_rect(double _z, double _x0, double _x1, double _y0, double _y1, std::shared_ptr<material> _m)
            : z(_z), x0(_x0), x1(_x1), y0(_y0), y1(_y1), mat(_m) {}

    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const override;
    virtual AABB bounding_box() const override;
};

class xz_rect : public geometry
{
private:
    double y;
    double x0, x1, z0, z1;
    std::shared_ptr<material> mat;

public:
    xz_rect() {}
    xz_rect(double _y, double _x0, double _x1, double _z0, double _z1, std::shared_ptr<material> _m)
            : y(_y), x0(_x0), x1(_x1), z0(_z0), z1(_z1), mat(_m) {}

    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const override;
    virtual AABB bounding_box() const override;
};

class geometry_list : public geometry
{
public:
    std::vector<std::shared_ptr<geometry> > objects;

public:
    void clear() { objects.clear(); }
    void add(std::shared_ptr<geometry> _a) { objects.push_back(_a); }

    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const override;
    virtual AABB bounding_box() const override;
};

#include "geometry.inl"