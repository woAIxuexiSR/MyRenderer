#include "vector.hpp"

namespace srm
{
    template<class T>
    T vec2<T>::dot(const vec2<T>& a) const
    {
        return x * a.x + y * a.y;
    }

    template<class V>
    auto dot(const V& _a, const V& _b)
    {
        return _a.dot(_b);
    }
}