#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "GL/gl.h"
#include "GL/glut.h"
#include <math.h>

#define K 8

typedef int element;
typedef struct noeud {
	element vide;
	struct noeud * pere;
	struct noeud *fils[K];
}karbre_noeud;
typedef karbre_noeud * karbre;

karbre kArbreVide();
karbre kConsArbre(element e,...);
karbre kFils(int ieme, karbre a);
element kRacine (karbre a);
int kEstVide(karbre a);
void kAfficher(karbre a);
void kAfficher2(karbre a, int prof, int fils);
