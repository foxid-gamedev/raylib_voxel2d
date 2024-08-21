#pragma once

#include "raylib.h"
#include "collision_object.h"
#include "interface_all_actors.h"

namespace game
{
	class Solid : public CollisionObject
	{
	public:
		explicit Solid(IAllActors* all_actors);
		virtual ~Solid() = default;

		void move(float x, float y);

		bool isCollidable() const;
	private:
		Vector2 remainder;
		bool collidable;
		IAllActors* allActors;
	};
}
