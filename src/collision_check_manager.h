#pragma once

#include "interface_collision_check.h"
#include <vector>

namespace game
{
	class CollisionCheckManager : public ICollisionCheck
	{
	public:
		bool checkCollision(float x, float y, float w, float h) const;

		void addCheck(ICollisionCheck* check);
	private:
		std::vector<ICollisionCheck*> checks;
	};
}
