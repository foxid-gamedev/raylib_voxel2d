#pragma once

#include "memory/pool_allocator.h"
#include "memory/pointer_array.h"
#include "chunk.h"

namespace game
{
	class World
	{
	public:
		explicit World();
		~World();
	private:
		mem::PoolAllocator<Chunk> poolAllocator;
		mem::PointerArray<Chunk> chunkArray;
	};
}
