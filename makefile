OPTIONS=-Wall -g -O0
GLUT=-lglut -lGLU -lGL -lm
CC = gcc
all: proj

proj: main.o karbre.o float3.o draw.o construction_karbre.o
	$(CC) $(OPTIONS) main.o karbre.o float3.o draw.o construction_karbre.o -o proj $(GLUT)

main.o: main.c karbre.h
	$(CC) $(OPTIONS) -c main.c $(GLUT)

karbre.o: karbre.c karbre.h
	$(CC) $(OPTIONS) -c karbre.c $(GLUT)

float3.o: float3.c float3.h
	$(CC) $(OPTIONS) -c float3.c $(GLUT)

draw.o: draw.c draw.h
	$(CC) $(OPTIONS) -c draw.c $(GLUT)

construction_karbre.o: construction_karbre.c construction_karbre.h
	$(CC) $(OPTIONS) -c construction_karbre.c $(GLUT)

clean:
	rm -rf *.o proj
