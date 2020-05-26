#include <iostream>
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_vector.h"
#include "stl_list.h"
#include "stl_pair.h"
#include <vector>
int main() {
    f00l_stl::vector<int>v(3,10);
    f00l_stl::list<int>l;
    f00l_stl::pair<int,int>p;
    v.push_back(10);
    v.insert(v.begin()+1,10);
    bool a = v.empty();
    if(a)
        std::cout<<"good"<<std::endl;
    else
        std::cout<<"shit"<<std::endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}