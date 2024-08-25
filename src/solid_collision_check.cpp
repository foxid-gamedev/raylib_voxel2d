#include "pch.h"
#include "solid_collision_check.h"

game::SolidCollisionCheck::SolidCollisionCheck(IAllSolids* all_solids) :
    allSolids{ all_solids }
{
}

bool game::SolidCollisionCheck::checkCollision(float x, float y, float w, float h) const
{
    Rectangle other = { x,y,w,h };

    for (Solid* solid : allSolids->getAllSolids())
    {
        if (!solid->isCollidable())
            continue;

        Rectangle rect = {
            solid->getLeft(),
            solid->getTop(),
            solid->getCollider().width,
            solid->getCollider().height
        };

        if (CheckCollisionRecs(rect, other))
            return true;
    }
    return false;
}
