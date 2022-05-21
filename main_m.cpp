#include <iostream>
#include "vector/vector.hpp"


int main()
{
    ft::vector<int> v(10);

    for (int i = 0; i < 11; i++)
    {
        std::cout << v[i] << std::endl;
    }
    return (0);
}