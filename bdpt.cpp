#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include "camera/framebuffer.hpp"
#include "camera/camera.hpp"
#include "geometry/geometry.hpp"
#include "geometry/bvhnode.hpp"
#include "material/material.hpp"
#include "pdf/pdf.hpp"
#include "gmm/gmm.hpp"
#include "kdtree/kdTree.hpp"

using std::shared_ptr;
using std::make_shared;
using std::vector;

class vertex
{
public:
    point p;
    color beta;
    double pA;
    direction norm;

    vertex() {}
    vertex(const point& _p, const color& _b, double _pA, const direction& _n) : p(_p), beta(_b), pA(_pA), norm(_n) {}
};

inline color MC_PT(const ray& camera_r, const BVHnode& world, const shared_ptr<geometry>& lights, int depth)
{
    color L(0.0), beta(1.0);
    ray r = camera_r;
    bool specularBounce = true;
    
    for(int i = 0; i < depth; ++i)
    {
        hit_record rec;
        if(!world.hit(r, rec))
            break;
        
        // sampled direction from the last vertex is from a specular BRDF, add emitted term
        if(specularBounce)
            L = L + beta * rec.hit_mat->emitted(rec.uv);

        scatter_record srec;
        if(!rec.hit_mat->scatter(r, rec, srec))
            break;

        if(srec.is_specular)
        {
            beta = beta * srec.attenuation;
            r = srec.specular_ray;
            specularBounce = true;
            continue;
        }

        // sample light
        geometry_pdf gp(rec.p, lights);
        direction out = gp.generate();
        double pdf_val = gp.value(out);
        ray light_ray(rec.p, out);

        hit_record l_rec1, l_rec2;
        if(world.hit(light_ray, l_rec1) && lights->hit(light_ray, l_rec2))
        {
            if((l_rec1.p - l_rec2.p).length_square() < EPS)
                L = L + beta * srec.attenuation * rec.hit_mat->brdf_cos(r, rec, light_ray) * l_rec1.hit_mat->emitted(l_rec1.uv) / pdf_val;
        }

        // sample brdf
        shared_ptr<pdf> bp = srec.brdf_pdf;
        direction o = bp->generate();
        double pv = bp->value(o);
        ray scattered(rec.p, o);

        beta = beta * srec.attenuation * rec.hit_mat->brdf_cos(r, rec, scattered) / pv;
        r = scattered;
        specularBounce = false;

        if(i > 3)
        {
            double RR = 0.05 > 1 - beta.y ? 0.05 : 1 - beta.y;
            if(random_double() < RR)
                break;
            beta = beta / (1 - RR);
        }
    }

    return L;
}

