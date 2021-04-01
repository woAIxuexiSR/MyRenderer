#include <iostream>
#include "math/vector.hpp"

using namespace std;
using namespace srm;

int main()
{
    vec2 a(5, 0), b(100, 1);
    cout << cross(a, b) << endl;
    return 0;
}