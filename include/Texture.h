/*
*	The class to represent an opengl texture
*/

#ifndef __TEXTURE__
#define __TEXTURE__

#include<stdint.h>
#include"CImg.h"
using namespace cimg_library;

class Texture
{
	public:		
		int width, height, spectrum;
		uint8_t * data;
		
		Texture(){};
		~Texture() {delete [] data;};
		
		
		void load(const char * fileName);
};

#endif