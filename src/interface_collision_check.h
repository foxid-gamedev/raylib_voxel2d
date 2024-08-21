#pragma once

namespace game
{
	struct ICollisionCheck
	{
		virtual ~ICollisionCheck() = default;
		virtual bool checkCollision(float x, float y, float w, float h) const = 0;
	};
}
