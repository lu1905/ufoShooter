#include "Bullet.h"

Bullet :: Bullet(float x, float y, float z, float sx, float sy, float sz, int t)
{
    posX = x;
    posY = y;
    posZ = z;

    speedX = sx;
    speedY = sy;
    speedZ = sz;

    type = t;
}

void Bullet :: move()
{
    posX += speedX;
    posY += speedY;
    posZ += speedZ;
}
