#if defined(__APLE_CC__)

#include<OpenGL/gl.h>
#include<OpenGL/glu.h>
#include<GLUT/glut.h>
#elif defined(WIN32)
#include<windows.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#else
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<stdint.h>
#endif

#include<iostream>
#include<stdlib.h>
#include <stdio.h>
#include<math.h>
#include <iostream>
#include <vector>

#include "Bullet.h"
#include "UFO.h"
#include "Texture.h"
#include "Explosion.h"

using namespace std;


/*============= Global Variables ===============*/

// Display
#define WIN_WIDTH 	1024
#define WIN_HEIGHT 	768
#define zNear 			0.1f
#define zFar  			100.0f 
#define FOVY 			70.0f
#define MAX_X 		15
#define MAX_Y 		10
#define SHIP_Z_SHIFT 	-3.0
#define TARGET_FPS 	50

// GAME
#define GAME_TIME 	300
#define MENU 			0
#define GAME 			1
#define END 			2
#define MOVE_SPEED	0.18f
#define SHIP_BULLET_SPEED 		-0.3f
#define UFO_CREATE_Z			-40.0
#define UFO_CREATE_INTERVAL_1 	200
#define UFO_CREATE_INTERVAL_2 	180
#define UFO_CREATE_INTERVAL_3 	150
#define UFO_SHOOT_INTERVAL_1	100
#define UFO_SHOOT_INTERVAL_2	85
#define UFO_SHOOT_INTERVAL_3	70
#define UFO_HP_1	10
#define UFO_HP_2	14
#define UFO_HP_3	17
#define SHOOT_CD	15
#define INVISIBLE_INTERVAL	400
#define INVISIBLE_CD		1500
#define KILL_CD			1000
#define HEALTH_MAX		50
#define END_DELAY 		100


// Display
float x_move = 0.0f;
float y_move = 0.0f;
int turn_rotate = 0;
Texture textures[13];
GLuint texInts[13];
int play_time = 0;
int previous = 0;
int frames = 0;
int fps;
bool keyDown[256];
int win_width = WIN_WIDTH;
int win_height = WIN_HEIGHT;

// Game
int stage =  MENU;
bool playing = false;
int level = 0;
int temp_level = 0;
int ufo_create_interval[3] = {UFO_CREATE_INTERVAL_1, UFO_CREATE_INTERVAL_2, UFO_CREATE_INTERVAL_3};
int ufo_shoot_interval[3]= {UFO_SHOOT_INTERVAL_1, UFO_SHOOT_INTERVAL_2, UFO_SHOOT_INTERVAL_3};
int ufo_hp[3] = {UFO_HP_1, UFO_HP_2, UFO_HP_3};
vector<Bullet> bullets;
vector<UFO> ufos;
vector<Explosion> explosions;
bool getHit = false;
int health = HEALTH_MAX;
int score = 0;
int total_ufo = 0;
int UFOPick = -1;
bool invisible = false;
int invisible_interval = INVISIBLE_INTERVAL;
int invisible_cd = 0;
int shoot_cd = 0;
int kill_cd = 0;
int delay = END_DELAY;

/*=========== Global Variables End =============*/


/*================ Functions ==============*/

// Display
void init(void);
void display(void);
void resize(int width, int height);
void idle(int value);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void keyOp(void);
void mouseClick(int key, int state, int x, int y);

// Drawing
void drawBackground(void);
void drawShip(void);
void drawBullets(void);
void drawExplosions(void);
void drawInterface(void);
void drawUFOs(void);
void texturing(int id, float bl_x, float tl_x, float tr_x, float br_x, 
			float bl_y, float tl_y, float tr_y, float br_y, 
			float d_tl, float d_bl, float d_tr, float d_br);
void pick(int x, int y);
void drawCube(float lenX, float lenY, float lenZ, float r, float g, float b, bool transparent);
void drawSphere(float radiusX, float radiusY, float radiusZ, float r, float g, float b,  bool transparent);

// Game
void createUFO(void);
int hitUFO(int bid);
bool hitShip(int bid);
bool shipHitUFO();
int getFPS(void);

/*============== Functions End ==============*/


/*============== Implementations ============*/

