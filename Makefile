CC = g++

GLLIBS = -lglut -lGLEW -lGL

all: trabalho.cpp
	$(CC) trabalho.cpp ./lib/utils.cpp -o trabalho $(GLLIBS)

clean:
	rm -f trabalho
