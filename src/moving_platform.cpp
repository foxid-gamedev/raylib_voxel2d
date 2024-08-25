#include "pch.h"
#include "moving_platform.h"

#include "raymath.h"

game::MovingPlatform::MovingPlatform(
	Vector2 moveDir, 
	float moveSpd, 
	game::IAllActors* all_actors, 
	mem::BumpAllocator& temp_allocator) :

	moveDirection{ moveDir }, moveSpeed{ moveSpd },
	game::Solid(all_actors, temp_allocator)
{
	std::cout << "temp_alloc in moving platform: " << &tempAllocator << " and address from parameter: " << &temp_allocator << "\n";

}

void game::MovingPlatform::update(float deltaTime)
{
	Vector2 velocity = Vector2Normalize(moveDirection);
	velocity.x *= moveSpeed;
	velocity.y *= moveSpeed;

	move(velocity.x * deltaTime, velocity.y * deltaTime);
}

void game::MovingPlatform::render() const
{
	DrawRectangleV(position, {collider.width, collider.height}, ORANGE);
}

void game::MovingPlatform::setMoveDirection(float x, float y)
{
	moveDirection.x = x;
	moveDirection.y = y;
}

void game::MovingPlatform::setMoveDirection(const Vector2& value)
{
	moveDirection = value;
}

Vector2 game::MovingPlatform::getMoveDirection() const
{
	return moveDirection;
}

float game::MovingPlatform::getMoveSpeed() const
{
	return moveSpeed;
}
