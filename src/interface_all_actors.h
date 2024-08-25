#pragma once

#include <vector>
#include "actor.h"
#include <tuple>

namespace game
{
	struct IAllActors
	{
		virtual ~IAllActors() = default;
		// virtual std::vector<Actor*> getAllActors() = 0;
		virtual std::tuple<Actor**, size_t> getAllActors() = 0;
	};
}
