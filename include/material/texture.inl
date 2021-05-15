#include "texture.hpp"

color solid_color::get_color(coord uv) const
{
    return c;
}

color checker::get_color(coord uv) const
{
    double weight = 2 * PI * cycle;
    double sines = sin(uv.x * weight) * sin(uv.y * weight);
    
    if(sines < 0) return odd->get_color(uv);
    return even->get_color(uv);
}

imageTex::imageTex(const std::string& imgPath)
{
    nchannel = bytes_per_pixel;

    data = stbi_load(imgPath.c_str(), &width, &height, &nchannel, nchannel);

    if(!data)
    {
        std::cout << "Error: Could not load texture image file '" << imgPath << "'.\n";
        width = height = 0;
    }
}

color imageTex::get_color(coord uv) const
{
    if(data == nullptr) return color(0, 1, 1);

    int x = uv.x * width;
    int y = (1.0 - uv.y) * height;
    
    x = myclamp(x, 0, width - 1);
    y = myclamp(y, 0, height - 1);

    unsigned char* pixel = data + y * height * nchannel + x * nchannel; 
    return color(pixel[0], pixel[1], pixel[2]) / 255.0;
}