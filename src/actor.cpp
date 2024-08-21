#include "pch.h"
#include "actor.h"

#include "solid.h"

game::Actor::Actor(const ICollisionCheck& collision_check) :	
	remainder{ 0 }, 
	collisionCheck{ collision_check },
	isOnFloor{ false }, 
	isOnRoof{ false }
{

}

void game::Actor::moveX(float amount, std::function<void()> on_collide)
{
	remainder.x += amount;
	int move = static_cast<int>(std::roundf(remainder.x));

	if (move != 0)
	{
		remainder.x -= static_cast<float>(move);
		int sign = util::sign(move);

		while (move != 0)
		{
			if (!collisionCheck.checkCollision(position.x + sign, position.y, collider.width, collider.height))
			{
				position.x += static_cast<float>(sign);
				move -= sign;
				
			}
			else
			{
				on_collide();
				break;
			}
		}
	}
}

void game::Actor::moveY(float amount, std::function<void()> on_collide)
{
	remainder.y += amount;
	int move = static_cast<int>(std::roundf(remainder.y));

	if (move != 0)
	{
		remainder.y -= static_cast<float>(move);
		int sign = util::sign(move);

		while (move != 0)
		{
			if (!collisionCheck.checkCollision(position.x, position.y + sign, collider.width, collider.height))
			{
				position.y += static_cast<float>(sign);
				move -= sign;
				
				if (sign < 0)
					isOnFloor = false;
				else
					isOnRoof = false;
			}
			else
			{
				if (sign >= 0)
					isOnFloor = true;
				else
					isOnRoof = true;
				on_collide();
				break;
			}
		}
	}
}

bool game::Actor::isRiding(const game::Solid& solid) const
{
	const Rectangle& other = solid.getCollider();

	bool&& is_above_x = (collider.x + collider.width) > other.x && collider.x < (other.x + other.width);
	bool&& is_above_y = (collider.y + collider.height) >= other.y && (collider.y + collider.height) <= (other.y + other.height);

	return is_above_x && is_above_y;
}

void game::Actor::squish()
{
	// do nothing (override to handle behaviour)
}
