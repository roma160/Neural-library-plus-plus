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
