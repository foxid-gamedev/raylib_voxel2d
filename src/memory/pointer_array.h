#pragma once

#include "pool_allocator.h"
#include "bump_allocator.h"
#include <cassert>

namespace mem
{
	template<typename T>
	class PointerArray
	{
	public:
		explicit PointerArray(BumpAllocator& allocator, size_t amount) : 
			amount(amount), 
			size(sizeof(T*) * amount)
		{
			data = reinterpret_cast<T**>(allocator.allocate(size));
		}

		template<typename... Args>
		explicit PointerArray(PoolAllocator& allocator, size_t amount, Args&&... args) :
			amount(amount),
			size(sizeof(T*)* amount)
		{
			data = reinterpret_cast<T**>(allocator.allocate<T>(std::forward<Args>(args)));
		}

		PointerArray(const PointerArray& other) noexcept :
			amount(other.amount),
			size(sizeof(T*) * other.amount),
			data(other.data)
		{}

		PointerArray& operator=(const PointerArray& other) noexcept
		{
			if (this != &other)
			{
				data = other.data;
				amount = other.amount;
				size = other.size;
			}
			return *this;
		}

		PointerArray(PointerArray&& other) noexcept
		{
			data = other.data;
			amount = other.amount;
			size = other.size;
			other.data = nullptr;
			other.amount = 0;
			other.size = 0;
		}

		PointerArray& operator=(PointerArray&& other) noexcept
		{
			if (this != &other)
			{
				data = other.data;
				amount = other.amount;
				size = other.size;
				other.data = nullptr;
				other.amount = 0;
				other.size = 0;
			}
			return *this;
		}

		virtual ~PointerArray() 
		{
			amount = 0;
			size = 0;
			data = nullptr;
		}

		T* operator[](size_t index) const 
		{
			assert(index < size);
			return data[index]; 
		}

		void set(T* value, size_t index)
		{
			assert(index < size);
			data[index] = value;
		}

		T** begin() { return data; }
		T** end() { return data + size; }

		const T** cbegin() const { return const_cast<const T**>(data); }
		const T** cend() const { return const_cast<const T**>(data + size); }

		bool contains(const T* value) const
		{
			for (auto current = cbegin(); current < cend(); ++current)
			{
				if (*current == value)
					return true;
			}
			return false;
		}

		size_t getAmount() const { return amount; }
		size_t getSize() const { return size; }

	private:
		size_t amount;
		size_t size;
		T** data;
	};
}
