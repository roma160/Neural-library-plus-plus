/**
 * Implementation of vec.h
 * */
#include <iostream>
#include <sstream>
#include <cstdio>
#include <exception>
#include "vec.h"

//#define _DEBUG_VEC
#ifdef _DEBUG_VEC
#define DEBUG_MSG(msg) std::cout<<msg<<"\n"
#else
#define DEBUG_MSG(msg)
#endif

template<typename T>
const double vec<T>::resize_coof = 1.5;
template<typename T>
const char* vec<T>::def_tab = "   ";

// Constructors :
template<typename T>
vec<T>::vec()
{
    vec_data = NULL;
    vec_size = 0;
    vec_array_size = 0;
}
template<typename T>
vec<T>::vec(size_t size, bool fill_zeros) {
    DEBUG_MSG("VEC creation call");
    vec_data = new T[size];
    vec_size = size;
    vec_array_size = size;
    if (fill_zeros)
        std::fill(vec_data, vec_data + vec_size, T());
}
template<typename T>
vec<T>::vec(size_t size, T* values) : vec(size, false) {
    std::copy(values, values + size, vec_data);
}
template<typename T>
vec<T>::vec(const std::initializer_list<T> values) :
	vec(values.size(), false) {
    std::copy(values.begin(), values.end(), vec_data);
}

template<typename T>
void _vec_copy(vec<T>* to, const vec<T>* from)
{
    std::copy(from->vec_data,
        from->vec_data + to->vec_size,
        to->vec_data);
}
template<typename T>
void _vec_copy(vec<vec<T>>* to, const vec<vec<T>>* from)
{
    for (size_t i = 0; i < to->vec_size; i++)
        to->vec_data[i] = vec<T>(from->vec_data[i]);
}
template<typename T>
vec<T>::vec(const vec &to_copy) : vec(to_copy.vec_size, false) {
    DEBUG_MSG("VEC.tpp : copy constructor");
    _vec_copy(this, &to_copy);
}

template <typename T>
vec<T>::vec(vec&& to_move) noexcept
{
    DEBUG_MSG("VEC.tpp : move constructor");
    vec_array_size = to_move.vec_array_size;
    vec_size = to_move.vec_size;
    vec_data = to_move.vec_data;

    to_move.vec_data = NULL;
    to_move.vec_array_size = 0;
    to_move.vec_size = 0;
}
template<typename T>
vec<T>::~vec() {
    DEBUG_MSG("VEC.tpp : destructor call");
    delete[] vec_data;
}

template <typename T>
size_t vec<T>::size() const { return vec_size; }


template<typename T>
void _get_shape(T& v, vec<size_t>* res){}
template<typename T>
void _get_shape(vec<T>& v, vec<size_t>* res)
{
    res->push_back(v.vec_size);
    _get_shape(v.vec_data[0], res);
}
template <typename T>
vec<size_t> vec<T>::get_shape() const
{
    vec<size_t> ret({vec_size});
    _get_shape(vec_data[0], &ret);
    return ret;
}

template <typename T>
void vec<T>::resize(size_t new_size)
{
	if(new_size > vec_array_size)
	{
        vec_array_size = new_size * resize_coof;
        T* new_data = new T[vec_array_size];
        std::copy(vec_data, vec_data + vec_size, new_data);
        delete[] vec_data;
        vec_data = new_data;
	}
    vec_size = new_size;
}

template <typename T>
void vec<T>::reverse(int from, int to)
{
    if (to == -1) to = vec_size;
#ifdef _DEBUG_VEC
	if(to < from){
		std::stringstream ss;
    	ss << "You\'ve tried to reverse array with invalid";
    	ss << "from(" << from << ") and to(" << to << ") arguments!";
    	std::cerr << ss.str();
    	throw std::out_of_range(ss.str());
	}
#endif
    std::reverse(vec_data + from, vec_data + to);
}

template <typename T>
void vec<T>::push_back(const T& element)
{
    resize(vec_size + 1);
    vec_data[vec_size - 1] = element;
}

template <typename T>
void vec<T>::push_back(const vec& elements)
{
    int buff = vec_size;
    resize(vec_size + elements.vec_size);
    std::copy(elements.vec_data, 
        elements.vec_data + elements.vec_size,
        vec_data + buff);
}

