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
    cout << d * f << endl;
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
    sphere ball1(point(10.1, 10.1, 10.1), 10, nullptr);
    ray r(point(0, 0, 0), direction(-1, -1, -1));

    hit_record rec;

    cout << (ball1.hit(r, rec) ? "YES" : "NO") << endl;
}

inline color ray_color(const ray& r, const BVHnode& world, int depth)
{
    if(depth <= 0) return color(0, 0, 0);
     
    hit_record rec;
    if(world.hit(r, rec))
    {
        color attenuation;
        ray scattered;
        if(rec.hit_mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }
    
    double t = 0.5 * (r.get_dir().normalize().y + 1.0);
    return color(1, 1, 1) * t + color(0.5, 0.7, 1.0) * (1 - t);
}

void draw_pic()
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

double pdf(double x)
{
    return 0.5 * x;
}

// importance sampling

int main()
{
    clock_t start = clock();
    //math_test();
    //framebuffer_test();
    //geometry_test();
    draw_pic();

    // const int N = 10000000;
    // double sum = 0;
    // for(int i = 0; i < N; ++i)
    // {
    //     double x = sqrt(random_double(0, 4));
    //     sum += x * x * x / pdf(x);
    // }
    // cout << sum / (double)N << endl;

    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
    return 0;
}