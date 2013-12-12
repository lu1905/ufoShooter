/*
*   The class to represent a single bullet
*/

#ifndef __BULLET__
#define __BULLET__

#include <vector>
using namespace std;

#define BULLET_SHIP 0
#define BULLET_UFO  1

class Bullet
{
    public:
        float posX, posY, posZ;
        float speedX, speedY, speedZ;
        int type;

        Bullet(float x, float y, float z, float sx, float sy, float sz, int t);
        ~Bullet() {};

        void move();
};

#endif
