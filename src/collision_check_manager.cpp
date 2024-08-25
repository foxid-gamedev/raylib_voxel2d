#include "pch.h"
#include "collision_check_manager.h"

bool game::CollisionCheckManager::checkCollision(float x, float y, float w, float h) const
{
    for (ICollisionCheck* check : checks)
    {
        if (check->checkCollision(x, y, w, h))
            return true;
    }
    return false;
}

void game::CollisionCheckManager::addCheck(ICollisionCheck* check)
{
    checks.push_back(check);
}
