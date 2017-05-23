#include "karbre.h"
#include "float3.h"

#define BLEU 1
#define GRIS 2
#define ROUGE 3

void affiche_cube_plein(float3 p1, float3 p2, float opacity);
void affiche_cube(float3 p1, float3 p2);
void dessin_carton_lait();
void dessin_arbre2();
void couleur(int c);
void dessin_jauge( float coord_x, float coord_y,float largeur, float hauteur, float valeur, int couleur_FG, int couleur_BG, int couleur_vide);
float clamp_min_max_f(float n, float min, float max);
