/**
 * Implementation of vec.h
 * */
#include <iostream>
#include <sstream>
#include <cstdio>
#include <exception>
#include "vec.h"

#ifdef _DEBUG
#define DEBUG_MSG(msg) std::cout<<msg<<"\n"
#else
#define DEBUG_MSG(msg)
#endif

// Constructors :
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
    DEBUG_MSG("VEC.tpp : copy constructor call");
    std::copy(to_copy.vec_data,
            to_copy.vec_data + vec_size,
            vec_data);
}
template<typename T>
vec<T>::vec() : vec(0) {}
template<typename T>
vec<T>::~vec() {
    DEBUG_MSG("VEC.tpp : destructor call");
    delete[] vec_data;
}

template<typename T>
T& vec<T>::operator[](int i) {
#ifdef _DEBUG
    if(i >= vec_size) {
        std::stringstream ss;
        ss<<"You\'ve tried to access "<<i<<"th ";
        ss<<"element of vec with size of "<<vec_size;
        throw std::out_of_range(ss.str());
    }
#endif
    return vec_data[i];
}
template<typename T>
vec<T>& vec<T>::operator=(const vec b) {
    DEBUG_MSG("The VEC.h assigment operator call");
    if (vec_data == b.vec_data)
        return *this;
	
    vec_size = b.vec_size;
    delete[](vec_data);
    vec_data = new T[vec_size];
    std::copy(b.vec_data,
            b.vec_data + vec_size,
            vec_data);
    return *this;
}

template <typename T>
vec<T> vec<T>::operator+(T b) const
{
    vec ret(*this);
    for (int i = 0; i < vec_size; i++)
        ret.vec_data[i] += b;
    return ret;
}

template <typename T>
vec<T> vec<T>::operator+(vec b) const
{
    vec *bigger = (vec*) this, *smaller = &b;
    if (bigger->vec_size < smaller->vec_size)
        std::swap(bigger, smaller);
    vec ret(*bigger);
    for (int i = 0; i < smaller->vec_size; i++)
        ret.vec_data[i] += smaller->vec_data[i];
    return ret;
}

template <typename T>
void vec<T>::operator+=(T b)
{
    for (int i = 0; i < vec_size; i++)
        vec_data[i] += b;
}

template <typename T>
void vec<T>::operator+=(vec b)
{
	vec *bigger = this, *smaller = &b;
    if (bigger->vec_size < smaller->vec_size)
        std::swap(bigger, smaller);
    for (int i = 0; i < smaller->vec_size; i++)
        bigger->vec_data[i] += smaller->vec_data[i];
    *this = *bigger;
}

template <typename T>
vec<T> vec<T>::operator-(T b) const
{
    vec ret(*this);
    for (int i = 0; i < vec_size; i++)
        ret.vec_data[i] -= b;
    return ret;
}

template <typename T>
vec<T> vec<T>::operator-(vec b) const
{
	vec *bigger = this, *smaller = &b;
    bool was_swapped = false;
    if (bigger->vec_size < smaller->vec_size) {
        std::swap(bigger, smaller);
        was_swapped = true;
    }
    vec ret(bigger->vec_size);
	if(!was_swapped)
	    for (int i = 0; i < smaller->vec_size; i++)
	        ret.vec_data[i] = bigger->vec_data[i] - smaller->vec_data[i];
    else {
        for (int i = 0; i < smaller->vec_size; i++)
            ret.vec_data[i] = smaller->vec_data[i] - bigger->vec_data[i];
        for (int i = smaller->vec_size; i < bigger->vec_size; i++)
            ret.vec_data[i] = -bigger->vec_data[i];
    }
    return ret;
}

template <typename T>
void vec<T>::operator-=(T b)
{
    for (int i = 0; i < vec_size; i++)
        vec_data[i] -= b;
}

template <typename T>
void vec<T>::operator-=(vec b)
{
    vec* bigger = (vec*) this, * smaller = &b;
    bool was_swapped = false;
    if (bigger->vec_size < smaller->vec_size) {
        std::swap(bigger, smaller);
        was_swapped = true;
    }
    if (!was_swapped)
        for (int i = 0; i < smaller->vec_size; i++)
            bigger->vec_data[i] -= smaller->vec_data[i];
    else {
        for (int i = 0; i < smaller->vec_size; i++)
            bigger->vec_data[i] = smaller->vec_data[i] - bigger->vec_data[i];
        for (int i = smaller->vec_size; i < bigger->vec_size; i++)
            bigger->vec_data[i] = -bigger->vec_data[i];
    }
    *this = *bigger;
}

template <typename T>
vec<T> vec<T>::operator*(T b) const
{
    vec ret(*this);
    for (int i = 0; i < vec_size; i++)
        ret.vec_data[i] *= b;
    return ret;
}

template <typename T>
vec<T> vec<T>::operator*(vec b) const
{
	vec *bigger = (vec*) this, *smaller = &b;
    if (bigger->vec_size < smaller->vec_size)
        std::swap(bigger, smaller);
    vec ret(*smaller);
    for (int i = 0; i < smaller->vec_size; i++)
        ret.vec_data[i] *= bigger->vec_data[i];
    return ret;
}

template <typename T>
void vec<T>::operator*=(T b)
{
    for (int i = 0; i < vec_size; i++)
        vec_data[i] *= b;
}

template <typename T>
void vec<T>::operator*=(vec b)
{
    vec* bigger = this, * smaller = &b;
    if (bigger->vec_size < smaller->vec_size)
        std::swap(bigger, smaller);
    for (int i = 0; i < smaller->vec_size; i++)
        smaller->vec_data[i] *= bigger->vec_data[i];
    *this = *smaller;
}

template <typename T>
vec<T> vec<T>::operator/(T b) const
{
    vec ret(*this);
    for (int i = 0; i < vec_size; i++)
        ret.vec_data[i] /= b;
    return ret;
}

template <typename T>
vec<T> vec<T>::operator/(vec b) const
{
    int ret_size = std::min(vec_size, b.vec_size);
    vec ret(ret_size);
    std::copy(vec_data, vec_data + ret_size, ret.vec_data);
    for (int i = 0; i < ret_size; i++)
        ret.vec_data[i] /= b.vec_data[i];
    return ret;
}

template <typename T>
void vec<T>::operator/=(T b)
{
    for (int i = 0; i < vec_size; i++)
        vec_data[i] /= b;
}

template <typename T>
void vec<T>::operator/=(vec b)
{
    int ret_size = std::min(vec_size, b.vec_size);
    for (int i = 0; i < ret_size; i++)
        vec_data[i] /= b.vec_data[i];
    T* new_data = new T[ret_size];
    std::copy(vec_data, vec_data + ret_size, new_data);
    delete[] vec_data;
    vec_data = new_data;
}

template<typename T>
std::string vec<T>::to_string() const {
    std::stringstream ss;
    ss<<"vec["<<vec_size<<"]{ ";
    if(vec_size >= 1)
        ss<<vec_data[0];
    for(int i = 1; i < vec_size; i++)
        ss<<", "<<vec_data[i];
    ss<<" }";
    return ss.str();
}

template <typename T>
vec<vec<T>> operator&(vec<T> &a, vec<T> &b)
{
    vec<vec<T>> ret(a.vec_size);
	for(int a_i = 0; a_i < a.vec_size; a_i++)
	{
        ret.vec_data[a_i] = vec<T>(b.vec_size);
        for (int b_i = 0; b_i < b.vec_size; b_i++)
            ret.vec_data[a_i].vec_data[b_i] =
            a.vec_data[a_i] * b.vec_data[b_i];
	}
    return ret;
}
