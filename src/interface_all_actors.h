#pragma once

#include <vector>
#include "actor.h"

namespace game
{
	struct IAllActors
	{
		virtual ~IAllActors() = default;
		virtual std::vector<Actor*> getAllActors() = 0;
	};
}
