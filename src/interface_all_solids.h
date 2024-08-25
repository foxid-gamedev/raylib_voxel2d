#pragma once

#include "solid.h"
#include "memory/pointer_array.h"

namespace game
{
	struct IAllSolids
	{
		virtual ~IAllSolids() = default;
		virtual mem::PointerArray<Solid>& getAllSolids() = 0;
	};
}
