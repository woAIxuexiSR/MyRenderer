#include <iostream>
#include "geometry/geometry.hpp"
#include "geometry/bvhnode.hpp"
#include "geometry/ray.hpp"
#include "material/material.hpp"
#include "material/texture.hpp"
#include "camera/framebuffer.hpp"
#include "camera/camera.hpp"
#include "pdf/pdf.hpp"

using std::make_shared;
using std::shared_ptr;

inline color check_nan(const color& c)
{
    color ans = c;
    if(ans.x != ans.x) ans.x = 0;
    if(ans.y != ans.y) ans.y = 0;
    if(ans.z != ans.z) ans.z = 0;
    return ans;
}

const double RR = 0.6;

inline color ray_color(const ray& r, const BVHnode& world, const shared_ptr<geometry>& light, int depth)
{
    static const color background(0, 0, 0);

    if(depth <= 0) return color(0, 0, 0);

    hit_record rec;
    if(!world.hit(r, rec))
        return background;

    color emit = rec.hit_mat->emitted(rec.uv);

    color attenuation;
    ray scattered;
    double pdf_val;
    if(!rec.hit_mat->scatter(r, rec, attenuation, scattered, pdf_val))
        return emit;

    if(random_double() > RR)
        return emit;

    mixture_pdf mp;
    mp.add(make_shared<cosine_pdf>(rec.normal));
    mp.add(make_shared<geometry_pdf>(rec.p, light));
    scattered = ray(rec.p, mp.generate());
    pdf_val = mp.value(scattered.get_dir());

    return emit + attenuation * ray_color(scattered, world, light, depth - 1) * rec.hit_mat->brdf_cos(r, rec, scattered) / pdf_val / RR;
}

void cornell_box()
{
    const double aspect_ratio = 1.0;
    const int height = 600, width = height * aspect_ratio;
    const int max_depth = 50;
    const int sample_per_pixel = 500;

    FrameBuffer fb(width, height);
    
    Camera mycamera(point(278, 278, -800), point(278, 278, 0), direction(0, 1, 0), 40, aspect_ratio);

    geometry_list world;

    auto red   = make_shared<diffuse>(color(.65, .05, .05));
    auto white = make_shared<diffuse>(color(.73, .73, .73));
    auto green = make_shared<diffuse>(color(.12, .45, .15));
    auto light_material = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<yz_rect>(555, 0, 555, 0, 555, green));
    world.add(make_shared<yz_rect>(0, 0, 555, 0, 555, red));
    world.add(make_shared<xz_rect>(0, 0, 555, 0, 555, white));
    world.add(make_shared<xz_rect>(555, 0, 555, 0, 555, white));
    world.add(make_shared<xy_rect>(555, 0, 555, 0, 555, white));

    shared_ptr<geometry> box1 = make_shared<box>(point(0, 0, 0), point(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, direction(265, 0, 295));
    world.add(box1);

    shared_ptr<geometry> box2 = make_shared<box>(point(0, 0, 0), point(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, direction(130, 0, 65));
    world.add(box2);

    shared_ptr<geometry> light = make_shared<xz_rect>(554.9, 213, 343, 227, 332, light_material);
    world.add(light);

    BVHnode bvh(world);

    for(int i = 0; i < height; ++i)
        for(int j = 0; j < width; ++j)
        {
            color result(0, 0, 0);
            for(int k = 0; k < sample_per_pixel; ++k)
            {
                double u = (i + random_double()) / height;
                double v = (j + random_double()) / width;

                ray r = mycamera.get_ray(v, u);
                color rc = ray_color(r, bvh, light, max_depth);

                result = result + rc;
            }

            result = check_nan(result);
            fb.set_pixel(i, j, result / sample_per_pixel);
        }

    fb.output("./images/test.ppm");
}

int main()
{
    clock_t start = clock();

    cornell_box();

    clock_t end = clock();
    std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

    return 0;
}