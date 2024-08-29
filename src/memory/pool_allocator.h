#pragma once

#include <utility>
#include <cassert>

namespace mem
{
	template<typename T>
	union PoolChunk
	{
		T value;
		PoolChunk* next;

		PoolChunk() {}
		~PoolChunk() {}
	};

	template<typename T>
	class PoolAllocator
	{
	public:
		explicit PoolAllocator(size_t amount) :
			amount(amount),
			size(sizeof(T) * amount),
			data(nullptr),
			head(nullptr)
		{
			data = new PoolChunk<T>[amount];
			head = data;

			for (size_t i = 0; i < amount - 1; ++i)
			{
				data[i].next = &(data[i + 1]);
			}
			data[amount - 1].next = nullptr;
		}

		~PoolAllocator() 
		{
			delete[] data;
			data = nullptr;
			head = nullptr;
		}

		template <typename... Args>
		T* allocate(Args&&... args)
		{
			if (head == nullptr)
			{
				return nullptr;
			}

			PoolChunk<T>* chunk = head;
			head = head->next;

			T* retVal = new (&(chunk->value)) T(std::forward<Args>(args));
			return retVal;
		}

		void deallocate(T* data)
		{
			data->~T();
			PoolChunk<T>* poolChunk = reinterpret_cast<PoolChunk<T>*>(data);
		}
	private:
		size_t amount;
		size_t size;
		
		PoolChunk<T>* data;
		PoolChunk<T>* head;
	};
}
