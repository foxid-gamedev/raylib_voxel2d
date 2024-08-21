#include "pch.h"
#include "solid.h"

bool contains_actor(const std::vector<game::Actor*>& actors, game::Actor* actor);

game::Solid::Solid(IAllActors* all_actors)
	: remainder{ 0 }, collidable{ true }, allActors{all_actors}
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
		std::vector<Actor*>&& actors = allActors->getAllActors();	
		std::vector<Actor*> riding_actors;

		std::copy_if(
			actors.begin(),
			actors.end(), 
			std::back_inserter(riding_actors),
			// lambda filter:
			[this](Actor* actor) { 
				return actor->isRiding(*this); 
			}
		);
		
		collidable = false;

		// horizontal movement
		if (moveX != 0)
		{
			remainder.x -= moveX;
			position.x += moveX;

			if (moveX > 0 && allActors != nullptr)
			{
				for (Actor* actor : actors)
				{
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						// TODO: add callback actor->sqiush()
						// push right
						actor->moveX(getRight() - actor->getLeft(), std::bind(&Actor::squish, actor));
					}
					else if (contains_actor(riding_actors, actor))
					{
						// carry right
						actor->moveX(moveX, DO_NOTHING_FUNC);
					}
				}
			}
			else if (moveX < 0 && allActors != nullptr)
			{
				for (Actor* actor : actors)
				{
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						// TODO: add callback actor->sqiush()
						// push left
						actor->moveX(getLeft() - actor->getRight(), std::bind(&Actor::squish, actor));
					}
					else if (contains_actor(riding_actors, actor))
					{
						// carry left
						actor->moveX(moveX, DO_NOTHING_FUNC);
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
				for (Actor* actor : actors)
				{
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						// TODO: add callback actor->sqiush()
						// push down
						actor->moveX(getBottom() - actor->getTop(), std::bind(&Actor::squish, actor));
					}
					else if (contains_actor(riding_actors, actor))
					{
						// carry right
						actor->moveY(moveY, DO_NOTHING_FUNC);
					}
				}
			}
			else if (moveY < 0 && allActors != nullptr)
			{
				for (Actor* actor : actors)
				{
					if (actor == nullptr) continue;

					if (checkCollisionRecs(*actor))
					{
						// TODO: add callback actor->sqiush()
						// push up
						actor->moveX(getTop() - actor->getBottom(), std::bind(&Actor::squish, actor));
					}
					else if (contains_actor(riding_actors, actor))
					{
						// carry right
						actor->moveY(moveY, DO_NOTHING_FUNC);
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

bool contains_actor(const std::vector<game::Actor*>& actors, game::Actor* actor)
{
	return std::find(actors.begin(), actors.end(), actor) != actors.end();
}
