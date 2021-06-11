#pragma once

#include "pch.h"

#include "math/vec.h"

using namespace System;
using namespace Runtime::InteropServices;

template<typename T>
cli::array<T>^ vec_to_array(vec<T>* v)
{
	size_t length = v->size();
	cli::array<T>^ ret = gcnew cli::array<T>(length);
	Marshal::Copy(IntPtr(v->p()), ret, 0, length);
	return ret;
}

template<typename T>
vec<T> array_to_vec(cli::array<T>^ arr)
{
	vec<T> ret(arr->Length, false);
	Marshal::Copy(arr, 0, IntPtr(ret.p()), arr->Length);
	return ret;
}