CC=	g++
CFLAGS=	-Iinclude
PROG=   ufoShooter
SRCS=   src/Explosion.cpp src/Bullet.cpp src/UFO.cpp src/Texture.cpp src/main.cpp
OBJS=	$(SRCS:.cpp=.o)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) \
		-lGL -lGLU -lglut -lX11


%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	
Explosion.o: include/Explosion.h
Bullet.o: include/Bullet.h
UFO.o: include/Bullet.h include/UFO.h
Texture.o: include/Clmg.h include/Texture.h
main.o: include/Bullet.h include/UFO.h include/Texture.h include/Explosion.h

clean:
	rm -f *~ $(OBJS) $(PROG)
