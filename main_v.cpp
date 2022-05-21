#include <iostream>
#include "vector/vector.hpp"
#include <vector>

#include <utility>
int main()
{
    ft::vector<int> v;

    for (int i = 0; i < 100; i++)
    {
        v.insert(v.begin() + i, i);
    }
    v.insert(v.begin(),2);

    for (int i = 0; i < 101; i++)
    {
        std::cout << v[i] << std::endl;
    }
    std::cout << v.capacity() << std::endl;
    return (0);
}
