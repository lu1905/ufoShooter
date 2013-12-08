/*
*	The class to represent a single UFO
*/

#ifndef __UFO__
#define __UFO__

#include "Bullet.h"
#include <vector>
#include <math.h>
using namespace std;

#define SPEED 0.04f
#define BULLET_SPEED 0.2f

class UFO
{
	public:
		float posX,posY,posZ;
		int hp;
		int shoot_counter;
		int shoot_interval;
		bool getHit;
		
		UFO(float x, float y, float z, int h, int interval);
		~UFO(){};

		void move();
		void shoot(float x, float y, float z, vector<Bullet> * bs);
};

#endif