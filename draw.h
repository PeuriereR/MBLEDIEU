#include "karbre.h"
#include "float3.h"

#define BLEU 1
#define GRIS 2
#define ROUGE 3
#define NOIR 4
#define BLANC 5
#define FRAMBOISE 6
void affiche_cube_plein(float3 p1, float3 p2, float opacity);
void affiche_cube(float3 p1, float3 p2);
void dessin_carton_lait();
void dessin_arbre2();

void dessin_tronc();
void couleur(int c);
void dessin_jauge( float coord_x, float coord_y,float largeur, float hauteur, float valeur, int couleur_FG, int couleur_BG, int couleur_vide);
float clamp_min_max_f(float n, float min, float max);
void dessin_munitions( float coord_x, float coord_y,float largeur, float hauteur, float valeur,float val_max, int couleur_FG, int couleur_BG);
void dessin_score(float coord_x, float coord_y,float largeur, float hauteur, int valeur,  int couleur_FG, int couleur_BG, int couleur_police);

void dessin_chiffre(float coord_x, float coord_y, float largeur, float hauteur, float epaisseur, int valeur);
void dessin_rectangle(float x1, float y1, float x2, float y2);
void dessin_boussole(float3 v_pos_p, float3 v_pos);
