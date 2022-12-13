# Martins's Makefile

CC = g++
CFLAGS = -pedantic-errors -Wall -g
LDFLASGS = -lglut -lGLEW -lGL

FONTES = $(wildcard *.cpp)

LOCALLIB = ./lib/utils.cpp

OBJETOS = $(FONTES:.cpp=.o)

EXECUTAVEL = main.exe

all: $(EXECUTAVEL)
	./$(EXECUTAVEL)

$(EXECUTAVEL) : $(OBJETOS) $(LOCALLIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLASGS) 

%.o: %.cpp
	$(CC) -c $(CFLAGS) $^

clean:
	rm -r *.o

rmproper: clean
	rm -r $(EXECUTAVEL)

run: $(EXECUTAVEL)