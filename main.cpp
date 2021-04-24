#include <iostream>
#include "geometry/geometry.hpp"
#include "geometry/bvhnode.hpp"
#include "geometry/ray.hpp"
#include "material/material.hpp"
#include "material/texture.hpp"
#include "camera/framebuffer.hpp"
#include "camera/camera.hpp"

using std::make_shared;

color ray_color(const ray& r, const BVHnode& world, int depth)
{
    static const color background(0, 0, 0);

    if(depth <= 0) return color(0, 0, 0);

    hit_record rec;
    if(world.hit(r, rec))
    {
        color emit = rec.hit_mat->emitted(rec.uv);

        color attenuation;
        ray scattered;
        if(rec.hit_mat->scatter(r, rec, attenuation, scattered))
            return emit + attenuation * ray_color(scattered, world, depth - 1);
        else
            return emit;
    }

    return background;
}

void cornell_box()
{
    const double aspect_ratio = 1.0;
    const int height = 600, width = height * aspect_ratio;
    const int max_depth = 50;
    const int sample_per_pixel = 20;

    FrameBuffer fb(width, height);
    
    Camera mycamera(point(278, 278, -800), point(278, 278, 0), direction(0, 1, 0), 40, aspect_ratio);

    geometry_list world;

    auto red   = make_shared<diffuse>(color(.65, .05, .05));
    auto white = make_shared<diffuse>(color(.73, .73, .73));
    auto green = make_shared<diffuse>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<yz_rect>(555, 0, 555, 0, 555, green));
    world.add(make_shared<yz_rect>(0, 0, 555, 0, 555, red));
    world.add(make_shared<xz_rect>(554, 213, 343, 227, 332, light));
    world.add(make_shared<xz_rect>(0, 0, 555, 0, 555, white));
    world.add(make_shared<xz_rect>(555, 0, 555, 0, 555, white));
    world.add(make_shared<xy_rect>(555, 0, 555, 0, 555, white));

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
                color rc = ray_color(r, bvh, max_depth);

                result = result + rc;
            }

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