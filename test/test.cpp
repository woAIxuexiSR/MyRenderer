#include <iostream>
#include <time.h>
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "camera/framebuffer.hpp"
#include "camera/camera.hpp"
#include "geometry/geometry.hpp"
#include "material/material.hpp"
#include "geometry/bvhnode.hpp"
#include "kdtree/kdTree.hpp"
#include "gmm/gmm.hpp"

using namespace std;

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

void GMM_test()
{
    GMM g(4);

    std::default_random_engine e;
	std::normal_distribution<double> nx(0, 1);
    std::normal_distribution<double> ny(0, 1);

    mat2<double> A(1.1, 2, 0.3, 1.4);
    mat2<double> B(1.45, -0.3, -1.4, 1.7);
    mat2<double> C(2.1, 0.4, 1.1, -0.5);

    vec2<double> muA(2, 1), muB(-1, 3), muC(5, 5);
    mat2<double> sigmaA = A * A.transpose();
    mat2<double> sigmaB = B * B.transpose();
    mat2<double> sigmaC = C * C.transpose();

    std::vector<vec2<double> > data;
    for(int i = 0; i < 10; ++i)
    {
        vec2<double> x(nx(e), ny(e));
        data.push_back(A * x + muA);
    }
    for(int i = 0; i < 20; ++i)
    {
        vec2<double> y(nx(e), ny(e));
        data.push_back(B * y + muB);
    }
    for(int i = 0; i < 20; ++i)
    {
        vec2<double> z(nx(e), ny(e));
        data.push_back(C * z + muC);
    }


    g.offline_trainModel(data);
    g.show();

    for(int i = 0; i < 100000; ++i)
    {
        double x = random_double();
        if(x < 0.2)
            g.online_trainModel(A * vec2<double>(nx(e), ny(e)) + muA);
        else if(x < 0.6)
            g.online_trainModel(B * vec2<double>(nx(e), ny(e)) + muB);
        else
            g.online_trainModel(C * vec2<double>(nx(e), ny(e)) + muC);
    }

    g.show();
    
    std::cout << muA << std::endl << sigmaA << std::endl
             << muB << std::endl << sigmaB << std::endl
             << muC << std::endl << sigmaC << std::endl;
}

void WGMM_test()
{
    WGMM g(3);

    std::default_random_engine e;
	// std::normal_distribution<double> nx(0, 1);
    // std::normal_distribution<double> ny(0, 1);

    std::uniform_real_distribution<double> nx(-1, 1);
    std::uniform_real_distribution<double> ny(-1, 1);

    mat2<double> A(1.1, 2, 0.3, 1.4);
    mat2<double> B(1.45, -0.3, -1.4, 1.7);
    mat2<double> C(2.1, 0.4, 1.1, -0.5);

    vec2<double> muA(2, 5), muB(-5, -3), muC(10, 10);
    mat2<double> sigmaA = A * A.transpose();
    mat2<double> sigmaB = B * B.transpose();
    mat2<double> sigmaC = C * C.transpose();

    Gaussian a(muA, sigmaA);
    Gaussian b(muB, sigmaB);
    Gaussian c(muC, sigmaC);

    std::vector<vec2<double> > data;
    std::vector<double> w;
    for(int i = 0; i < 10; ++i)
    {
        vec2<double> x(nx(e), ny(e));
        vec2<double> xx = A * x + muA;
        data.push_back(xx);
        w.push_back(5 * a.pdf(xx));
    }
    for(int i = 0; i < 20; ++i)
    {
        vec2<double> y(nx(e), ny(e));
        vec2<double> yy = B * y + muB;
        data.push_back(yy);
        w.push_back(5 * b.pdf(yy));
    }
    for(int i = 0; i < 20; ++i)
    {
        vec2<double> z(nx(e), ny(e));
        vec2<double> zz = C * z + muC;
        data.push_back(zz);
        w.push_back(5 * c.pdf(zz));
    }

    g.offline_trainModel(data, w);
    g.show();

    for(int i = 0; i < 100000; ++i)
    {
        double x = random_double();
        if(x < 0.2)
        {
            vec2<double> xx = A * vec2<double>(nx(e), ny(e)) + muA;
            g.online_trainModel(xx, 5 * a.pdf(xx));
        }
        else if(x < 0.6)
        {
            vec2<double> xx = B * vec2<double>(nx(e), ny(e)) + muB;
            g.online_trainModel(xx, 5 * b.pdf(xx));
        }
        else
        {
            vec2<double> xx = C * vec2<double>(nx(e), ny(e)) + muC;
            g.online_trainModel(xx, 5 * c.pdf(xx));
        }
    }
    g.show();
    
    std::cout << muA << std::endl << sigmaA << std::endl
             << muB << std::endl << sigmaB << std::endl
             << muC << std::endl << sigmaC << std::endl;
}

void kdtree_test()
{
    point p(-1, -5, 0.0);

    std::vector<point> data {
        point(6.27, 5.50, 0.0),
        point(1.24, -2.86, 0.0),
        point(-6.88, -5.4, 0.0),
        point(-4.6, -10.55, 0.0),
        point(-2.96, -0.5, 0.0),
        point(-4.96, 12.61, 0.0),
        point(1.75, 12.26, 0.0),
        point(17.05, -12.79, 0.0),
        point(7.75, -22.68, 0.0),
        point(15.31, -13.16, 0.0),
        point(10.8, -5.03, 0.0),
        point(7.83, 15.7, 0.0),
        point(14.63, -0.35, 0.0)
    };

    // kdTree* tree = new kdTree(data);
    kdTree* tree = new kdTree(data[0], AXIS::AXIS_X);
    for(int i = 1; i < data.size(); ++i)
        tree->insert(data[i]);

    auto out = tree->knn(3, p);

    for(int i = 0; i < out.size(); ++i)
        std::cout << out[i] << std::endl;

    delete tree;
}

void kdtree_test2()
{
    std::vector<point> p;

    for(int i = 0; i < 1000000; ++i)
        p.push_back(point(random_double(0, 1), random_double(0, 1), random_double(0, 1)) * 10);

    //kdTree tree(p);
    kdTree tree(p[0], AXIS::AXIS_X);
    for(int i = 1; i < p.size(); ++i)
        tree.insert(p[i]);

    auto out = tree.knn(5, point(7, 2, 1));

    for(int i = 0; i < out.size(); ++i)
        std::cout << out[i] << " ";
    std::cout << std::endl;
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
                color rc;
                result = result + rc;
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
                color rc;
                // color rc = ray_color(r, bvh, max_depth);
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
    // auto iso = make_shared<isotropic>(color(0.2, 0.4, 0.9));
    // world.add(make_shared<constant_medium>(boundary, 0.2, iso));
    
    boundary = make_shared<sphere>(point(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    // iso = make_shared<isotropic>(color(1, 1, 1));
    // world.add(make_shared<constant_medium>(boundary, .0001, iso));      // There is a problem!!!

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
                color rc;
                // color rc = ray_color(r, bvh, max_depth);
                result = result + rc;
            }
            
            fb.set_pixel(i, j, result / sample_per_pixel);
        }

    fb.output("../images/test.ppm");
}

int main()
{
    clock_t start = clock();
    //math_test();
    //framebuffer_test();
    //geometry_test();
    // GMM_test();
    // WGMM_test();
    // kdtree_test();
    kdtree_test2();

    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
    return 0;
}