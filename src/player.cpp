#include "pch.h"

#include "player.h"

game::Player::Player(const ICollisionCheck& collision_check)
	: game::Actor(collision_check), velocity{ 0 }
{
	collider = Rectangle{
		0,0,
		16,16
	};
}

void game::Player::update(float deltaTime)
{
	// apply gravity
	velocity.y += 981.f * deltaTime;
	
	if (isOnFloor && IsKeyDown(KEY_UP))
		velocity.y = -SPEED * 1.5f;

	velocity.y = std::min(velocity.y, 600.f);

	moveY(velocity.y * deltaTime, std::bind(&Player::onCollideY, this));

	if (IsKeyDown(KEY_LEFT))
		velocity.x = -SPEED;
	else if (IsKeyDown(KEY_RIGHT))
		velocity.x = SPEED;
	else
		velocity.x = 0.f;

	moveX(velocity.x * deltaTime, std::bind(&Player::onCollideX, this));

}

void game::Player::render() const
{
	DrawRectangleV(position, { 16.f,16.f }, BLUE);
}

void game::Player::onCollideX()
{
	velocity.x = 0;
}

void game::Player::onCollideY()
{
	velocity.y = 0;
}
