CC = g++

GLLIBS = -lglut -lGLEW -lGL

all: phong.cpp
	$(CC) phong.cpp ./lib/utils.cpp -o phong $(GLLIBS)

clean:
	rm -f ambient diffuse specular phong
