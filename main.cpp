#include <iostream>

class p
{
public:
    int t;
public:
    p()
    {
        *this = p(3);
    }
    p(int a): t(a) {};
    p(double a)
    {
        *this = p(4);
    }
};

int main()
{
    p x(3.3);

    std::cout << x.t << std::endl;
    return 0;
}