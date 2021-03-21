#include <iostream>
#include "math/vec.h"

using namespace std;

int main(){
    //std::initializer_list<int> initializerList{};
    unsigned long arr[1000];
    for(unsigned long & i : arr)
        i = 1;
    vec<unsigned long> vector(1000, arr);
    std::cout<<vector<<"\n";
    vec<unsigned long> t(0);
    t = vector + vector;
    cout<<t;
    return 0;
}