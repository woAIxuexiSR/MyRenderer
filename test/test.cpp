#include <iostream>
#include <time.h>
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "camera/framebuffer.hpp"
#include "camera/camera.hpp"
#include "geometry/geometry.hpp"
#include "material/material.hpp"
#include "geometry/bvhnode.hpp"

using namespace std;
using namespace srm;

void math_test()
{
    mat3<float> a(vec3<float>(3, 4, 5), vec3<float>(1, 2, 4), vec3<float>(4, 3, 1));
    mat3<float> b(vec3<float>(3, 1, 5), vec3<float>(4, 2, 4), vec3<float>(1, 3, 1));
    vec3<float> c(1.1, 2.2, 3.3);

    mat4<float> d(vec4<float>(3, 4, 5, 6), vec4<float>(1, 2, 3, 3), vec4<float>(4, 3, 1, 2), vec4<float>(2, 3, 3, 5));
    mat4<float> e(vec4<float>(3, 4, 5, 6), vec4<float>(1, 2, 3, 3), vec4<float>(4, 3, 1, 2), vec4<float>(2, 4, 3, 5));
    vec4<float> f(1.1, 2.2, 3.3, 4.4);

    cout << a.inverse() << endl;
}

void framebuffer_test()
{
    const int height = 256, width = 256;
    FrameBuffer fb(width, height);

    for (int j = height - 1; j >= 0; --j)
        for (int i = 0; i < width; ++i)
            fb.set_pixel(i, j, vec3<double>(255, 255, 255));

    fb.output("../images/test.ppm");
}

void geometry_test()
{
    ray r(point(0, 0, 0), direction(1, 0, 0));

    geometry_list world;
    world.add(make_shared<triangle>(point(2, 0, 0), point(2, 1, 1), point(2, -1, 1), nullptr));
    
    BVHnode bvh(world);

    hit_record rec;

    cout << (bvh.hit(r, rec) ? "YES" : "NO") << endl;
    cout << rec.t << " " << rec.p << endl;
    cout << rec.normal << endl;

}

inline color ray_color(const ray& r, const BVHnode& world, int depth)
{
    static const color background = color(0.0, 0.0, 0.0);

    if(depth <= 0) return color(0.0, 0.0, 0.0);
     
    hit_record rec;
    if(world.hit(r, rec))
    {
        color attenuation;
        ray scattered;
        color emit = rec.hit_mat->emitted(rec.uv);

        if(rec.hit_mat->scatter(r, rec, attenuation, scattered))
            return emit + attenuation * ray_color(scattered, world, depth - 1);
        return emit;
    }
    
    return background;
}

void rt1()
{
    const int height = 720, width = 720 * 1.25;
    const int max_depth = 100;
    const int samples_per_pixel = 200;
    FrameBuffer fb(width, height);

    Camera mycamera(point(13, 2, 3), point(0, 0, 0), direction(0, 1, 0), 20);
    //Camera mycamera;

    geometry_list world;

    auto material_ground = make_shared<diffuse>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point(0.0, -1000.0, -1.0), 1000, material_ground));

    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            double choose_mat = random_double();
            point center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point(4, 0.2, 0)).length() > 0.9) 
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.6) 
                {
                    // diffuse
                    color albedo = random_v3();
                    sphere_material = make_shared<diffuse>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } 
                else if (choose_mat < 0.8)
                {
                    // metal
                    color albedo = random_v3(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<glossy>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } 
                else 
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<diffuse>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<glossy>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point(4, 1, 0), 1.0, material3));

    BVHnode bvh(world);

    for(int i = 0; i < height; ++i)
        for(int j = 0; j < width; ++j)
        {
            color result(0, 0, 0);
            for(int k = 0; k < samples_per_pixel; ++k)
            {
                double u = (i + random_double()) / height;
                double v = (j + random_double()) / width;

                ray r = mycamera.get_ray(v, u);
                result = result + ray_color(r, bvh, max_depth);
            }
            result = result / samples_per_pixel;

            fb.set_pixel(i, j, result);
        }

    fb.output("../images/test.ppm");
}

