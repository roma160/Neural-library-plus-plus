/**
 * Implementation of vec.h
 * */
#include <iostream>
#include <sstream>
#include <cstdio>
#include <exception>
#include "vec.h"

#ifdef _DEBUG_VEC
#define DEBUG_MSG(msg) std::cout<<msg<<"\n"
#else
#define DEBUG_MSG(msg)
#endif

// Constructors :
template<typename T>
vec<T>::vec(size_t size, bool fill_zeros) {
    vec_data = new T[size];
    vec_size = size;
    vec_array_size = size;
    if (fill_zeros)
        std::fill(vec_data, vec_data + vec_size, T());
}
template<typename T>
vec<T>::vec(const std::initializer_list<T> values) :
vec(values.size(), false) {
    std::copy(values.begin(), values.end(), vec_data);
}
template<typename T>
vec<T>::vec(size_t size, T *values) : vec(size, false) {
    std::copy(values, values + size, vec_data);
}

template<typename T>
vec<T>::vec(const vec &to_copy) : vec(to_copy.vec_size, false) {
    DEBUG_MSG("VEC.tpp : copy constructor call");
    std::copy(to_copy.vec_data,
            to_copy.vec_data + vec_size,
            vec_data);
}
template<typename T>
vec<T>::vec()
{
    vec_data = NULL;
    vec_size = 0;
    vec_array_size = 0;
}
template<typename T>
vec<T>::~vec() {
    DEBUG_MSG("VEC.tpp : destructor call");
    delete[] vec_data;
}

template <typename T>
size_t vec<T>::size() const { return vec_size; }

template <typename T>
void vec<T>::resize(size_t new_size)
{
	if(new_size > vec_array_size)
	{
        T* new_data = new T[vec_size];
        std::copy(vec_data, vec_data + vec_size, new_data);
        delete[] vec_data;
        vec_data = new_data;
        vec_array_size = new_size;
	}
    vec_size = new_size;
}

template <typename T>
T vec<T>::max_element()
{
    if (!vec_size) return NULL;
    T ret = vec_data[0];
    for (int i = 1; i < vec_size; i++)
        if (vec_data[i] > ret) ret = vec_data[i];
    return ret;
}

template <typename T>
T vec<T>::sum() const
{
    if (!vec_size) return NULL;
    T ret = vec_data[0];
    for (int i = 1; i < vec_size; i++)
        ret += vec_data[i];
    return ret;
}

template<typename T>
T& vec<T>::operator[](size_t i) {
#ifdef _DEBUG_VEC
    if(i >= vec_size) {
        std::stringstream ss;
        ss<<"You\'ve tried to access "<<i<<"th ";
        ss<<"element of vec with size of "<<vec_size;
        std::cerr << ss.str();
        throw std::out_of_range(ss.str());
    }
#endif
    return vec_data[i];
}

