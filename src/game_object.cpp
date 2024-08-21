#include "pch.h"
#include "game_object.h"

game::GameObject::GameObject()
    : position{ 0 }
{
}

void game::GameObject::render() const
{
}

void game::GameObject::update(float deltaTime)
{
}

void game::GameObject::setPosition(const Vector2& pos)
{
    position = pos;
}

const Vector2& game::GameObject::getPosition() const
{
    return position;
}
