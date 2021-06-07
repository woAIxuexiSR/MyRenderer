#pragma once

#include "math/utility.hpp"
#include "math/matrix.hpp"
#include "geometry/geometry.hpp"
#include "gmm/gmm.hpp"

class pdf
{
public:
    virtual double value(const direction& dir) const = 0;
    virtual direction generate() const = 0;
};



class cosine_pdf : public pdf
{
private:
    direction normal;

public:
    cosine_pdf() {}
    cosine_pdf(direction _n) : normal(_n.normalize()) {}

    virtual double value(const direction& dir) const override;
    virtual direction generate() const override;
};



class geometry_pdf : public pdf
{
private:
    point pos;
    std::shared_ptr<geometry> object;

public:
    geometry_pdf() {}
    geometry_pdf(const point& _p, std::shared_ptr<geometry> _o) : pos(_p), object(_o) {}

    virtual double value(const direction& dir) const override;
    virtual direction generate() const override;
};



class mixture_pdf : public pdf
{
private:
    std::vector<std::shared_ptr<pdf> > pdf_list;

public:
    mixture_pdf() {}
    void add(std::shared_ptr<pdf> _p) { pdf_list.push_back(_p); }

    virtual double value(const direction& dir) const override;
    virtual direction generate() const override;
};



class gmm_pdf : public pdf
{
private:
    GMM model;

public:
    gmm_pdf() {}

    virtual double value(const direction& dir) const override;
    virtual direction generate() const override;
};

#include "pdf.inl"