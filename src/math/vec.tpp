/**
 * Implementation of vec.h
 * */
#include <iostream>
#include <sstream>
#include <cstdio>
#include "vec.h"

template<typename T>
vec<T>::vec(int size) {
    vec_data = new T[size];
    vec_size = size;
}
template<typename T>
vec<T>::vec(const std::initializer_list<T> values) :
vec(values.size()) {
    std::copy(values.begin(), values.end(), vec_data);
}
template<typename T>
vec<T>::vec(int size, T *values) : vec(size) {
    std::copy(values, values + size, vec_data);
}
template<typename T>
vec<T>::vec(const vec &to_copy) : vec(to_copy.vec_size) {
    //std::cout<<to_copy.vec_data<<"\n";
    std::copy(to_copy.vec_data,
            to_copy.vec_data + vec_size,
            vec_data);
}
template<typename T>
vec<T>::vec() : vec(0) {}
template<typename T>
vec<T>::~vec() {
    delete[] vec_data;
}

template<typename T>
T& vec<T>::operator[](int i) {
    if(i >= vec_size) {
        std::stringstream ss;
        ss<<"You\'ve tried to access "<<i<<"th ";
        ss<<"element of vec with size of "<<vec_size;
        throw std::out_of_range(ss.str());
    }
    return vec_data[i];
}
template<typename T>
vec<T>& vec<T>::operator=(const vec b) {
    delete[](vec_data);
    vec_size = b.vec_size;
    std::copy(b.vec_data,
            b.vec_data + vec_size,
            vec_data);
    return *this;
}

template<typename T>
std::string vec<T>::to_string() const {
    std::stringstream ss;
    ss<<"vec["<<vec_size<<"]{ ";
    if(vec_size >= 1)
        ss<<vec_data[0];
    for(int i = 1; i < vec_size; i++)
        ss<<", "<<vec_data[i];
    ss<<" };";
    return ss.str();
}

template<typename T>
void vec<T>::operator+=(T b) {
    for(int i = 0; i < vec_size; i++)
        vec_data[i] += b;
}
template<typename T>
vec<T> vec<T>::operator+(T b) {
    vec<T> ret(*this);
    for(int i = 0; i < vec_size; i++)
        ret.vec_data[i] += b;
    return ret;
}

template<typename T>
vec<T> vec<T>::operator+(vec b) {
    vec *bigger = this, *smaller = &b;
    if(bigger->vec_size < smaller->vec_size)
        std::swap(bigger, smaller);
    vec ret(*bigger);
    for(int i = 0; i < smaller->vec_size; i++)
        ret.vec_data[i] += smaller->vec_data[i];
    return ret;
}
template<typename T>
void vec<T>::operator+=(vec b) {
    vec *bigger = this, *smaller = &b;
    if(bigger->vec_size < smaller->vec_size)
        std::swap(bigger, smaller);
    for(int i = 0; i < smaller->vec_size; i++)
        bigger->vec_data[i] += smaller->vec_data[i];
    if(bigger != this)
        *this = *bigger;
}
