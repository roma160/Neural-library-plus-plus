//
// Created by 1 on 20.03.2021.
//

#ifndef NEURALNETWORKTRY_VEC_H
#define NEURALNETWORKTRY_VEC_H

#include "stringable.h"
#include <initializer_list>
#include <stdexcept>

template <typename T>
class vec : public stringable {
private:
    T* vec_data;
    size_t vec_array_size;
    size_t vec_size;

public:
    vec();
    vec(size_t size);
    vec(std::initializer_list<T> values);
    vec(size_t size, T* values);
    vec(const vec &to_copy);
    ~vec();

	//Functions
    size_t size() const;
    void resize(size_t new_size);

    T max_element();
    T sum() const;

    //Operators override
    T& operator[](int i);
    T operator[](int i) const;
    vec& operator=(vec b);

	//Operators
    template<typename T>
    friend vec<T> operator+(const vec<T> &a, const T &b);
    template<typename T>
    friend vec<T> operator+(const vec<T> &a, const vec<T> &b);
    template<typename T>
	friend vec<T> &operator+=(vec<T> &a, const T &b);
    template<typename T>
    friend vec<T> &operator+=(vec<T> &a, const vec<T> &b);

    template<typename T>
    friend vec<T> operator-(const vec<T>& a, const T& b);
    template<typename T>
    friend vec<T> operator-(const vec<T>& a, const vec<T>& b);
    template<typename T>
    friend vec<T> &operator-=(vec<T>& a, const T& b);
    template<typename T>
    friend vec<T> &operator-=(vec<T>& a, const vec<T>& b);

    template<typename T>
    friend vec<T> operator*(const vec<T>& a, const T& b);
    template<typename T>
    friend vec<T> operator*(const vec<T>& a, const vec<T>& b);
    template<typename T>
    friend vec<T> &operator*=(vec<T>& a, const T& b);
    template<typename T>
    friend vec<T> &operator*=(vec<T>& a, const vec<T>& b);

    template<typename T>
    friend vec<T> operator/(const vec<T>& a, const T& b);
    template<typename T>
    friend vec<T> operator/(const vec<T>& a, const vec<T>& b);
    template<typename T>
    friend vec<T> &operator/=(vec<T>& a, const T& b);
    template<typename T>
    friend vec<T> &operator/=(vec<T>& a, const vec<T>& b);

    /// <summary>
    /// Multiplies two 1D arrays and returns 2D array
    /// </summary>
    /// <example>
    /// [1] & [3, 4] = [3, 4]
    /// [2]            [6, 8]
    /// </example>
    /// <returns>
    /// 2D multiplication result
    /// res[a_index][b_index]
    /// </returns>
    template<typename T>
    friend vec<vec<T>> operator&(vec<T> &a, vec<T> &b);

    std::string to_string() const override;
};

#include "vec.tpp"

#endif //NEURALNETWORKTRY_VEC_H
