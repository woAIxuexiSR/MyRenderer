#pragma once

#include <cmath>
#include <vector>
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "math/utility.hpp"

class material;

class ray
{
private:
    point ori;
    direction dir;

public:
    ray() {}
    ray(const point& _o, const direction _d) : ori(_o), dir(_d.normalize()) {}

    point get_ori() const { return ori; }
    direction get_dir() const { return dir; }
    point at(double t) const { return ori + dir * t; }
};

class hit_record
{
public:
    point p;
    direction normal;
    double t;
    std::shared_ptr<material> hit_mat;
};

class geometry
{
public:
    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const = 0;
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
};

class geometry_list : public geometry
{
private:
    std::vector<std::shared_ptr<geometry> > objects;

public:
    void clear() { objects.clear(); }
    void add(std::shared_ptr<geometry> _a) { objects.push_back(_a); }

    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const override;
};

#include "geometry.inl"