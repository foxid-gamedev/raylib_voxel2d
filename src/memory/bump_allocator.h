#pragma once

#include <cstdint>

namespace mem
{
	class BumpAllocator
	{
	public:
		explicit BumpAllocator(size_t capacity);
		~BumpAllocator();

		uint8_t* allocate(size_t amount);

		void reset(bool clear_memory = false);

		size_t getCapacity() const;
		size_t getSize() const;
	private:
		size_t capacity;
		size_t size;
		uint8_t* data;
	};
}