// the initialization function
void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// viewport
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	// lighting
	GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat diffuse[] = {0.9, 0.9, 0.9, 1.0};
	GLfloat specular[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat position0[] = {1.0, 1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	GLfloat position1[] = {-1.0, -1.0, -1.0, 0.0};
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
    
	// projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT)), zNear, zFar);
	    
	// keyboard
	for(int i = 0; i < 256; i++)
		keyDown[i] = false;

	// texturing
	textures[0].load( "data/space.bmp");
	textures[1].load( "data/new.bmp");
	textures[2].load( "data/normal.bmp");
	textures[3].load( "data/hard.bmp");
	textures[4].load( "data/crazy.bmp");
	textures[5].load( "data/continue.bmp");
	textures[6].load( "data/quit.bmp" );
	textures[7].load("data/ufo.bmp");
	textures[8].load("data/health.bmp");
	textures[9].load("data/kill.bmp");
	textures[10].load("data/invisible.bmp");
	textures[11].load("data/menu_back.bmp");
	textures[12].load("data/canada.bmp");
	glGenTextures(13, texInts);
	
	// points property
	glEnable(GL_POINT_SMOOTH);
	glPointSize(2.0f);
}

// the display callback function
void display()
{
	// display game scene
	if (stage == GAME)
	{
		keyOp();	// hendle keyboard operations
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		drawBackground();
		glTranslatef(0,-1.0,-3);	
		if(health > 0)
			drawShip();
		glTranslatef(x_move,y_move, 0);
		drawBullets();
		drawExplosions();
		drawInterface();
		drawUFOs();
	}
	// display game menu
	else
	{
		// using orthographic projection
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		glOrtho(-win_width / 2, win_width / 2, -win_height / 2, win_height /  2, -1.0f, 1.0f);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// game menu buttons
		if (stage == MENU)
		{
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

			// draw menu background 
			texturing(11, -win_width / 2, -win_width / 2, win_width / 2, win_width / 2,
				-win_height / 2, win_height / 2, win_height / 2, -win_height / 2, 0.0, 0.0, 0.0, 0.0);
		
			float lenX = win_width / 7.0;
			float lenY = win_height / 21.0;
			
			texturing(1, -lenX - lenX / 10, -lenX - lenX / 10, -lenX / 10, -lenX / 10,
				0, lenY, lenY, 0, 1.0, 1.0, 1.0, 1.0);
				
			texturing(temp_level + 2, lenX / 10, lenX / 10, lenX + lenX / 10, lenX + lenX / 10,
				0, lenY, lenY, 0, 1.0, 1.0, 1.0, 1.0);
				
			texturing(5, -lenX / 2, -lenX / 2, lenX / 2, lenX / 2,
				-lenY * 2, -lenY, -lenY, -lenY * 2, 1.0, 1.0, 1.0, 1.0);
				
			texturing(6, -lenX / 2, -lenX / 2, lenX / 2, lenX / 2,
				-lenY * 4, -lenY * 3, -lenY * 3, -lenY * 4, 1.0, 1.0, 1.0, 1.0);
			
			glFlush();
			glDisable(GL_TEXTURE_2D);
		}
		// game over scene
		else
		{
			glDisable(GL_DEPTH_TEST);
			
			// score
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2i(-60, 0);
			char chs[16];
			sprintf(chs, "%d", score);
			string str = "Score: ";
			str = str + chs;
			sprintf(chs, "%d", total_ufo);
			str = str + " / " + chs;
			for (int i = 0; i < str.size(); i++)
				glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			
			glEnable(GL_DEPTH_TEST);
		}
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	glutSwapBuffers();
}

// the resize callback function
void resize(int width, int height)
{    
	// viewport
	glViewport(0, 0, width, height);

	// projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT)), zNear, zFar);	    

	win_width = width;
	win_height = height;
}

// the idle callback function
void idle(int value)
{
	if (stage == GAME && play_time / TARGET_FPS < GAME_TIME && health > 0)
	{
		// update play time
		play_time++;
		// create ufos
		createUFO();
	}
	else if (playing && stage != MENU && delay == 0)
		stage = END;
	
	if (play_time / TARGET_FPS >= GAME_TIME)
	{
		delay = 0;
	}
	if (health <= 0)
	{
		if (delay == END_DELAY)
		{
			Explosion e(-x_move, -y_move, -1);
				explosions.push_back(e);
		}
		
		delay--;
	}
	glutPostRedisplay();
    
	glutTimerFunc(1000.0f * 1.0f / TARGET_FPS, idle, 0);	// call at fixed time to aim target fps
}


// keboard key down callback function
void keyboardDown(unsigned char key, int x, int y)
{	    
	// for mutiple press
	keyDown[key] = true;

	// for single press
	switch(key)
	{
		case 'q':
			if (stage == END)
				playing = false;
			stage = MENU;
			temp_level = level;
			break;
		case 'i':
			if(invisible_cd == 0)
			{
				invisible = true;
				invisible_interval = INVISIBLE_INTERVAL;
			}
			break;
		case 'k':
			if (UFOPick != -1 && kill_cd == 0)
			{
				ufos[UFOPick].hp = 0;
				kill_cd = KILL_CD;
			}
			break;
		case ' ':			
			if (ufos.size() == 0)
				UFOPick  = -1;
			else
				UFOPick = (++UFOPick) % ufos.size();
			break;
	}
}

// keboard key up callback function
void keyboardUp( unsigned char key, int x, int y )
{
	keyDown[key] = false;
}

// perform keypoard operations
void keyOp()
{
	if (keyDown['a'])
	{	
		if (turn_rotate < 10)
			turn_rotate ++;
		if (x_move <= MAX_X)
			x_move += MOVE_SPEED;
	}
	if (keyDown['d'])
	{
		if (turn_rotate > -10)
			turn_rotate --;
		
		if (x_move >= -MAX_X)
			x_move -= MOVE_SPEED;
	}
	if (keyDown['s'])
		if (y_move <= MAX_Y)
			y_move += MOVE_SPEED;
	if (keyDown['w'])
		if (y_move >= -MAX_Y)
			y_move -= MOVE_SPEED;
	if (keyDown['j'] && shoot_cd == 0)
	{
		Bullet b(-x_move, -y_move, -0.6, 0, 0, SHIP_BULLET_SPEED, BULLET_SHIP);
		bullets.push_back(b);
		shoot_cd = SHOOT_CD;
	}
}

// draw a background scene using texturing
void drawBackground()
{	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	float h = zFar * tan(FOVY / 2.0f) * 1.48;
	float aspect = float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT));
	float w = h * aspect;
	
	texturing(0, -w, -w, w, w,
			-h, h, h, -h, -zFar + 0.1, -zFar + 0.1, -zFar + 0.1, -zFar + 0.1);

	glFlush();
	glDisable(GL_TEXTURE_2D);
}

