#pragma once

#include "math/vector.hpp"
#include "math/utility.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

class texture
{
public:
    virtual color get_color(coord uv) const = 0;
};

class solid_color : public texture
{
private:
    color c;

public:
    solid_color() {}
    solid_color(color _c) : c(_c) {}

    virtual color get_color(coord uv) const override;
};

class checker : public texture
{
private:
    std::shared_ptr<texture> odd;
    std::shared_ptr<texture> even;
    double cycle;   // change times within 0 - 1

public:
    checker() {}
    checker(const color& _o, const color& _e, double _c) : odd(std::make_shared<solid_color>(_o)), even(std::make_shared<solid_color>(_e)), cycle(_c) {}
    checker(const std::shared_ptr<texture>& _o, const std::shared_ptr<texture>& _e, double _c) : odd(_o), even(_e), cycle(_c) {}

    virtual color get_color(coord uv) const override;
};

class imageTex : public texture
{
private:
    unsigned char* data;
    int width, height, nchannel;

public:
    const static int bytes_per_pixel = 3;

    imageTex() : data(nullptr), width(0), height(0), nchannel(0) {};
    imageTex(const std::string& imgPath);
    ~imageTex() { delete data; }

    virtual color get_color(coord uv) const override;
};

#include "texture.inl"