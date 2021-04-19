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
    double u, v;

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
};

class triangle : public geometry
{
private:
    point vertex[3];
    direction normal;
    std::shared_ptr<material> mat;

public:
    triangle() {}
    triangle(const point& _a, const point& _b, const point& _c, std::shared_ptr<material> _m) : vertex{_a, _b, _c} { normal = srm::cross(_a - _b, _a - _c).normalize(); }

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