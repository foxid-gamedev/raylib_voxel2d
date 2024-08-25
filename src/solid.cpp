#include "pch.h"
#include "solid.h"

bool contains_actor(game::Actor** begin, game::Actor** end, game::Actor* actor);

game::Solid::Solid(IAllActors* all_actors, mem::BumpAllocator& temp_allocator)
	: remainder{ 0 }, collidable{ true }, allActors{ all_actors }, tempAllocator{ temp_allocator }
{
	std::cout << "temp_alloc in solid: " << &tempAllocator << " and address from parameter: " << &temp_allocator << "\n";
}

void game::Solid::move(float x, float y)
{
	remainder.x += x;
	remainder.y += y;

	int moveX = static_cast<int>(std::roundf(remainder.x));
	int moveY = static_cast<int>(std::roundf(remainder.y));

	if (moveX != 0 || moveY != 0)
	{
		auto [actors,number] = allActors->getAllActors();
		constexpr size_t max_actors_size = sizeof(Actor*) * MAXIMUM_ACTOR_SLIDING_SIZE;

		uint8_t* ridingActors = tempAllocator.allocate(max_actors_size);
		
		uint8_t* count = ridingActors;
		int riding_actors_count = 0;
		
		for (size_t i = 0; i < number; i++)
		//for (Actor* actor : actors)
		{
			Actor* actor = *(actors + i);
			if (actor == nullptr) continue;

			if (actor->isRiding(*this)) 
			{
				if (count + sizeof(Actor*) > ridingActors + max_actors_size)
					break;
				memcpy(count, &actor, sizeof(Actor*));
				count += sizeof(Actor*);
				riding_actors_count++;
			}
		}

		Actor** begin = reinterpret_cast<Actor**>(ridingActors);
		Actor** end = reinterpret_cast<Actor**>(count);
		
		collidable = false;

		// horizontal movement
		if (moveX != 0)
		{
			remainder.x -= moveX;
			position.x += moveX;

			if (moveX > 0 && allActors != nullptr)
			{
				for (size_t i = 0; i < number; i++)
				//for (Actor* actor : actors)
				{
					Actor* actor = *(actors + i);
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						actor->moveX(getRight() - actor->getLeft(), std::bind(&Actor::squish, actor));
					}
					else if (riding_actors_count > 0 && contains_actor(begin, end, actor))
					{
						actor->moveX(static_cast<float>(moveX), DO_NOTHING_FUNC);
					}
				}
			}
			else if (moveX < 0 && allActors != nullptr)
			{
				for (size_t i = 0; i < number; i++)
				//for (Actor* actor : actors)
				{
					Actor* actor = *(actors + i);
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						actor->moveX(getLeft() - actor->getRight(), std::bind(&Actor::squish, actor));
					}
					else if (riding_actors_count > 0 && contains_actor(begin, end, actor))
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
				for (size_t i = 0; i < number; i++)
				//for (Actor* actor : actors)
				{
					Actor* actor = *(actors + i);
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						actor->moveX(getBottom() - actor->getTop(), std::bind(&Actor::squish, actor));
					}
					else if (riding_actors_count > 0 && contains_actor(begin, end, actor))
					{
						actor->moveY(static_cast<float>(moveY), DO_NOTHING_FUNC);
					}
				}
			}
			else if (moveY < 0 && allActors != nullptr)
			{
				for (size_t i = 0; i < number; i++)
				//for (Actor* actor : actors)
				{
					Actor* actor = *(actors + i);
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						actor->moveX(getTop() - actor->getBottom(), std::bind(&Actor::squish, actor));
					}
					else if (riding_actors_count > 0 && contains_actor(begin, end, actor))
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

bool contains_actor(game::Actor** begin, game::Actor** end, game::Actor* actor)
{
	for (game::Actor** current = begin; current <= end; current++)
	{
		if (actor == (*current))
		{
			return true;
		}
	}
	return false;
}
