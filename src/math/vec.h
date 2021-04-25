//
// Created by 1 on 20.03.2021.
//

#ifndef NEURALNETWORKTRY_VEC_H
#define NEURALNETWORKTRY_VEC_H

#include "stringable.h"
#include <initializer_list>
#include <stdexcept>

//TODO: get rid of the l and r values!
template <typename T>
class vec : public stringable {
private:
    static const double resize_coof;
	
    T* vec_data;
    size_t vec_array_size;
    size_t vec_size;

    template<typename T>
    friend void _get_shape(vec<T>& v, vec<size_t>* res);

    template<typename T>
    friend void _fill_vec(const vec<vec<T>*>& to_fill, const vec<size_t>* shape, size_t ind);
    template<typename T>
    friend void _fill_vec(const vec<vec<vec<T>>*>& to_fill, const vec<size_t>* shape, size_t ind);

    static const char* def_tab;
    template<typename T>
    friend std::string _vec_to_string(const vec<T>* v, const vec<size_t>* shape, const std::string* depth);
    template<typename T>
    friend std::string _vec_to_string(const vec<vec<T>>* v, const vec<size_t>* shape, const std::string* depth);
	
public:
    vec();
    vec(size_t size, bool fill_zeros);
    vec(std::initializer_list<T> values);
    vec(size_t size, T* values);
    vec(const vec &to_copy);
    ~vec();

	//Functions
    size_t size() const;
    vec<size_t> get_shape() const;
	
    void resize(size_t new_size);
    void reverse(int from = 0, int to = -1);
    void push_back(const T& element);
    void push_back(const vec& elements);

	//TODO: create method `create_same_shape`
    /// <summary>
    /// Makes new vector with the shape of shape_vec
    /// </summary>
    static vec create_from_shape(const vec<size_t>& shape_vec);
	
    T max_element();
    size_t max_element_ind();
    T min_element();
    size_t min_element_ind();
    T sum() const;

    vec square();
    vec sqrt();

    /// <summary>
    /// Function to get pointer to the wrapped array.
    /// </summary>
    /// <returns>
    /// Pointer to the first element of the vec.
    /// </returns>
    T* p() const;

    //Operators override
    T& operator[](size_t i);
    T& operator[](size_t i) const;
    vec& operator=(const vec& b);

	//Math operators
    template<typename T>
    friend vec<T> operator+(const vec<T>& a, const vec<T>& b);
    template<typename T, typename G>
    friend vec<T> operator+(const vec<T> &a, const G &b);
    template<typename T>
    friend vec<T>& operator+=(vec<T>& a, const vec<T>& b);
    template<typename T, typename G>
	friend vec<T> &operator+=(vec<T> &a, const G &b);

    template<typename T>
    friend vec<T> operator-(const vec<T>& a, const vec<T>& b);
    template<typename T, typename G>
    friend vec<T> operator-(const vec<T>& a, const G& b);
    template<typename T>
    friend vec<T>& operator-=(vec<T>& a, const vec<T>& b);
    template<typename T, typename G>
    friend vec<T> &operator-=(vec<T>& a, const G& b);

    template<typename T>
    friend vec<T> operator*(const vec<T>& a, const vec<T>& b);
    template<typename T, typename G>
    friend vec<T> operator*(const vec<T>& a, const G& b);
    template<typename T>
    friend vec<T>& operator*=(vec<T>& a, const vec<T>& b);
    template<typename T, typename G>
    friend vec<T> &operator*=(vec<T>& a, const G& b);

    template<typename T>
    friend vec<T> operator/(const vec<T>& a, const vec<T>& b);
    template<typename T, typename G>
    friend vec<T> operator/(const vec<T>& a, const G& b);
    template<typename T>
    friend vec<T>& operator/=(vec<T>& a, const vec<T>& b);
    template<typename T, typename G>
    friend vec<T> &operator/=(vec<T>& a, const G& b);

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
