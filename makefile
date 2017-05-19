OPTIONS=-g -Wall -std=c99 
GLUT=-lglut -lGLU -lGL -lm

all: proj

proj: main.o karbre.o float3.o draw.o
	$(CC) $(OPTIONS) main.o karbre.o float3.o draw.o -o proj $(GLUT)

main.o: main.c karbre.h
karbre.o: karbre.c karbre.h
float3.o: float3.c float3.h
draw.o: draw.c draw.h

clean:
	rm -rf *.o proj
