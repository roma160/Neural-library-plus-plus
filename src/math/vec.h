//
// Created by 1 on 20.03.2021.
//

#ifndef NEURALNETWORKTRY_VEC_H
#define NEURALNETWORKTRY_VEC_H

#include "stringable.h"
#include <initializer_list>
#include <exception>
#include <stdexcept>

template <typename T>
class vec : public stringable {
private:
    T* vec_data;
    int vec_size;
public:
    vec();
    vec(int size);
    vec(std::initializer_list<T> values);
    vec(int size, T* values);
    vec(const vec &to_copy);
    ~vec();

    //Exceptions
    class Exception_VectorsSizesDoesntMatch :
            public std::logic_error{};

    //Operators override
    T& operator[](int i);
    vec& operator=(vec b);
    vec operator+(T b);
    vec operator+(vec b);
    void operator+=(T b);
    void operator+=(vec b);

    std::string to_string() const override;
};

#include "vec.tpp"

#endif //NEURALNETWORKTRY_VEC_H
