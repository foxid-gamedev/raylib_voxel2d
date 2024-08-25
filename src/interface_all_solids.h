#pragma once

#include <vector>
#include "solid.h"

namespace game
{
	struct IAllSolids
	{
		virtual ~IAllSolids() = default;
		virtual std::vector<Solid*> getAllSolids() = 0;
	};
}
