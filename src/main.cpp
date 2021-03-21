#include <iostream>
#include "math/vec.h"

using namespace std;

int main(){
    vec<double> a({ 1, 2, 3 }), b({ 1, 2 });
    auto c = b & a;
    cout << a << " " << b << "\n" << c;
    return 0;
}