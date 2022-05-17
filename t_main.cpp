#include <iostream>
#include <string>
#include <deque>

#if 0 //CREATE A REAL STL EXAMPLE
#include <map>
#include <stack>
#include <vector>
namespace ft = std;
#else

#include "map/map.hpp"
#include "stack/stack.hpp"
#include "vector/vector.hpp"

#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer {
    int idx;
    char buff[BUFFER_SIZE];
};


#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T> {
public:
    MutantStack() {}

    MutantStack(const MutantStack<T> &src) { *this = src; }

    MutantStack<T> &operator=(const MutantStack<T> &rhs) {
        this->c = rhs.c;
        return *this;
    }

    ~MutantStack() {}

    typedef typename ft::stack<T>::container_type::iterator iterator;

    iterator begin() { return this->c.begin(); }

    iterator end() { return this->c.end(); }
};

// int main(int argc, char **argv) {
//     if (argc != 2) {
//         std::cerr << "Usage: ./test seed" << std::endl;
//         std::cerr << "Provide a seed please" << std::endl;
//         std::cerr << "Count value:" << COUNT << std::endl;
//         return 1;
//     }
//     const int seed = atoi(argv[1]);
//     srand(seed);

//     ft::vector<std::string> vector_str;
//     ft::vector<int> vector_int;
//     ft::stack<int> stack_int;
//     ft::vector<Buffer> vector_buffer;
//     ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
//     ft::map<int, int> map_int;

//     for (int i = 0; i < COUNT; i++) {
//         vector_buffer.push_back(Buffer());
//     }

//     for (int i = 0; i < COUNT; i++) {
//         const int idx = rand() % COUNT;
//         vector_buffer[idx].idx = 5;
//     }
//     ft::vector<Buffer>().swap(vector_buffer);

//     try {
//         for (int i = 0; i < COUNT; i++) {
//             const int idx = rand() % COUNT;
//             vector_buffer.at(idx);
//             std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" << std::endl;
//         }
//     }
//     catch (const std::exception &e) {
//         //NORMAL ! :P
//     }

//     for (int i = 0; i < COUNT; ++i) {
//         map_int.insert(ft::make_pair(rand(), rand()));
//     }

//     int sum = 0;
//     for (int i = 0; i < 10000; i++) {
//         int access = rand();
//         sum += map_int[access];
//     }
//     std::cout << "should be constant with the same seed: " << sum << std::endl;

//     {
//         ft::map<int, int> copy = map_int;
//     }
//     MutantStack<char> iterable_stack;
//     for (char letter = 'a'; letter <= 'z'; letter++)
//         iterable_stack.push(letter);
//     for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++) {
//         std::cout << *it;
//     }
//     std::cout << std::endl;
//     return (0);
// }
//
//
int main()
{
   ft::map<int, int> map;
   std::string r;
   srand(0);
//     while(1)
//     {
//         std::cin >> r;
//         if (r == "c")
//             break;
//         map.insert(ft::make_pair(std::stoi(r), 0));
//     }
  for (int i = 0; i < 1000000; i++)
  {
      int r = rand();
    //    std::cout << r << std::endl;
      map.insert(ft::make_pair(i, 0));

    //   map.erase(r - 1);

  }
    //   map.erase(2147474126);
    //   map.erase(2147474126);
    //   map.erase(2147474126);
    // map.erase(2147468961);
    // map.erase(2147367579);
    // map.erase(2144612508);


//  map.clear();
// map.clear();
std::cout << " ______________ " << r << std::endl;

for (int i = 0; i < 1000; i++)
{
   int r = rand();
//    std::cout << "check" << std::endl;
   if (map.erase(r) == 1)
   {
    //    std::cout << i << " erase o : " << r << std::endl;
   }
   else
   {
    //    std::cout << i << " erase x : " << r << std::endl;
   }
//    std::cout << "check"
}

// map.clear();

//    while(1)
//    {
//        std::cin >> r;
//        if (r == "c")
//            break;
//        map.erase(std::stoi(r));
//    }

    // map.clear();

//    ft::map<int, int>::iterator it = map.begin();
//     for (; it != map.end(); it++)
//     {
//         std::cout << it->first << std::endl;
//     }
    std::cout << map.size() << std::endl;
}