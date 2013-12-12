/*
*   The class to represent a single explosion using particle system
*/

#ifndef __EXPLOSION__
#define __EXPLOSION__

#define MAX_TIME 60
#define MIN_SPEED -0.4
#define MAX_SPEED 0.4

typedef struct Particle
{
    float posX, posY, posZ;
    float speedX, speedY, speedZ;
    
} Particle;

class Explosion
{
    public:
        static const int SIZE = 120;
        int time;
        Particle * particles;

        Explosion(float x, float y, float z);
        ~Explosion(){};
        int size();
        void cleanup();
        void evolve();
};

#endif
