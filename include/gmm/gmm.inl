#include "gmm.hpp"

// return convergence?
bool Gaussian::set_miu(const vec2<double>& m)
{
    if((miu - m).length_square() < EPS)
        return true;
    miu = m;
    return false;
}

// return convergence?
bool Gaussian::set_cov(const mat2<double>& s)
{
    if(s.determinant() <= 0.0)
        return false;

    mat2<double> p = sigma - s;
    if((p[0][0] * p[0][0] + p[0][1] * p[0][1] + p[1][0] * p[1][0] + p[1][1] * p[1][1]) < EPS)
        return true;
    sigma = s;
    return false;
}

vec2<double> Gaussian::get_miu() const
{
    return miu;
}

mat2<double> Gaussian::get_cov() const
{
    return sigma;
}

void Gaussian::show() const
{
    std::cout << miu << std::endl;
    std::cout << sigma << std::endl;
}

double Gaussian::pdf(const vec2<double>& x)
{
    double sigAbs = sigma.determinant();
    
    vec2<double> x_miu = x - miu;
    mat2<double> inv_sigma = sigma.inverse();
    double t = dot(x_miu, inv_sigma * x_miu);

    return 1.0 / (2 * PI * sqrt(sigAbs)) * exp(-0.5 * t);
}

void GMM::Expectation(const std::vector<vec2<double> >& data, bool stepwise)
{
    int n = data.size();

    std::vector<std::vector<double> > tau(ncomponent);
    for(int j = 0; j < ncomponent; ++j)
    {
        for(int k = 0; k < n; ++k)
            tau[j].push_back(weight[j] * components[j].pdf(data[k]));
    }

    for(int k = 0; k < n; ++k)
    {
        double s = 0.0;
        for(int j = 0; j < ncomponent; ++j)
            s += tau[j][k];

        for(int j = 0; j < ncomponent; ++j)
            tau[j][k] /= s;
    }

    double a = stepwise ? pow(index, -alpha) : 1.0;

    for(int j = 0; j < ncomponent; ++j)
    {
        double sum = 0.0;
        for(int k = 0; k < n; ++k)
            sum += tau[j][k];
        ss_sum[j] = ss_sum[j] * (1.0 - a) + sum / n * a;
    }

    for(int j = 0; j < ncomponent; ++j)
    {
        vec2<double> m(0.0);
        for(int k = 0; k < n; ++k)
            m = m + data[k] * tau[j][k];
        ss_miu[j] = ss_miu[j] * (1.0 - a) + m / n * a;
    }

    for(int j = 0; j < ncomponent; ++j)
    {
        mat2<double> s(0.0, 0.0, 0.0, 0.0);
        for(int k = 0; k < n; ++k)
        {
            vec2<double> d = data[k] - components[j].get_miu();
            s = s + mat2<double>(d.x * d.x, d.x * d.y, d.y * d.x, d.y * d.y) * tau[j][k];
        }
        ss_cov[j] = ss_cov[j] * (1.0 - a) + s / n * a;
    }
}

// return convergence?
bool GMM::Maximization()
{
    bool miu_conv = true, cov_conv = true;
    for(int j = 0; j < ncomponent; ++j)
    {
        weight[j] = ss_sum[j];
        miu_conv &= components[j].set_miu(ss_miu[j] / ss_sum[j]);
        cov_conv &= components[j].set_cov(ss_cov[j] / ss_sum[j]);
    }
    return miu_conv & cov_conv;
}

void GMM::offline_trainModel(const std::vector<vec2<double> >& data)
{
    int n = data.size();

    // init miu
    vec2<double> M(-INF), m(INF);
    for(int k = 0; k < n; ++k)
    {
        M.x = M.x > data[k].x ? M.x : data[k].x;
        M.y = M.y > data[k].y ? M.y : data[k].y;
        m.x = m.x < data[k].x ? m.x : data[k].x;
        m.y = m.y < data[k].y ? m.y : data[k].y;
    }
    for(int j = 0; j < ncomponent; ++j)
    {
        double p = (double)j / ncomponent;
        components[j].set_miu(M * p + m * (1 - p));
    }

    // EM algorithm
    for(int i = 0; i < max_iter; ++i)
    {
        // E step
        Expectation(data, false);

        // M step
        bool convergence = Maximization();

        if(convergence) break;
    }
    index = n;
}