template<typename T>
void _fill_vec(const vec<vec<T>*>& to_fill, const vec<size_t>* shape, size_t ind)
{
    size_t to_fill_size = to_fill.size(),
        prev_shape = shape->vec_data[ind - 1],
        to_fill_shape = shape->vec_data[ind];
    vec<T>* to_fill_p;
    for (size_t i = 0; i < to_fill_size; i++) {
        to_fill_p = to_fill.vec_data[i];
        for (size_t j = 0; j < prev_shape; j++, to_fill_p++)
            *to_fill_p = vec<T>(to_fill_shape, false);
    }
}
template<typename T>
void _fill_vec(const vec<vec<vec<T>>*>& to_fill, const vec<size_t>* shape, size_t ind)
{
    size_t to_fill_size = to_fill.size(),
        prev_shape = shape->vec_data[ind - 1],
        to_fill_shape = shape->vec_data[ind],
        next_to_fill_ind = 0;
    vec<vec<T>*> next_to_fill(to_fill_size * prev_shape, false);
    vec<vec<T>>* to_fill_p;
    for (size_t i = 0; i < to_fill_size; i++) {
        to_fill_p = to_fill.vec_data[i];
        for (size_t j = 0; j < prev_shape; j++, to_fill_p++) {
            *to_fill_p = vec<vec<T>>(to_fill_shape, false);
            next_to_fill.vec_data[next_to_fill_ind++] = to_fill_p->vec_data;
        }
    }
    if (ind < shape->size())
        _fill_vec(next_to_fill, shape, ind + 1);
}
template <typename T>
vec<T> vec<T>::create_from_shape(const vec<size_t>& shape_vec)
{
    size_t size = shape_vec[0];
    vec<T> ret(size, false);
    vec<T*> to_fill({ret.vec_data});
	if(shape_vec.size() > 1)
		_fill_vec(to_fill, &shape_vec, 1);
    return ret;
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
size_t vec<T>::max_element_ind()
{
    if (!vec_size) return NULL;
    T buff = vec_data[0];
    size_t ret = 0;
    for (int i = 1; i < vec_size; i++)
        if (vec_data[i] > buff) {
            buff = vec_data[i];
            ret = i;
        }
    return ret;
}

template <typename T>
T vec<T>::min_element()
{
    if (!vec_size) return NULL;
    T ret = vec_data[0];
    for (int i = 1; i < vec_size; i++)
        if (vec_data[i] < ret) ret = vec_data[i];
    return ret;
}

template <typename T>
size_t vec<T>::min_element_ind()
{
    if (!vec_size) return NULL;
    T buff = vec_data[0];
    size_t ret = 0;
    for (int i = 1; i < vec_size; i++)
        if (vec_data[i] < buff) {
            buff = vec_data[i];
            ret = i;
        }
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

template <typename T>
T* vec<T>::p() const { return vec_data; }

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
vec<T>& vec<T>::operator=(const vec& b) {
    DEBUG_MSG("The VEC.h copy operator call");
    if (vec_data == b.vec_data)
        return *this;
	
    delete[] vec_data;
    vec_size = b.vec_size;
    vec_array_size = vec_size;
    vec_data = new T[vec_size];
    _vec_copy(this, &b);
    return *this;
}

template <typename T>
vec<T>& vec<T>::operator=(vec&& b) noexcept
{
    DEBUG_MSG("The VEC.h move operator call");
    if (vec_data == b.vec_data)
        return *this;
	
    delete[] vec_data;
    vec_array_size = b.vec_array_size;
    vec_size = b.vec_size;
    vec_data = b.vec_data;

    b.vec_data = NULL;
    b.vec_array_size = 0;
    b.vec_size = 0;
    return *this;
}

template <typename T, typename G>
vec<T> operator+(const vec<T> &a, const G &b)
{
    vec<T> ret(a.vec_size, false);
    for (size_t i = 0; i < ret.vec_size; i++)
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
    for (size_t i = 0; i < smaller->vec_size; i++)
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
        for (size_t i = 0; i < a.vec_size; i++)
            ret.vec_data[i] = a.vec_data[i] + b.vec_data[i];
        std::copy(b.vec_data + a.vec_size,
            b.vec_data + b.vec_size,
            ret.vec_data + a.vec_size);
        a = ret;
    }
    else for (size_t i = 0; i < b.vec_size; i++)
            a.vec_data[i] += b.vec_data[i];
    return a;
}

template <typename T, typename G>
vec<T> operator-(const vec<T> &a, const G &b)
{
    vec<T> ret(a.vec_size, false);
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
	    for (size_t i = 0; i < smaller->vec_size; i++)
	        ret.vec_data[i] = bigger->vec_data[i] - smaller->vec_data[i];
    else {
        for (size_t i = 0; i < smaller->vec_size; i++)
            ret.vec_data[i] = smaller->vec_data[i] - bigger->vec_data[i];
        for (size_t i = smaller->vec_size; i < bigger->vec_size; i++)
            ret.vec_data[i] = -bigger->vec_data[i];
    }
    return ret;
}

template <typename T, typename G>
vec<T> &operator-=(vec<T> &a, const G& b)
{
    for (size_t i = 0; i < a.vec_size; i++)
        a.vec_data[i] -= b;
    return a;
}

template <typename T>
vec<T> &operator-=(vec<T> &a, const vec<T> &b)
{
    if (a.vec_size < b.vec_size)
    {
        vec<T> buff(b.vec_size, false);
        for (size_t i = 0; i < a.vec_size; i++)
            buff.vec_data[i] = a.vec_data[i] - b.vec_data[i];
        for (size_t i = a.vec_size; i < b.vec_size; i++)
            buff.vec_data[i] = -b.vec_data[i];
        a = buff;
    }
    else
        for (size_t i = 0; i < b.vec_size; i++)
            a.vec_data[i] -= b.vec_data[i];
    return a;
}

template <typename T, typename G>
vec<T> operator*(const vec<T>& a, const G& b)
{
    vec<T> ret(a.vec_size, false);
    for (size_t i = 0; i < ret.vec_size; i++)
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
    for (size_t i = 0; i < a.vec_size; i++)
        a.vec_data[i] *= b;
    return a;
}

template <typename T>
vec<T> &operator*=(vec<T> &a, const vec<T> &b)
{
    a.vec_size = std::min(a.vec_size, b.vec_size);
    for (size_t i = 0; i < a.vec_size; i++)
        a.vec_data[i] *= b.vec_data[i];
    return a;
}

template <typename T, typename G>
vec<T> operator/(const vec<T> &a, const G& b)
{
    vec<T> ret(a.vec_size, false);
    for (size_t i = 0; i < ret.vec_size; i++)
        ret.vec_data[i] = a.vec_data[i] / b;
    return ret;
}

template <typename T>
vec<T> operator/(const vec<T> &a, const vec<T> &b)
{
    size_t ret_size = std::min(a.vec_size, b.vec_size);
    vec<T> ret(ret_size, false);
    for (size_t i = 0; i < ret_size; i++)
        ret.vec_data[i] = a.vec_data[i] / b.vec_data[i];
    return ret;
}

template <typename T, typename G>
vec<T> &operator/=(vec<T> &a, const G& b)
{
    for (size_t i = 0; i < a.vec_size; i++)
        a.vec_data[i] /= b;
    return a;
}

template <typename T>
vec<T> &operator/=(vec<T> &a, const vec<T> &b)
{
    a.vec_size = std::min(a.vec_size, b.vec_size);
    for (size_t i = 0; i < a.vec_size; i++)
        a.vec_data[i] /= b.vec_data[i];
    return a;
}

template<typename T>
std::string _vec_to_string(const vec<T>* v, const vec<size_t>* shape = NULL, const std::string* depth = NULL)
{
    std::stringstream ss;
    ss << "v(" << v->vec_size << "){ ";
    if (v->vec_size > 0) ss << v->vec_data[0];
    for (size_t i = 1; i < v->vec_size; i++)
        ss << ", " << v->vec_data[i];
    ss << " }";
    return ss.str();
}
template<typename T>
std::string _vec_to_string(const vec<vec<T>>* v, const vec<size_t>* shape, const std::string* depth)
{
    std::stringstream ss;
    ss << "v(" << v->vec_size << "){ ";
    if (v->vec_size > 0) {
        depth++;
        ss << *depth << _vec_to_string(&v->vec_data[0], shape, depth);
        for (size_t i = 1; i < v->vec_size; i++)
            ss << ',' << *depth <<
				_vec_to_string(&v->vec_data[i], shape, depth);
        depth--;
        ss << *depth << "}";
    }
    else ss << " }";
    return ss.str();
}
template<typename T>
std::string vec<T>::to_string() const {
    vec<size_t> shape = get_shape();
    size_t shape_size = shape.size();
    if (shape_size == 1)
        return _vec_to_string(this);
    std::string* depth_arr = new std::string[shape_size + 1];
    depth_arr[0] = "\n";
    for (size_t i = 1; i <= shape_size; i++)
        depth_arr[i] = depth_arr[i - 1] + def_tab;
    return _vec_to_string(this, &shape, depth_arr);
}

template <typename T>
vec<vec<T>> operator&(vec<T> &a, vec<T> &b)
{
    vec<vec<T>> ret(a.vec_size, false);
	for(size_t a_i = 0; a_i < a.vec_size; a_i++)
	{
        ret.vec_data[a_i] = vec<T>(b.vec_size, false);
        for (size_t b_i = 0; b_i < b.vec_size; b_i++)
            ret.vec_data[a_i].vec_data[b_i] =
            a.vec_data[a_i] * b.vec_data[b_i];
	}
    return ret;
}