// draw the ship
void drawShip()
{
	if(shipHitUFO())
	{
		health = 0;
		return;
	}
	
	glPushMatrix();
	
	// set colors based on the state of the ship
	float r = 1.0f, g = 1.0f, b = 1.0f;
	if (invisible)
	{
		if (--invisible_interval == 0)
		{
			invisible = false;
			invisible_cd = INVISIBLE_CD;
		}
	}
	if (getHit)
	{
		g = 0.5;
		b = 0.5;
		getHit = false;
	}
	
	// rotation effect
	if (!keyDown['a'] && !keyDown['d'] && turn_rotate != 0)
	{
		if (turn_rotate > 0)
			turn_rotate--;
		else
			turn_rotate++;
	}
	
	glRotatef(turn_rotate, 0, 0, 1);
	
	// draw a line to help aim target
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0, 0, -30);
	glEnd();
	
	// draw main body
	drawCube(0.4, 0.1, 0.3, r, g, b, invisible);

	// draw flag
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	texturing(12, -0.2, -0.2, 0.2, 0.2,
			0.051, 0.051, 0.051, 0.051,
			-0.15, 0.15, 0.15, -0.15);		
	glFlush();
	glDisable(GL_TEXTURE_2D);

	int mutiplier = 1;
	for(int i = 0; i < 2; i++)
	{
		glPushMatrix();
		
		glRotatef(mutiplier * 2.0 , 0.0 ,0.0 ,1.0 );		
		glTranslatef(mutiplier * (0.2 + 0.1), 0.02, 0.0);
		drawCube(0.2, 0.015, 0.3, r, g, b, invisible);
		glTranslatef(mutiplier * 0.1, 0.0, 0.0);
		glRotatef(-mutiplier * 30.0, 0.0, 0.0, 1.0);
		glTranslatef(mutiplier * 0.05, 0.0, 0.0);
		drawCube(0.1, 0.015, 0.3, r, g, b, invisible);
		glTranslatef(mutiplier * 0.05, 0.0, 0.0);
		drawSphere(0.02, 0.02, 0.5, r, g, b, invisible);

		glPopMatrix();
		
		mutiplier = -1;
	}

	glTranslatef(0.0, 0.0, -0.15 - 0.1);
	drawCube(0.3, 0.1, 0.2, r, g, b, invisible);

	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(0.0, 0.0, -0.1);
	
	drawSphere(0.1, 0.05, 0.5, r, g, b, invisible);
	
	// update skill cool down time
	if (invisible_cd > 0)
		invisible_cd --;
	if (shoot_cd > 0)
		shoot_cd --;
	if (kill_cd > 0)
		kill_cd --;
	
	glPopMatrix();
}

