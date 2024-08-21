#pragma once

#include "raylib.h"
#include <functional>

#include "interface_collision_check.h"
#include "collision_object.h"

namespace game
{
	class Solid;

	class Actor : public CollisionObject
	{
	public:
		explicit Actor(const ICollisionCheck& collision_check);
		virtual ~Actor() = default;

		void moveX(float amount, std::function<void()> on_collide);
		void moveY(float amount, std::function<void()> on_collide);

		virtual bool isRiding(const Solid& solid) const;
		virtual void squish();
	protected:
		bool isOnFloor;
		bool isOnRoof;
	private:
		Vector2 remainder;
		const ICollisionCheck& collisionCheck;

	};
}
