#include "../pch.h"

#include "bump_allocator.h"

mem::BumpAllocator::BumpAllocator(size_t capacity) :
    capacity(capacity),
    size(0)
{
    data = static_cast<uint8_t*>(malloc(capacity));
}

mem::BumpAllocator::~BumpAllocator()
{
    if (data != nullptr) free(data);
    data = nullptr;
    size = 0;
}

uint8_t* mem::BumpAllocator::allocate(size_t amount)
{
    assert(size + amount < capacity);
    auto p = data + size;
    size += amount;
    return p;
}

void mem::BumpAllocator::reset(bool clear_memory)
{
    if (clear_memory)
        memset(data, 0, capacity);

    size = 0;
}

size_t mem::BumpAllocator::getCapacity() const
{
    return capacity;
}

size_t mem::BumpAllocator::getSize() const
{
    return size;
}
