#pragma once

#include "raylib.h"
#include "collision_object.h"
#include "interface_all_actors.h"

#include "memory/bump_allocator.h"

namespace game
{
	class Solid : public CollisionObject
	{
	public:
		explicit Solid(IAllActors* all_actors, mem::BumpAllocator& temp_allocator);
		virtual ~Solid() = default;

		void move(float x, float y);

		bool isCollidable() const;

		static constexpr size_t MAXIMUM_ACTOR_SLIDING_SIZE = 16u;
	protected:
		IAllActors* allActors;
		mem::BumpAllocator& tempAllocator;
	private:
		Vector2 remainder;
		bool collidable;
	};
}
