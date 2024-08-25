#pragma once

#include "bump_allocator.h"

#include <cassert>

namespace mem
{
	template<typename T>
	class PointerArray
	{
	public:
		PointerArray(T** data, size_t size) : data(data), size(size) {}

		T* operator[](size_t index) const 
		{
			assert(index < size);
			return data[index]; 
		}

		size_t size() const { return size; }

	private:
		T** data;
		size_t size;
	};

	//template<typename T>
	//PointerArray<T> create_pointer_array(BumpAllocator& allocator, size_t element_count)
	//{
	//	T** elements = reinterpret_cast<T**>(allocator.allocate(sizeof(T*) * element_count));
	//	for (size_t i = 0; i < element_count; ++i)
	//	{
	//		elements[i] = &
	//	}
	//}
}
