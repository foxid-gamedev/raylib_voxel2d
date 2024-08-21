#pragma once

namespace game
{
	struct IDrawable
	{
		virtual ~IDrawable() = default;
		virtual void render() const = 0;
	};
}
