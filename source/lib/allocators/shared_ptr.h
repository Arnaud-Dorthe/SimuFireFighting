/* Copyright (c) 2010 Wildfire Games
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef INCLUDED_SHARED_PTR
#define INCLUDED_SHARED_PTR

#include "lib/sysdep/arch/x86_x64/cache.h"
#include "lib/sysdep/rtl.h" // rtl_AllocateAligned

struct DummyDeleter
{
	template<class T>
	void operator()(T*)
	{
	}
};

template<class T>
inline shared_ptr<T> DummySharedPtr(T* ptr)
{
	return shared_ptr<T>(ptr, DummyDeleter());
}

struct ArrayDeleter
{
	template<class T>
	void operator()(T* p)
	{
		delete[] p;
	}
};

struct FreeDeleter
{
	template<class T>
	void operator()(T* p)
	{
		free(p);
	}
};

// (note: uses CheckedArrayDeleter)
LIB_API shared_ptr<u8> Allocate(size_t size);

struct AlignedDeleter
{
	template<class T>
	void operator()(T* t)
	{
		rtl_FreeAligned(t);
	}
};

template<class T>
inline shared_ptr<T> AllocateAligned(size_t size)
{
	return shared_ptr<T>((T*)rtl_AllocateAligned(size, x86_x64_Caches(L2D)->entrySize), AlignedDeleter());
}

#endif	// #ifndef INCLUDED_SHARED_PTR
