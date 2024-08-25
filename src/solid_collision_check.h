#pragma once

#include "interface_collision_check.h"
#include "interface_all_solids.h"

namespace game
{
	class SolidCollisionCheck : public ICollisionCheck
	{
	public:
		explicit SolidCollisionCheck(IAllSolids* all_solids);
		virtual ~SolidCollisionCheck() = default;

		bool checkCollision(float x, float y, float w, float h) const override;
	private:
		IAllSolids* allSolids;
	};
}
