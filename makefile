CC=gcc
OPTIONS=-Wall -ansi -g 
GLUT=-lglut -lGLU -lGL -lm

all: proj

proj: main.o karbre.o float3.o
	$(CC) $(OPTIONS) main.o karbre.o float3.o -o proj $(GLUT)

main.o: main.c karbre.h
karbre.o: karbre.c karbre.h
float3.o: float3.c float3.h

clean:
	rm -rf *.o proj
