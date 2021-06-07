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