// draw bullets
void drawBullets()
{
	// draw each bullet
	int i = 0;
	while(i != bullets.size())
	{	
		if (bullets[i].posZ < UFO_CREATE_Z ||  bullets[i].posZ > -SHIP_Z_SHIFT)
			bullets.erase(bullets.begin() + i);
		else
		{
			glPushMatrix();
			
			glTranslatef(bullets[i].posX, bullets[i].posY, bullets[i].posZ);
			if (bullets[i].type == BULLET_SHIP)
				drawSphere(0.05, 0.05, 0.05, 0.2, 1.0, 0.2, false);
			else
				drawSphere(0.05, 0.05, 0.05, 1.0, 0.2, 0.2, false);
			
			glPopMatrix();

			bullets[i].move();	// update postion
			
			// hit detection for ship
			if (bullets[i].type == BULLET_UFO && hitShip(i))
			{
				bullets.erase(bullets.begin() + i);
				getHit = true;
				if (health > 0)
					health--;
			}
			// hit detection for ufos
			else if (bullets[i].type == BULLET_SHIP)
			{
				int hit = hitUFO(i);
				if (hit != -1)
				{
					bullets.erase(bullets.begin() + i);
					ufos[hit].getHit = true;
					
					ufos[hit].hp--;
				}
				else
					i++;
			}
			else
				i ++;
		}
	}
}

// draw explosions
void drawExplosions()
{
	
	int i = 0;
	while(i != explosions.size())
	{	
		// if an explosion finish
		if (explosions[i].time <= 0)
		{
			explosions[i].cleanup();
			explosions.erase(explosions.begin() + i);
		}
		else
		{
			glColor3f(0.7, 0.7, 0.4);
			// draw particles
			for (int j = 0; j < explosions[i].size(); j++)
			{
				glBegin(GL_POINTS);
				glVertex3f(explosions[i].particles[j].posX, explosions[i].particles[j].posY, explosions[i].particles[j].posZ);
				glEnd();
			}
			
			// update explosion
			explosions[i].evolve();
			i ++;
		}
	}
}

