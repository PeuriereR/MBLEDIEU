#ifndef KARBRE_H
#define KARBRE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "GL/gl.h"
#include "GL/glut.h"
#include "float3.h"
#include <math.h>
#include <time.h>

#define N 10
#define K 8
#define ARBRE 1

typedef int element;
typedef struct{
  float3 coord;
  int type;
}elem_coord;
typedef struct noeud {
  int vide;
  elem_coord elem;
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
karbre copie_karbre(karbre k);

#endif
