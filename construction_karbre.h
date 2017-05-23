#include "karbre.h"
#include "float3.h"
#include "draw.h"

int dans_cube(float3 p,float3 m1,float3 m2);
karbre cons_arbre(float3* tab_coord,float* tab_scale,int taille_coord,float** tab_decors);
karbre cons_arbre2(float3* tab_coord,float* tab_scale,int taille_coord,float** tab_decors, float3 m1, float3 m2, int n);
int cube_intersection_tab(float3* tab_coord,int taille_coord,float** tab_decors,float3 m1,float3 m2);
elem_coord elem_dans_cube(float3 coord,float** tab_decors,float3 m1,float3 m2);
void affiche_karbre_simple2(karbre k,float3 m1, float3 m2);
void affiche_karbre_simple(karbre k);

void affiche_karbre_clipping2(karbre k,float3 m1, float3 m2, float3 vdir,float3 vup, float3 v90, float3 point_clipping, float3 pt_eye,float3 pt_vaisseau);
void affiche_karbre_clipping(karbre k,float3 centre, float3 vdir,float3 vup, float3 v90);
int intersection_cime_vaisseau(float3 pt_vaisseau,elem_coord arbre, float3 v_dir, float3 v_up, float3 v_90);
