#include "bvhnode.hpp"

enum class AXIS { AXIS_X, AXIS_Y, AXIS_Z };

bool box_compare(const std::shared_ptr<geometry>& a, const std::shared_ptr<geometry>& b, AXIS axis)
{
    AABB abox = a->bounding_box();
    AABB bbox = b->bounding_box();

    switch(axis)
    {
    case AXIS::AXIS_X : return abox.minimum.x < bbox.minimum.x;
    case AXIS::AXIS_Y : return abox.minimum.y < bbox.minimum.y;
    case AXIS::AXIS_Z : return abox.minimum.z < bbox.minimum.z;
    }

    return true;
}

bool box_cmp_x(const std::shared_ptr<geometry>& a, const std::shared_ptr<geometry>& b)
{
    return box_compare(a, b, AXIS::AXIS_X);
}
bool box_cmp_y(const std::shared_ptr<geometry>& a, const std::shared_ptr<geometry>& b)
{
    return box_compare(a, b, AXIS::AXIS_Y);
}
bool box_cmp_z(const std::shared_ptr<geometry>& a, const std::shared_ptr<geometry>& b)
{
    return box_compare(a, b, AXIS::AXIS_Z);
}

BVHnode::BVHnode(const geometry_list& list)
{
    auto objects = list.objects;
    *this = BVHnode(objects, 0, objects.size());
}

BVHnode::BVHnode(std::vector<std::shared_ptr<geometry> >& src_objects, int start, int end)
{
    if(end - start == 1)
    {
        left = right = src_objects[start];
    }
    else if(end - start == 2)
    {
        left = src_objects[start];
        right = src_objects[start + 1];
    }
    else
    {
        AXIS axis = (AXIS)random_int(0, 2);
        auto cmp = (axis == AXIS::AXIS_X) ? box_cmp_x
                 : (axis == AXIS::AXIS_Y) ? box_cmp_y
                                          : box_cmp_z;

        std::sort(src_objects.begin() + start, src_objects.begin() + end, cmp);
        int mid = start + (end - start) / 2;
        left = std::make_shared<BVHnode>(src_objects, start, mid);
        right = std::make_shared<BVHnode>(src_objects, mid, end);
    }

    AABB bleft = left->bounding_box();
    AABB bright = right->bounding_box();
    box = AABB(bleft, bright);
}

bool BVHnode::hit(const ray& r, hit_record& rec, interval t_interval) const
{
    if(!box.hit(r, t_interval))
        return false;

    bool hit_left = left->hit(r, rec, t_interval);
    if(hit_left) t_interval.y = rec.t;
    bool hit_right = right->hit(r, rec, t_interval);

    return hit_left || hit_right;
}
    
AABB BVHnode::bounding_box() const
{
    return box;
}