-Introduction:
This program is a first-person flight shooting game for one player. The player controls an spaceship that can move in four directions: up, down, left, right. The spaceship itself is moving forward in a constant speed. As the spaceship moving forward, there will appear UFO in front of the player. These UFO are not moving and can attack the spaceship by shooting. The goal of the game is to control the spaceship to shoot these UFOs.


-Features:

What:
1. Spaceship's point of view
2. Menu and game interface
3. Space environment
4. Bullets management
5. UFO creation
6. Collision between objects
7. Transparency effect
8. Picking objects
9. Explosion effect
10. Multiple keys pressed detection

How:
1. Spaceship's point of view
I keep track of the position of the spaceship using variables. When doing draw, I first draw the ship in camera coordinate system. Then shift the world according the position of ship, and draw other objects.

2. Menu and game interface
These are done using 2D texturing under orthographic projection. I first make the orthographic frustum have the same width and height as the viewport. Then draw the images at the position where they are supposed to be by mapping to the same world coordinates. Text is printed in similar way.

3. Space environment
This is done using 2D texturing under perspective projection. Under the camera coordinate, I map a space image to the world coordinates that are corresponding to the four end points of the persoective frustum. So the backgroung always moves with the eye and appears to be very far away.

4. Bullets management
A bullet is created with position (posX, posY, posZ) and speed (speedX, speedY, speedZ). I use a vector to store all bullets that have been created. Draw them at each frame and update their positions according to their speed. A bullet is removed when it is out of certain range.

5. UFO creation
UFOs are created in fixed rate depending on the game level. An UFO is created at fixed Z position away from the spaceship, and random X and random Y positions within certain range. I use a vector to store all UFOs that have been created. Draw them at each frame and update their positions. An UFO is removed when it is behine the space ship.

6. Collision between objects
Collision is done using rectangle proxy. When drawing a bullet, check whether the bullect is inside the bounding box of the ship or any UFO. If so, decrease the hp of the hit object and remove the bullet. When drawing the ship, check whether its bounding box intersects with boxes of any UFO. If so, set the ship hp to 0.

7. Transparency effect
When the ship trigger a special skill called invisiblity, it appears to be transparent. This is done by enabling blending and setting the alpha value of the drawing color.

8. Picking objects
Picking is done using opengl selection mode. That is, specify a small viewport aroung the cursor, load the id of each UFO to the name stack before drawing it. Let the pipeline do the clipping and if it finally draw something. If so, the id on top of the name stack is stored in the hit list.

9. Explosion effectThis is done using a simple partical system. An explosion has a set of partical, each of whom has position (posX, posY, posZ) and speed (speedX, speedY, speedZ). The initial position of particles is the position where the explosion happens. The speed values are randomly set. I use a vector to store all UFOs that have been created. When drawing an explosion, draw the particles according to their current positions and then update their positions based on their speed. Also, decrement the time value of the explosion. when the time value becomes 0, the explosion is removed from the vector.
 
10. Multiple keys pressed detection
I used an array to keep track of whether a key is down. At each frame, check the array to see which keys are pressed and perform corresponding actions.


-Platform: Linux


-How to install:
$ cd ufoShooter
$ make
$ ./ufoShooter


-Hwo to play:
'w'	move up
's'	move down
'a'	move left
'd'	move right
'j'	shoot
'k'	kill the selected UFO
'i'	trigger invisiblity
space	select an UFO by traversing them one by one
mouse_left	select UFO under the cursor
