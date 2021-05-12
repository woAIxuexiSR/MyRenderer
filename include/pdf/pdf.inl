#include "pdf.hpp"

double cosine_pdf::value(const direction& dir) const
{
    double cosine = srm::dot(normal, dir);
    return (cosine <= 0) ? 0 : cosine / pi;
}
    
direction cosine_pdf::generate() const
{
    direction dir = normal + random_sphere_surface();
    if(srm::dot(dir, dir) < srm::EPS) 
        dir = normal;
    return dir.normalize();
}

double geometry_pdf::value(const direction& dir) const
{
    return object->pdf_value(ray(pos, dir));
}

direction geometry_pdf::generate() const
{
    return object->random(pos);
}

double mixture_pdf::value(const direction& dir) const
{
    double ans = 0;
    for(const auto& p : pdf_list)
        ans += p->value(dir);

    int num = pdf_list.size();
    return (num == 0) ? 0 : ans / num;
}

direction mixture_pdf::generate() const
{
    int k = random_int(0, pdf_list.size() - 1);
    return pdf_list[k]->generate();
}