inline color BDPT(const ray& camera_r, const BVHnode& world, const shared_ptr<geometry>& lights, int depth)
{
    vector<vertex> lightPath;
    vector<vertex> cameraPath;
    
    // generate light path
    geometry_pdf gp(point(0, 0, 0), lights);
    direction light_dir = gp.generate();
    double pA = 1.0 / lights->area();

    hit_record l_rec;
    lights->hit(ray(point(0, 0, 0), light_dir), l_rec);
    color beta = l_rec.hit_mat->emitted(l_rec.uv);

    lightPath.push_back(vertex(l_rec.p, beta, pA, l_rec.normal));

    cosine_pdf cp(l_rec.normal);
    light_dir = cp.generate();

    ray light_ray(l_rec.p, light_dir);
    double pw = cp.value(light_dir);
    for(int i = 0; i < depth; ++i)
    {
        hit_record rec;
        if(!world.hit(light_ray, rec))
            break;

        scatter_record srec;
        if(!rec.hit_mat->scatter(light_ray, rec, srec))
            break;

        double distance_square = rec.t * rec.t;
        double cosine = fabs(dot(light_ray.get_dir(), rec.normal));
        pA = pw * cosine / distance_square;

        shared_ptr<pdf> bp = srec.brdf_pdf;
        direction out = bp->generate();
        pw = bp->value(out);
        ray scattered(rec.p, out);

        beta = beta * srec.attenuation * rec.hit_mat->brdf_cos(ray(), rec, ray(rec.p, -light_ray.get_dir()));

        lightPath.push_back(vertex(rec.p, beta, pA, rec.normal));
        
        light_ray = scattered;

        if(i > 3)
        {
            double RR = 0.05 > 1 - beta.y ? 0.05 : 1 - beta.y;
            if(random_double() < RR)
                break;
            beta = beta / (1 - RR);
        }
    }

    double mul_pw = 1.0;
    for(int i = 0; i < (int)lightPath.size() - 1; ++i)
    {
        vertex current = lightPath[i];
        vertex next = lightPath[i + 1];

        double distance_square = (current.p - next.p).length_square();
        double cosine = fabs(dot((current.p - next.p).normalize(), current.norm));

        mul_pw *= current.pA * distance_square / cosine;

        lightPath[i + 1].beta = lightPath[i + 1].beta / mul_pw;
    }

    // generate camera path
    ray r = camera_r;
    beta = color(1.0);
    for(int i = 0; i < depth; ++i)
    {
        hit_record rec;
        if(!world.hit(r, rec))
            break;
        
        scatter_record srec;
        if(!rec.hit_mat->scatter(r, rec, srec))
            break;

        shared_ptr<pdf> bp = srec.brdf_pdf;
        direction o = bp->generate();
        double pv = bp->value(o);
        ray scattered(rec.p, o);

        beta = beta * srec.attenuation;
        r = scattered;

        cameraPath.push_back(vertex(rec.p, beta, pv / rec.hit_mat->brdf_cos(r, rec, scattered), rec.normal));

        if(i > 3)
        {
            double RR = 0.05 > 1 - beta.y ? 0.05 : 1 - beta.y;
            if(random_double() < RR)
                break;
            beta = beta / (1 - RR);
        }
    }

    double mul_pv = 1.0;
    for(int i = 0; i < (int)cameraPath.size() - 1; ++i)
    {
        mul_pv *= cameraPath[i].pA;
        cameraPath[i + 1].beta = cameraPath[i + 1].beta / mul_pv;
    }

    // connect path
    color L(0.0);
    double weight = 0.0;

    int s = lightPath.size(), t = cameraPath.size();

    double w = 1.0;
    for(int j = 0; j < s; ++j)
    {
        vertex li = lightPath[j];
        for(int i = 0; i < t; ++i)
        {
            vertex ca = cameraPath[i];

            ray connect(ca.p, li.p - ca.p);
            hit_record rec;
            if(!world.hit(connect, rec))
                continue;

            if((rec.p - li.p).length_square() > EPS)
                continue;

            double distance_square = rec.t * rec.t;
            double cosine = fabs(dot(connect.get_dir(), li.norm));
            pw = li.pA * distance_square / cosine;

            // std::cout << distance_square << " " << cosine << " " << li.pA << std::endl;
            // std::cout << ca.beta << " " << li.beta << " " << pw << std::endl;

            ray _c(li.p, ca.p - li.p);
            world.hit(_c, rec);

            L = L + ca.beta * li.beta * rec.hit_mat->brdf_cos(ray(), rec, connect) / pw * w;
        }

        weight += w;
        w *= 0.1;
    }

    
    if(weight > 0) 
        return L / weight;
    return L;
}

void cornell_box()
{
    const double aspect_ratio = 1.0;
    const int height = 600, width = height * aspect_ratio;
    const int max_depth = 5;
    const int sample_per_pixel = 10;

    FrameBuffer fb(width, height);
    
    Camera mycamera(point(278, 278, -800), point(278, 278, 0), direction(0, 1, 0), 40, aspect_ratio);

    geometry_list world;

    auto red   = make_shared<diffuse>(color(.65, .05, .05));
    auto white = make_shared<diffuse>(color(.73, .73, .73));
    auto green = make_shared<diffuse>(color(.12, .45, .15));
    auto light_material = make_shared<diffuse_light>(color(15, 15, 15));
    auto aluminum = make_shared<glossy>(color(0.8, 0.85, 0.88), 0.0);
    auto glass = make_shared<dielectric>(1.5);

    world.add(make_shared<yz_rect>(555, 0, 555, 0, 555, green));
    world.add(make_shared<yz_rect>(0, 0, 555, 0, 555, red));
    world.add(make_shared<xz_rect>(0, 0, 555, 0, 555, white));
    world.add(make_shared<xz_rect>(555, 0, 555, 0, 555, white));
    world.add(make_shared<xy_rect>(555, 0, 555, 0, 555, white));

    // shared_ptr<geometry> ball = make_shared<sphere>(point(190, 90, 190), 90, glass);
    // world.add(ball);

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

    shared_ptr<geometry_list> lights = make_shared<geometry_list>();
    lights->add(light);

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
                // color rc = MC_PT(r, bvh, lights, max_depth);
                color rc = BDPT(r, bvh, lights, max_depth);

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