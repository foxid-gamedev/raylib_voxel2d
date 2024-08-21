#pragma once

#include "raylib.h"
#include "game_object.h"

namespace game
{
	class CollisionObject : public GameObject
	{
	public:
		CollisionObject();
		virtual ~CollisionObject() = default;

		void setCollider(float x, float y, float w, float h);
		void setCollider(const Vector2& pos, const Vector2& size);
		const Rectangle& getCollider() const;

		bool checkCollisionRecs(const CollisionObject& other) const;

		float getRight() const;
		float getLeft() const;
		float getTop() const;
		float getBottom() const;
	protected:
		Rectangle collider;
	};
}
