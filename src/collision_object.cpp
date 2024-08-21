#include "pch.h"
#include "collision_object.h"

game::CollisionObject::CollisionObject()
    : collider{0}
{
}

void game::CollisionObject::setCollider(float x, float y, float w, float h)
{
    collider.x = x;
    collider.y = y;
    collider.width = w;
    collider.height = h;
}

void game::CollisionObject::setCollider(const Vector2& pos, const Vector2& size)
{
    collider.x = pos.x;
    collider.y = pos.y;
    collider.width = size.x;
    collider.height = size.y;
}

const Rectangle& game::CollisionObject::getCollider() const
{
    return collider;
}

bool game::CollisionObject::checkCollisionRecs(const CollisionObject& other) const
{
    return CheckCollisionRecs(
    {
		position.x + collider.x,
		position.y + collider.y,
		collider.width,
		collider.height
    },
    {
		other.getPosition().x + other.getCollider().x,
		other.getPosition().y + other.getCollider().y,
		other.getCollider().width,
		other.getCollider().height
    });
}

float game::CollisionObject::getRight() const
{
    return position.x + collider.x + collider.width;
}

float game::CollisionObject::getLeft() const
{
    return position.x + collider.x;
}

float game::CollisionObject::getTop() const
{
    return position.y + collider.y;
}

float game::CollisionObject::getBottom() const
{
    return position.y + collider.y + collider.height;
}
