#pragma once

#include "raylib.h"

#include "interface_updatable.h"
#include "interface_drawable.h"

namespace game
{
	class GameObject : public IDrawable, public IUpdatable
	{
	public:
		explicit GameObject();
		virtual ~GameObject() = default;

		virtual void render() const override;
		virtual void update(float deltaTime) override;

		void setPosition(const Vector2& pos);
		const Vector2& getPosition() const;
	protected:
		Vector2 position;
	};
}
