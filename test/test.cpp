#include <iostream>
#include <time.h>
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "framebuffer.hpp"
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

color ray_color(const ray& r, const BVHnode& world, int depth)
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
    
    double t = 0.5 * (r.get_dir().y + 1.0);
    return color(1.0) * (1 - t) + color(0.5, 0.7, 1.0) * t;
}

void draw_pic()
{
    const int height = 1024, width = 1024;
    const int max_depth = 50;
    const int samples_per_pixel = 20;
    FrameBuffer fb(width, height);

    point origin(0, 0, 1);
    direction horizontal(2.0, 0, 0);
    direction vertical(0, 2.0, 0);
    double focal_length = 2.0;
    point low_left = origin - horizontal / 2 - vertical / 2 - direction(0, 0, focal_length); 

    geometry_list world;

    auto material_ground = make_shared<diffuse>(color(0.0, 0.8, 0.8));
    auto material_center = make_shared<diffuse>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<glossy>(color(0.8, 0.8, 0.8), 0.5);
    auto material_right  = make_shared<specular>(color(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point( 1.0,    0.0, -1.0),   0.5, material_left));

    BVHnode bvh(world);

    for(int i = 0; i < height; ++i)
        for(int j = 0; j < width; ++j)
        {
            color result(0, 0, 0);
            for(int k = 0; k < samples_per_pixel; ++k)
            {
                double u = (j + random_double()) / (width - 1);
                double v = (i + random_double()) / (height - 1);

                ray r(origin, low_left + horizontal * u + vertical * v);
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