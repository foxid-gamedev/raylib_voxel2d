#pragma once

#include <raylib.h>
#include "actor.h"

namespace game 
{
	class Player : public Actor
	{
	public:
		explicit Player(const ICollisionCheck& collision_check);
		~Player() = default;

		void update(float deltaTime) override;
		void render() const override;
	private:
		void onCollideX();
		void onCollideY();

		constexpr static float SPEED = 200.f;
		Vector2 velocity;
	};
}
