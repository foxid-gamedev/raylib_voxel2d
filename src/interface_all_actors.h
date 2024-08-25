#pragma once


#include "actor.h"
#include "memory/pointer_array.h"

namespace game
{
	struct IAllActors
	{
		virtual ~IAllActors() = default;
		virtual mem::PointerArray<Actor>& getAllActors() = 0;
	};
}
