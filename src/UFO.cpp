#include "UFO.h"

UFO::UFO(float x, float y, float z, int h,int interval)
{
    posX = x;
    posY = y;
    posZ = z;
    shoot_interval = interval;
    
    shoot_counter = 0;
    hp = h;
    getHit = false;
}

void UFO::move()
{
    posZ += SPEED;
    shoot_counter++;
}

void UFO::shoot(float x, float y, float z, vector<Bullet> * bs)
{
    if (shoot_counter % shoot_interval == 0)
    {
        float speedX = x - posX;
        float speedY = y - posY;
        float speedZ = z - posZ;

        float normalize = sqrt(speedX * speedX + speedY * speedY + speedZ * speedZ);

        speedX= speedX / normalize * BULLET_SPEED;
        speedY = speedY / normalize * BULLET_SPEED;
        speedZ = speedZ / normalize * BULLET_SPEED;

        Bullet b(posX, posY, posZ, speedX, speedY, speedZ,  BULLET_UFO);

        bs -> push_back(b);
        shoot_counter = 0;
    }
}
