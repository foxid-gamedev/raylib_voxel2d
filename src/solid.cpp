#include "pch.h"
#include "solid.h"

game::Solid::Solid(IAllActors* all_actors, mem::BumpAllocator& temp_allocator)
	: remainder{ 0 }, collidable{ true }, allActors{ all_actors }, tempAllocator{ temp_allocator }
{
}

void game::Solid::move(float x, float y)
{
	remainder.x += x;
	remainder.y += y;

	int moveX = static_cast<int>(std::roundf(remainder.x));
	int moveY = static_cast<int>(std::roundf(remainder.y));

	if (moveX != 0 || moveY != 0)
	{
		mem::PointerArray<Actor> actors = allActors->getAllActors();
		mem::PointerArray<Actor> ridingActors(tempAllocator, MAXIMUM_ACTOR_SLIDING_SIZE);
		int riding_actors_count = 0;
		
		for (size_t i = 0; i < actors.getAmount(); i++)
		{
			Actor* actor = actors[i];
			if (actor == nullptr) continue;

			if (actor->isRiding(*this)) 
			{
				ridingActors.set(actor, riding_actors_count);
				riding_actors_count++;
			}
		}

		collidable = false;

		// horizontal movement
		if (moveX != 0)
		{
			remainder.x -= moveX;
			position.x += moveX;

			if (moveX > 0 && allActors != nullptr)
			{
				for (size_t i = 0; i < actors.getAmount(); i++)
				{
					Actor* actor = actors[i];
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						actor->moveX(getRight() - actor->getLeft(), std::bind(&Actor::squish, actor));
					}
					else if (riding_actors_count > 0 && ridingActors.contains(actor))
					{
						actor->moveX(static_cast<float>(moveX), DO_NOTHING_FUNC);
					}
				}
			}
			else if (moveX < 0 && allActors != nullptr)
			{
				for (size_t i = 0; i < actors.getAmount(); i++)
				{
					Actor* actor = actors[i];
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						actor->moveX(getLeft() - actor->getRight(), std::bind(&Actor::squish, actor));
					}
					else if (riding_actors_count > 0 && ridingActors.contains(actor))
					{
						actor->moveX(static_cast<float>(moveX), DO_NOTHING_FUNC);
					}
				}
			}
		}

		// vertical movement
		if (moveY  != 0)
		{
			remainder.y -= moveY;
			position.y += moveY;

			if (moveY > 0 && allActors != nullptr)
			{
				for (size_t i = 0; i < actors.getAmount(); i++)
				{
					Actor* actor = actors[i];
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						actor->moveX(getBottom() - actor->getTop(), std::bind(&Actor::squish, actor));
					}
					else if (riding_actors_count > 0 && ridingActors.contains(actor))
					{
						actor->moveY(static_cast<float>(moveY), DO_NOTHING_FUNC);
					}
				}
			}
			else if (moveY < 0 && allActors != nullptr)
			{
				for (size_t i = 0; i < actors.getAmount(); i++)
				{
					Actor* actor = actors[i];
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						actor->moveX(getTop() - actor->getBottom(), std::bind(&Actor::squish, actor));
					}
					else if (riding_actors_count > 0 && ridingActors.contains(actor))
					{
						actor->moveY(static_cast<float>(moveY), DO_NOTHING_FUNC);
					}
				}
			}
		}
		
		collidable = true;
	}
}

bool game::Solid::isCollidable() const
{
	return collidable;
}
