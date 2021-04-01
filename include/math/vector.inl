#include "vector.hpp"

namespace srm
{
    template<class T>
    T vec2<T>::dot(const vec2<T>& a) const
    {
        return x * a.x + y * a.y;
    }
}