void GMM::online_trainModel(const vec2<double>& data)
{
    // E step
    std::vector<vec2<double> > d {data};
    Expectation(d, true);

    // M step
    index++;

    if(index % update == 0)
        Maximization();
}

void GMM::show() const
{
    std::cout << "There are " << ncomponent << " components.\n" << std::endl;

    for(int i = 0; i < ncomponent; ++i)
    {
        std::cout << "Component " << i << ": weight " << weight[i] << std::endl;
        components[i].show();
    }
}


void WGMM::Expectation(const std::vector<vec2<double> >& data, const std::vector<double>& w, bool stepwise)
{
    int n = data.size();

    std::vector<std::vector<double> > tau(ncomponent);
    for(int j = 0; j < ncomponent; ++j)
    {
        for(int k = 0; k < n; ++k)
            tau[j].push_back(weight[j] * components[j].pdf(data[k]));
    }

    for(int k = 0; k < n; ++k)
    {
        double s = 0.0;
        for(int j = 0; j < ncomponent; ++j)
            s += tau[j][k];

        for(int j = 0; j < ncomponent; ++j)
            tau[j][k] /= s;
    }

    double a = stepwise ? pow(index, -alpha) : 1.0;

    for(int j = 0; j < ncomponent; ++j)
    {
        double ww = 0.0;
        for(int k = 0; k < n; ++k)
            ww += w[k];
        ss_w[j] = ss_w[j] * (1.0 - a) + ww / n * a;
    }

    for(int j = 0; j < ncomponent; ++j)
    {
        double sum = 0.0;
        for(int k = 0; k < n; ++k)
            sum += tau[j][k] * w[k];
        ss_sum[j] = ss_sum[j] * (1.0 - a) + sum / n * a;
    }

    for(int j = 0; j < ncomponent; ++j)
    {
        vec2<double> m(0.0);
        for(int k = 0; k < n; ++k)
            m = m + data[k] * (tau[j][k] * w[k]);
        ss_miu[j] = ss_miu[j] * (1.0 - a) + m / n * a;
    }

    for(int j = 0; j < ncomponent; ++j)
    {
        mat2<double> s(0.0, 0.0, 0.0, 0.0);
        for(int k = 0; k < n; ++k)
        {
            vec2<double> d = data[k] - components[j].get_miu();
            s = s + mat2<double>(d.x * d.x, d.x * d.y, d.y * d.x, d.y * d.y) * (tau[j][k] * w[k]);
        }
        ss_cov[j] = ss_cov[j] * (1.0 - a) + s / n * a;
    }
}

bool WGMM::Maximization()
{
    bool miu_conv = true, cov_conv = true;
    for(int j = 0; j < ncomponent; ++j)
    {
        weight[j] = ss_sum[j] / ss_w[j];
        miu_conv &= components[j].set_miu(ss_miu[j] / ss_sum[j]);
        cov_conv &= components[j].set_cov(ss_cov[j] / ss_sum[j]);
    }
    return miu_conv & cov_conv;
}

void WGMM::offline_trainModel(const std::vector<vec2<double> >& data, const std::vector<double>& w)
{
    int n = data.size();

    // init miu
    vec2<double> M(-INF), m(INF);
    for(int k = 0; k < n; ++k)
    {
        M.x = M.x > data[k].x ? M.x : data[k].x;
        M.y = M.y > data[k].y ? M.y : data[k].y;
        m.x = m.x < data[k].x ? m.x : data[k].x;
        m.y = m.y < data[k].y ? m.y : data[k].y;
    }
    for(int j = 0; j < ncomponent; ++j)
    {
        double p = (double)j / ncomponent;
        components[j].set_miu(M * p + m * (1 - p));
    }

    // EM algorithm
    for(int i = 0; i < max_iter; ++i)
    {
        // E step
        Expectation(data, w, false);

        // M step
        bool convergence = Maximization();

        if(convergence) break;
    }
    index = n;
}

void WGMM::online_trainModel(const vec2<double>& data, double w)
{
    // E step
    std::vector<vec2<double> > d {data};
    std::vector<double> ww {w};
    Expectation(d, ww, true);

    // M step
    index++;

    if(index % update == 0)
        Maximization();
}

void WGMM::show() const
{
    std::cout << "There are " << ncomponent << " components.\n" << std::endl;

    for(int i = 0; i < ncomponent; ++i)
    {
        std::cout << "Component " << i << ": weight " << weight[i] << std::endl;
        components[i].show();
    }
}