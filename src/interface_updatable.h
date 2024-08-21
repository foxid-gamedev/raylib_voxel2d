#pragma once

namespace game
{
	struct IUpdatable
	{
		virtual ~IUpdatable() = default;
		virtual void update(float deltaTime) = 0;
	};
}
