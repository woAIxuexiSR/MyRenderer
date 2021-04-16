#pragma once

#include <algorithm>
#include "geometry.hpp"
#include "aabb.hpp"

class BVHnode : public geometry
{
private:
    std::shared_ptr<geometry> left;
    std::shared_ptr<geometry> right;
    AABB box;

public:
    BVHnode() {}
    BVHnode(const geometry_list& list);
    BVHnode(std::vector<std::shared_ptr<geometry> >& src_objects, int start, int end);

    virtual bool hit(const ray& r, hit_record& rec, interval t_interval = interval(0.001, infinity)) const override;
    virtual AABB bounding_box() const override;
};

#include "bvhnode.inl"