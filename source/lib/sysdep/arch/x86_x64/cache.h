/* Copyright (c) 2011 Wildfire Games
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

#ifndef INCLUDED_X86_X64_CACHE
#define INCLUDED_X86_X64_CACHE

struct x86_x64_Cache	// POD (may be used before static constructors)
{
	enum Type
	{
		// (values match the CPUID.4 definition)
		kNull,
		kData,
		kInstruction,
		kUnified
		// note: further values are "reserved"
	};

	static const size_t maxLevels = 3;

	static const size_t fullyAssociative = 0xFF;

	void Init()
	{
		type = kNull;
		level = 0;
		associativity = 0;
		entrySize = 0;
		sharedBy = 0;
		numEntries = 0;
	}

	bool Validate() const
	{
		if(type == kNull)
			return true;

		if(!(1 <= level && level <= maxLevels))
			return false;

		if(entrySize == 0)
			return false;

		return true;
	}

	u64 TotalSize() const
	{
		return u64(numEntries)*entrySize;
	}

	/**
	 * if kNull, all other values are invalid.
	 **/
	Type type;

	/**
	 * 1..maxLevels
	 **/
	size_t level;

	/**
	 * = fullyAssociative or the actual ways of associativity
	 **/
	size_t associativity;

	/**
	 * NB: cache entries are lines, TLB entries are pages
	 **/
	size_t entrySize;

	/**
	 * how many logical processors share this cache?
	 **/
	size_t sharedBy;

	size_t numEntries;
};

enum IdxCache
{
	// (AddCache relies upon this order)
	L1D = 1,
	L2D,
	L3D,
	L1I,
	L2I,
	L3I,
	TLB
};

/**
 * @return 0 if idxCache >= TLB+numTLBs, otherwise a valid pointer to
 * a Cache whose type is null if not present.
 **/
LIB_API const x86_x64_Cache* x86_x64_Caches(size_t idxCache);

#endif	// #ifndef INCLUDED_X86_X64_CACHE
