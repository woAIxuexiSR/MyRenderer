#include <iostream>
#include "math/vector.hpp"

using namespace std;
using namespace srm;

int main()
{
    vec4<int> a(1, 2, 3, 5), b(2, 2, 3, 3), c(-1, 0, 1, 3);
    vec4<int> p = cross(a, b, c);
    cout << p << endl;
    cout << dot(a, p) << dot(b, p) << dot(c, p) << endl;
    return 0;
}