void rt2()
{
    const int width = 720 * 1.25, height = 720;
    const int max_depth = 100;
    const int sample_per_pixel = 20;
    
    FrameBuffer fb(width, height);

    //Camera mycamera(point(-2, 2, 1), point(0, 0, -1), direction(0, 1, 0), 90);
    Camera mycamera(point(0, 0, 3));

    geometry_list world;

    auto material_ground = make_shared<diffuse>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point(0.0, -100.5, -1.0), 100, material_ground));

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point(1, 0, -1), 0.5, material1));

    //auto texture = make_shared<checker>(color(1, 1, 1), color(1, 0, 0), 1);
    auto texture = make_shared<imageTex>("../images/test1.jpg");
    auto material2 = make_shared<diffuse>(texture);
    world.add(make_shared<sphere>(point(0, 0, -1), 0.5, material2));

    auto material3 = make_shared<glossy>(color(0.7, 0.6, 0.5), 0.3);
    world.add(make_shared<sphere>(point(-1, 0, -1), 0.5, material3));

    auto light_material = make_shared<diffuse_light>(color(4.0, 4.0, 4.0));
    //world.add(make_shared<sphere>(point(0, 2.2, -1), 1.5, light_material));

    auto material4 = make_shared<diffuse>(color(0.7, 0.0, 0.0));
    world.add(make_shared<triangle>(point(2, -0.5, -2), point(2, -0.5, 0), point(2, 2.5, -2), light_material));
    world.add(make_shared<triangle>(point(2, 2.5, -2), point(2, 2.5, 0), point(2, -0.5, 0), light_material));

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

    fb.output("../images/test.ppm");
}

void rt3()
{
    const int width = 720 * 1.25, height = 720;
    const int max_depth = 20;
    const int sample_per_pixel = 20;
    
    FrameBuffer fb(width, height);

    Camera mycamera(point(478, 278, -600), point(278, 278, 0), direction(0, 1, 0), 40.0);

    geometry_list world;

    geometry_list boxes1;
    auto ground = make_shared<diffuse>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point(x0, y0, z0), point(x1, y1, z1), ground));
        }
    }

    world.add(make_shared<BVHnode>(boxes1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    world.add(make_shared<xz_rect>(554, 123, 423, 147, 412, light));

    auto center = point(400, 400, 200);
    auto sphere_material = make_shared<diffuse>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center, 50, sphere_material));

    world.add(make_shared<sphere>(point(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(point(0, 150, 145), 50, make_shared<glossy>(color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<sphere>(point(360, 150, 145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    auto iso = make_shared<isotropic>(color(0.2, 0.4, 0.9));
    world.add(make_shared<constant_medium>(boundary, 0.2, iso));
    
    boundary = make_shared<sphere>(point(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    iso = make_shared<isotropic>(color(1, 1, 1));
    world.add(make_shared<constant_medium>(boundary, .0001, iso));      // There is a problem!!!

    auto emat = make_shared<diffuse>(make_shared<imageTex>("../images/test2.jpg"));
    world.add(make_shared<sphere>(point(400, 200, 400), 100, emat));
    auto pertext = make_shared<diffuse>(color(0.1, 0.1, 0.1));
    world.add(make_shared<sphere>(point(220, 280, 300), 80, pertext));

    geometry_list boxes2;
    auto white = make_shared<diffuse>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(random_v3(0, 165), 10, white));
    }

    world.add(make_shared<translate>(
                make_shared<rotate_y>(make_shared<BVHnode>(boxes2), 15),
                direction(-100, 270, 395)
                )
            );

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

    fb.output("../images/test.ppm");
}

double pdf(const point& p)
{
    return 1.0 / (4 * pi);
}

// importance sampling

int main()
{
    clock_t start = clock();
    //math_test();
    //framebuffer_test();
    //geometry_test();
    //rt1();
    //rt2();
    rt3();

    // const int N = 10000000;
    // double sum = 0;
    // for(int i = 0; i < N; ++i)
    // {
    //     point p = random_sphere_surface();
    //     sum += p.z * p.z / pdf(p);
    // }
    // cout << sum / (double)N << endl;
    // cout << 4.0 / 3.0 * pi << endl;

    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
    return 0;
}