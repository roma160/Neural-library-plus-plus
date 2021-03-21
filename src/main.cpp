#include <iostream>
#include "math/vec.h"

using namespace std;

int main(){
    vec<int> a({ 1, 2, 3 }), b({ 4, 5, 6, 7 });
    vec<int> c(0);
    c = a + b;
    a += b;
    c -= a;
    cout << "a : " << a << "\n";
    cout << "b : " << b << "\n";
    cout << "c : " << c << "\n";
    return 0;
}