// draw game interface using orthographic projection
void drawInterface()
{
	// save matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, win_width, win_height , 0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	float posX, posY, lenX, lenY;
	
	//=============== Text ============//
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_DEPTH_TEST);	// disable depth test to make drawing efficient
	
	// FPS
	posX = win_height / 50;
	posY = win_height / 1.01;
	glRasterPos2i(posX, posY);
	char chs[16];
	sprintf(chs, "%d", getFPS());
	string str = "FPS: ";
	str = str + chs;
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	
	// Remainnin time
	posX = win_width / 2.0 - 120.0;
	posY = win_height / 30.0;
	glRasterPos2i(posX, posY);
	sprintf(chs, "%d", GAME_TIME - play_time / TARGET_FPS);
	str = "Remainning Time: ";
	str = str + chs;
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	
	// HP
	posX = win_width / 50.0;
	posY = win_height / 20.0;
	glRasterPos2i(posX, posY);
	sprintf(chs, "%d", health);
	str = "HP: ";
	str = str + chs;
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	
	// score
	posX = win_width / 1.2 + win_height / 30.0;
	posY = win_height / 20.0 + win_height / 30.0 - 5.0;
	glRasterPos2i(posX, posY);
	sprintf(chs, "%d", score);
	str = ": ";
	str = str + chs;
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	
	// kill cool down time
	posX = win_width / 50.0 + win_height / 30.0;
	posY = win_height / 11.0 + win_height / 30.0 - 5.0;
	glRasterPos2i(posX, posY);
	if (kill_cd == 0)
		str = ": Ready";
	else
	{
		sprintf(chs, "%d", kill_cd / TARGET_FPS);
		str = ": ";
		str = str + chs;
	}
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	
	// invisible cool down time
	posX = win_width / 50.0 + win_height / 30.0;
	posY = win_height / 8.0 + win_height / 30.0 - 5.0;
	glRasterPos2i(posX, posY);
	if (invisible_cd == 0)
	{
		if (invisible_interval > 0 && invisible_interval < INVISIBLE_INTERVAL)
			str = ": On";
		else
			str = ": Ready";
	}
	else
	{
		sprintf(chs, "%d", invisible_cd / TARGET_FPS);
		str = ": ";
		str = str + chs;
	}
	for (int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	glEnable(GL_DEPTH_TEST);
	//============ Text End ============//
    
	//============ Image =============//
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	// HP
	posX = win_width / 50.0;
	posY = win_height / 20.0;
	lenX = win_width / 7.0 * ((float)health / HEALTH_MAX);
	lenY = win_height / 40.0;
	texturing(8, posX , posX , posX  + lenX, posX  + lenX,
			posY + lenY, posY, posY, posY + lenY, 0.0, 0.0, 0.0, 0.0);

	// score
	posX = win_width / 1.2;
	posY = win_height / 20.0;
	lenX = win_height / 30.0;
	lenY = win_height / 30.0;
	
	texturing(7, posX , posX , posX  + lenX, posX  + lenX,
			posY + lenY, posY, posY, posY + lenY, 0.0, 0.0, 0.0, 0.0);

	
	// kill cool down time
	posX = win_width / 50.0;
	posY = win_height / 11.0;
	lenX = win_height / 30.0;
	lenY = win_height / 30.0;
	texturing(9, posX , posX , posX  + lenX, posX  + lenX,
			posY + lenY, posY, posY, posY + lenY, 0.0, 0.0, 0.0, 0.0);

	// invisible cool down time
	posX = win_width / 50.0;
	posY = win_height / 8.0;
	lenX = win_height / 30.0;
	lenY = win_height / 30.0;
	
	texturing(10, posX , posX , posX  + lenX, posX  + lenX,
			posY + lenY, posY, posY, posY + lenY, 0.0, 0.0, 0.0, 0.0);
	
	glFlush();
	glDisable(GL_TEXTURE_2D);
	//========== Image End ===========//


	// restore matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// draw ufos
void drawUFOs()
{
	int i = 0;
	while(i != ufos.size())
	{	
		// destory ufo if hp is 0 or out of display range
		if (ufos[i].posZ > 0.0 || ufos[i].hp <= 0)
		{
			// create explosion if the ufo is destoried
			if (ufos[i].hp <= 0)
			{
				Explosion e(ufos[i].posX, ufos[i].posY, ufos[i].posZ);
				explosions.push_back(e);
				score++;
			}
			
			if (UFOPick != -1)
				UFOPick--;
			
			ufos.erase(ufos.begin() + i);
		}
		else
		{
			glLoadName(i + 1);	// load the ufo id to the name stack
			glPushMatrix();
			
			// draw the actual ufo
			
			// main body
			glTranslatef(ufos[i].posX, ufos[i].posY, ufos[i].posZ);
			if (ufos[i].getHit)
			{
				drawSphere(1.0, 0.5, 1.0 ,1.0, 0.4, 0.4, false);
				ufos[i].getHit = false;
			}
			else if(i == UFOPick)
				drawSphere(1.0 ,0.5,1.0 ,0.4, 1.0, 0.4, false);
			else
				drawSphere(1.0 ,0.5,1.0 ,0.4, 0.4, 0.4, false);
			
			// disk
			glTranslatef(0.0, -0.2, 0.0);
			drawSphere(2.0, 0.2, 2.0, 0.2, 0.1, 0.1, false);
			
			// "feet"
			glPushMatrix();
			glTranslatef(0.0, -0.5, -0.5);
			glRotatef(20.0, 1, 0.0, 0.0);
			drawSphere(0.1, 0.6, 0.1 ,0.4, 0.1, 0.4, false);
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(-0.5, -0.5, 0.5);
			glRotatef(-20.0, 1.0, 0.0, 0.0);
			glRotatef(-20.0, 0.0, 0.0, 1.0);
			drawSphere(0.1, 0.6, 0.1 ,0.4, 0.1, 0.4, false);
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(0.5, -0.5, 0.5);
			glRotatef(-20.0, 1.0, 0.0, 0.0);
			glRotatef(20, 0.0, 0.0, 1.0);
			drawSphere(0.1, 0.6, 0.1 , 0.4, 0.1, 0.4, false);
			glPopMatrix();
			
			glPopMatrix();

			// update position
			ufos[i].move();
			
			// shoot the ship
			if (!invisible & health > 0)
				ufos[i].shoot(-x_move, -y_move, 0, &bullets);
			
			i ++;
		}
	}
}

// perform texturing
void texturing(int id, float bl_x, float tl_x, float tr_x, float br_x, 
			float bl_y, float tl_y, float tr_y, float br_y, 
			float d_tl, float d_bl, float d_tr, float d_br)
{
	// bind texture
	glBindTexture(GL_TEXTURE_2D, texInts[id]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        int mode = GL_LUMINANCE;
        if(textures[id].spectrum == 3)
               mode = GL_RGB;
        if(textures[id].spectrum == 4)
               mode = GL_RGBA;
        gluBuild2DMipmaps(GL_TEXTURE_2D, mode, textures[id].width, textures[id].height, mode, GL_UNSIGNED_BYTE, textures[id].data);
        
	// map coordinate
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(tl_x, tl_y, d_tl);
	glTexCoord2f(0.0, 1.0); glVertex3f(bl_x,  bl_y, d_bl);
	glTexCoord2f(1.0, 1.0); glVertex3f(br_x, br_y, d_tr);
	glTexCoord2f(1.0, 0.0); glVertex3f(tr_x , tr_y, d_br);
	glEnd();
}

// perform picking
void pick(int x, int y)
{
	GLuint buf[512];
 	GLint viewport[4];
 
 	glSelectBuffer(512, buf);		// initializing hit buffer
	glRenderMode(GL_SELECT);	// enter selection mode
	
	glMatrixMode(GL_PROJECTION);	// push projection matrix
 	glPushMatrix();
	glLoadIdentity();
	
 	glGetIntegerv(GL_VIEWPORT, viewport);	// specify viewport
	gluPickMatrix(x, win_height - y, 4, 4, viewport);
 	gluPerspective(FOVY, float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT)), zNear, zFar);
 
 	glInitNames();		// initializing name stack
 	glPushName(0);

	display();		// draw objects using the new viewport
 
	glMatrixMode(GL_PROJECTION);	// restore projection matrix
 	glPopMatrix();

	// processing hit list
 	int hit_num = glRenderMode(GL_RENDER);
	if (hit_num > 1)
	{
		unsigned int target = (unsigned int) buf[7];
		UFOPick = target - 1;
	}
	else
		UFOPick = -1;
}

