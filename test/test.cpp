#include <iostream>
#include "math/vector.hpp"

using namespace std;

int main()
{
    srm::vec2<int> a(3, 4), b(1, 2);
    cout << srm::dot(a, b) << endl;
    return 0;
}