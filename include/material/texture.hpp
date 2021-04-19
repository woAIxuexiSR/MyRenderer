#pragma once

#include "math/vector.hpp"
#include "math/utility.hpp"

class texture
{
public:
    virtual color get_color(double u, double v) const = 0;
};

class solid_color : public texture
{
private:
    color c;

public:
    solid_color() {}
    solid_color(color _c) : c(_c) {}

    virtual color get_color(double u, double v) const override;
};

class checker : public texture
{
private:
    std::shared_ptr<texture> odd;
    std::shared_ptr<texture> even;
    double frequency;

public:
    checker() {}
    checker(const std::shared_ptr<texture>& _o, const std::shared_ptr<texture>& _e, double _f) : odd(_o), even(_e), frequency(_f) {}

    virtual color get_color(double u, double v) const override;
};

#include "texture.inl"