// draw a unit cube
void drawCube(float lenX, float lenY, float lenZ, float r, float g, float b, bool transparent)
{
	glPushMatrix();
	
	// transparent effect using blending
	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r, g, b, 0.2f);
	}
	else
		glColor3f(r, g, b);
	
	glScalef(lenX, lenY, lenZ);
	glutSolidCube(1.0f);
	
	if (transparent)
		glDisable(GL_BLEND);
	
	glPopMatrix();
}

// draw a unit sphere
void drawSphere(float radiusX, float radiusY, float radiusZ, float r, float g, float b, bool transparent)
{
	glPushMatrix();
	
	// transparent effect using blending
	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(r, g, b, 0.2f);
	}
	else
		glColor3f(r, g, b);
	
	glScalef(radiusX, radiusY, radiusZ);
	glutSolidSphere(1.0f, 14.0f, 14.0f);
	
	if (transparent)
		glDisable(GL_BLEND);
	
	glPopMatrix();
}

// create UFOs at fixed rate
void createUFO()
{
	if (play_time % ufo_create_interval[level] == 0)
	{
		// create at random place inside the movable region
		UFO u((rand() % (2 * MAX_X)) - MAX_X, (rand() % (2 * MAX_Y)) - MAX_Y, UFO_CREATE_Z, ufo_hp[level], ufo_shoot_interval[level]);
		ufos.push_back(u);
		total_ufo++;
	}
}