template <typename T>
T& vec<T>::operator[](size_t i) const
{
#ifdef _DEBUG_VEC
    if (i >= vec_size) {
        std::stringstream ss;
        ss << "You\'ve tried to access " << i << "th ";
        ss << "element of vec with size of " << vec_size;
        std::cerr << ss.str();
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

template <typename T, typename G>
vec<T> operator+(const vec<T> &a, const G &b)
{
    vec<T> ret(a.vec_size);
    for (int i = 0; i < ret.vec_size; i++)
        ret.vec_data[i] = a.vec_data[i] + b;
    return ret;
}

template <typename T>
vec<T> operator+(const vec<T> &a, const vec<T> &b)
{
    vec<T> *bigger = (vec<T>*) &a, *smaller = (vec<T>*) &b;
    if (bigger->vec_size < smaller->vec_size)
        std::swap(bigger, smaller);
    vec<T> ret(bigger->vec_size, false);
    for (int i = 0; i < smaller->vec_size; i++)
        ret.vec_data[i] = bigger->vec_data[i] + smaller->vec_data[i];
    std::copy(bigger->vec_data + smaller->vec_size,
        bigger->vec_data + bigger->vec_size,
        ret.vec_data + smaller->vec_size);
    return ret;
}

template <typename T, typename G>
vec<T> &operator+=(vec<T> &a, const G &b)
{
    for (int i = 0; i < a.vec_size; i++)
        a.vec_data[i] += b;
    return a;
}

template <typename T>
vec<T> &operator+=(vec<T> &a, const vec<T> &b)
{
    if (a.vec_size < b.vec_size)
    {
        vec<T> ret(b.vec_size, false);
        for (int i = 0; i < a.vec_size; i++)
            ret.vec_data[i] = a.vec_data[i] + b.vec_data[i];
        std::copy(b.vec_data + a.vec_size,
            b.vec_data + b.vec_size,
            ret.vec_data + a.vec_size);
        a = ret;
    }
    else
        for (int i = 0; i < b.vec_size; i++)
            a.vec_data[i] += b.vec_data[i];
    return a;
}

template <typename T, typename G>
vec<T> operator-(const vec<T> &a, const G &b)
{
    vec<T> ret(a.vec_size);
    for (int i = 0; i < ret.vec_size; i++)
        ret.vec_data[i] = a.vec_data[i] - b;
    return ret;
}

template <typename T>
vec<T> operator-(const vec<T> &a, const vec<T> &b)
{
	vec<T> *bigger = (vec<T>*) &a, *smaller = (vec<T>*) &b;
    bool was_swapped = false;
    if (bigger->vec_size < smaller->vec_size) {
        std::swap(bigger, smaller);
        was_swapped = true;
    }
    vec<T> ret(bigger->vec_size, false);
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

template <typename T, typename G>
vec<T> &operator-=(vec<T> &a, const G& b)
{
    for (int i = 0; i < a.vec_size; i++)
        a.vec_data[i] -= b;
    return a;
}

template <typename T>
vec<T> &operator-=(vec<T> &a, const vec<T> &b)
{
    if (a.vec_size < b.vec_size)
    {
        vec<T> buff(b.vec_size);
        for (int i = 0; i < a.vec_size; i++)
            buff.vec_data[i] = a.vec_data[i] - b.vec_data[i];
        for (int i = a.vec_size; i < b.vec_size; i++)
            buff.vec_data[i] = -b.vec_data[i];
        a = buff;
    }
    else
        for (int i = 0; i < b.vec_size; i++)
            a.vec_data[i] -= b.vec_data[i];
    return a;
}

template <typename T, typename G>
vec<T> operator*(const vec<T>& a, const G& b)
{
    vec<T> ret(a.vec_size, false);
    for (int i = 0; i < ret.vec_size; i++)
        ret.vec_data[i] = a.vec_data[i] * b;
    return ret;
}

template <typename T>
vec<T> operator*(const vec<T>& a, const vec<T>& b)
{
	vec<T> *bigger = (vec<T>*) &a, *smaller = (vec<T>*) &b;
    if (bigger->vec_size < smaller->vec_size)
        std::swap(bigger, smaller);
    vec<T> ret(smaller->vec_size, false);
    for (int i = 0; i < smaller->vec_size; i++)
        ret.vec_data[i] = smaller->vec_data[i] * bigger->vec_data[i];
    return ret;
}

template <typename T, typename G>
vec<T> &operator*=(vec<T> &a, const G& b)
{
    for (int i = 0; i < a.vec_size; i++)
        a.vec_data[i] *= b;
    return a;
}

template <typename T>
vec<T> &operator*=(vec<T> &a, const vec<T> &b)
{
    int new_size = std::min(a.vec_size, b.vec_size);
    for (int i = 0; i < new_size; i++)
        a.vec_data[i] *= b.vec_data[i];
    T* new_data = new T[new_size];
    std::copy(a.vec_data, a.vec_data + new_size, new_data);
    delete[] a.vec_data;
    a.vec_data = new_data;
    return a;
}

template <typename T, typename G>
vec<T> operator/(const vec<T> &a, const G& b)
{
    vec<T> ret(a.vec_size);
    for (int i = 0; i < ret.vec_size; i++)
        ret.vec_data[i] = a.vec_data[i] / b;
    return ret;
}

template <typename T>
vec<T> operator/(const vec<T> &a, const vec<T> &b)
{
    int ret_size = std::min(a.vec_size, b.vec_size);
    vec<T> ret(ret_size);
    for (int i = 0; i < ret_size; i++)
        ret.vec_data[i] = a.vec_data[i] / b.vec_data[i];
    return ret;
}

template <typename T, typename G>
vec<T> &operator/=(vec<T> &a, const G& b)
{
    for (int i = 0; i < a.vec_size; i++)
        a.vec_data[i] /= b;
    return a;
}

template <typename T>
vec<T> &operator/=(vec<T> &a, const vec<T> &b)
{
    int ret_size = std::min(a.vec_size, b.vec_size);
    for (int i = 0; i < ret_size; i++)
        a.vec_data[i] /= b.vec_data[i];
    T* new_data = new T[ret_size];
    std::copy(a.vec_data, a.vec_data + ret_size, new_data);
    delete[] a.vec_data;
    a.vec_data = new_data;
    return a;
}

template<typename T>
std::string vec<T>::to_string() const {
    std::stringstream ss;
    ss<<"v("<<vec_size<<"){ ";
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
    vec<vec<T>> ret(a.vec_size, false);
	for(int a_i = 0; a_i < a.vec_size; a_i++)
	{
        new (ret.vec_data + a_i) vec<T>(b.vec_size, false);
        for (int b_i = 0; b_i < b.vec_size; b_i++)
            ret.vec_data[a_i].vec_data[b_i] =
            a.vec_data[a_i] * b.vec_data[b_i];
	}
    return ret;
}
