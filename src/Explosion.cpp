#include <stdlib.h>
#include "Explosion.h"
#include <stdio.h>

Explosion :: Explosion(float x, float y, float z)
{
	float speed_range = MAX_SPEED - MIN_SPEED;
	time = MAX_TIME;
	particles = new Particle[SIZE];
	
	for (int i = 0; i < SIZE; i++)
	{
		particles[i].posX = x;
		particles[i].posY = y;
		particles[i].posZ = z;
		
		particles[i].speedX = MIN_SPEED + (float)rand() / ((float)RAND_MAX / speed_range);
		particles[i].speedY = MIN_SPEED + (float)rand() / ((float)RAND_MAX / speed_range);
		particles[i].speedZ = MIN_SPEED + (float)rand() / ((float)RAND_MAX / speed_range);
	}
}

int Explosion :: size()
{
	return SIZE;
}

void Explosion :: cleanup()
{
	delete[] particles;
}

void Explosion :: evolve()
{
	for (int i = 0; i < SIZE; i++)
	{
		particles[i].posX += particles[i].speedX;
		particles[i].posY += particles[i].speedY;
		particles[i].posZ += particles[i].speedZ;
	}
	time--;
}