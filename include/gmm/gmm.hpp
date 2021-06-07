#pragma once

#include <vector>
#include "math/utility.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"

class Gaussian
{
private:
    vec2<double> miu;
    mat2<double> sigma;

public:
    Gaussian() : miu(0.0, 0.0), sigma(1.0, 0.0, 0.0, 1.0) {}
    Gaussian(const vec2<double>& m, const mat2<double>& s) : miu(m), sigma(s) {}

    bool set_miu(const vec2<double>& m);
    bool set_cov(const mat2<double>& s);
    vec2<double> get_miu() const;
    mat2<double> get_cov() const;
    double pdf(const vec2<double>& x);

    void show() const;
};

class GMM
{
private:
    int ncomponent;
    std::vector<double> weight;
    std::vector<Gaussian> components;

    int index;
    double alpha;
    std::vector<double> ss_sum;         // 1
    std::vector<vec2<double> > ss_miu;  // x
    std::vector<mat2<double> > ss_cov;  // x xT

    static const int max_iter = 100;
    static const int update = 10;

    void Expectation(const std::vector<vec2<double> >& data, bool stepwise);
    bool Maximization();

public:
    GMM() {}
    GMM(int _n, double _a = 0.7) : ncomponent(_n), weight(_n, 1.0 / _n), components(_n), 
                                index(0), alpha(_a), ss_sum(_n), ss_miu(_n), ss_cov(_n) {}

    void offline_trainModel(const std::vector<vec2<double> >& data);
    void online_trainModel(const vec2<double>& data);
    void show() const;
};

// weighted GMM
class WGMM
{
private:
    int ncomponent;
    std::vector<double> weight;
    std::vector<Gaussian> components;

    int index;
    double alpha;
    std::vector<double> ss_w;           // weight
    std::vector<double> ss_sum;         // 1
    std::vector<vec2<double> > ss_miu;  // x
    std::vector<mat2<double> > ss_cov;  // x xT

    static const int max_iter = 100;
    static const int update = 10;

    void Expectation(const std::vector<vec2<double> >& data, const std::vector<double>& w, bool stepwise);
    bool Maximization();

public:
    WGMM() {}
    WGMM(int _n, double _a = 0.7) : ncomponent(_n), weight(_n, 1.0 / _n), components(_n),
                                index(0), alpha(_a), ss_w(_n), ss_sum(_n), ss_miu(_n), ss_cov(_n) {}

    void offline_trainModel(const std::vector<vec2<double> >& data, const std::vector<double>& w);
    void online_trainModel(const vec2<double>& data, double w);
    void show() const;
};

#include "gmm.inl"