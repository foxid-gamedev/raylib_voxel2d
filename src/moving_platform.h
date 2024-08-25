#pragma once

#include "solid.h"

namespace game
{
	class MovingPlatform : public Solid
	{
	public:
		MovingPlatform(Vector2 moveDir, float moveSpd, IAllActors* all_actors, mem::BumpAllocator& temp_allocator);
		virtual ~MovingPlatform() = default;

		void update(float deltaTime) override;
		void render() const override;

		void setMoveDirection(float x, float y);
		void setMoveDirection(const Vector2&);
		Vector2 getMoveDirection() const;
		float getMoveSpeed() const;
	private:
		Vector2 moveDirection;
		float moveSpeed;
	};
}
