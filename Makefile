OPT=-g
#-Os

MAPLE_SUBDIR=maple

SUBPIXEL=-DSUBPIXEL_COORDS -DSUBPIXEL_FRACTIONAL_BITS=6

default: pocketspin spin
clean:
	rm pocketspin spin

pocketspin: spin.cpp $(MAPLE_SUBDIR)/object_data.cpp spin_gl.cpp spin_gl.h $(MAPLE_SUBDIR)/pocketgl.h $(MAPLE_SUBDIR)/pocketgl.cpp
	g++ $(OPT) -Wall -I$(MAPLE_SUBDIR) spin.cpp spin_gl.cpp $(MAPLE_SUBDIR)/object_data.cpp $(MAPLE_SUBDIR)/pocketgl.cpp -DVSGL_ON_OPENGL $(SUBPIXEL) -o pocketspin -lm -lglut -lGLU -lrt

spin: spin.cpp $(MAPLE_SUBDIR)/object_data.cpp spin_gl.cpp spin_gl.h
	g++ $(OPT) -Wall -I$(MAPLE_SUBDIR) spin.cpp spin_gl.cpp $(MAPLE_SUBDIR)/object_data.cpp -o spin -lm -lglut -lGLU -lrt