// handle mouse operation
 void mouseClick(int key, int state, int x, int y) 
{
	
	if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// pick object in game
		if (stage == GAME)
			pick(x, y);
		
		// press button in game menu
		else if (stage == MENU)
		{
			// translate viewport coordinate to world coordinate
			float worldX = x - win_width / 2.0;
			float worldY = (win_height - y) - win_height / 2.0;
		
			float lenX = win_width / 7.0;
			float lenY = win_height / 21.0;	
			
			// new game
			if (worldX >=  -lenX - lenX / 10 && worldX  <= -lenX / 10
				&& worldY >= 0 && worldY <= lenY)
			{
				level = temp_level;
				ufos.clear();
				bullets.clear();
				explosions.clear();
				score = 0;
				total_ufo = 0;
				health = HEALTH_MAX;
				playing = true;
				play_time = 0;
				invisible_cd = 0;
				kill_cd = 0;
				getHit = false;
				invisible = false;
				delay = END_DELAY;
				
				stage = GAME;
			}
			
			// level
			else if (worldX >= lenX / 10 && worldX <= lenX + lenX / 10
				&& worldY >= 0 && worldY <= lenY)
					temp_level = ++temp_level % 3;
			
			// continue
			else if (worldX >=  -lenX / 2 && worldX <= lenX / 2
				&& worldY >= -lenY * 2 && worldY <= -lenY)
				{
					if (playing)
						stage = GAME;
				}
			
			// quit
			else if (worldX >=  -lenX / 2 && worldX <= lenX / 2
				&& worldY >= -lenY * 4 && worldY <= -lenY * 3)
				exit(0);
		}
	}
}


// cllision detection between bullets and UFOs using rectangle proxy
int hitUFO(int bid)
{
	// check for each UFO
	for(int i = 0; i < ufos.size(); i++)
	{
		// check if the bullet is inside the rectangle proxy for the UFO
		if (bullets[bid].posZ <= ufos[i].posZ + 0.7 && bullets[bid].posZ >= ufos[i].posZ - 0.7)
		{
			if (bullets[bid].posX <= ufos[i].posX + 1.1 && bullets[bid].posX >= ufos[i].posX - 1.1 &&
				bullets[bid].posY <= ufos[i].posY + 0.55 && bullets[bid].posY >= ufos[i].posY - 0.55)
				return i;
		}
		
	}
	
	return -1;
}

// cllision detection between bulltes and ship using rectangle proxy
bool hitShip(int bid)
{
	// check if the bullet is inside the rectangle proxy for the ship
	return (bullets[bid].posZ >= -0.15 && bullets[bid].posZ <= + 0.7 &&
			bullets[bid].posX >= -x_move - 0.4 && bullets[bid].posX <= -x_move + 0.4 &&
			bullets[bid].posY >= -y_move - 0.07 && bullets[bid].posY <= -y_move + 0.07 &&
			health > 0);
}

// cllision detection between ship and UFOs using rectangle proxy
bool shipHitUFO()
{
	// check for each UFO
	for(int i = 0; i < ufos.size(); i++)
	{
		if(-ufos[i].posZ <= 1.0 && abs(-x_move - ufos[i].posX) <= 1.1 && abs(-y_move - ufos[i].posY) <= 0.54)
			return true;
	}
	
	return false;
}

// get frames per second
int getFPS()
{
	frames++;
	
	// calculate time interval
	int now = glutGet(GLUT_ELAPSED_TIME);
	int interval = now - previous;

	// if interval is greater than 1 second, calculate frames / 1 second
	if (interval > 1000)
	{
		fps = frames / (interval / 1000);
		frames = 0;
		previous = now;
	}
	
	return fps;
}

// program entry point
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(0, 100);
	glutCreateWindow("UFO SHOOTER");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseClick);
	init();
	idle(0);

	glutMainLoop();

	return 0;
}

/*========== Implementations End =========*/

