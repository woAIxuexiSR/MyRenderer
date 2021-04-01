#pragma once

namespace srm
{
    template <class T>
    class vec2
    {
    private:
        T x, y;

    public:
        vec2(): x(0), y(0) {}
        vec2(T _x): x(_x), y(_x) {}
        vec2(T _x, T _y): x(_x), y(_y) {}

        T dot(const vec2<T>& _a) const;
    };

    template<class V>
    auto dot(const V& _a, const V& _b);
}

#include "vector.inl"