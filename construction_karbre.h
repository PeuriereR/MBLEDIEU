#include "karbre.h"
#include "float3.h"
#include "draw.h"

elem_coord get_elem_dans_cube(float3* tab_coord,float tab_decors[1000][1000],float3 m1,float3 m2);
karbre cons_arbre(float3* tab_coord,float tab_decors[1000][1000]);
karbre cons_arbre2(float3* tab_coord,float tab_decors[1000][1000], float3 m1, float3 m2, int n);
int cube_intersection_tab(float3* tab_coord,float tab_decors[1000][1000],float3 m1,float3 m2);
elem_coord elem_dans_cube(float3 coord,float tab_decors[1000][1000],float3 m1,float3 m2);
void affiche_karbre2(karbre k,float3 m1, float3 m2);
void affiche_karbre(karbre k);

void affiche_karbre3(karbre k,float3 m1, float3 m2, float3 vdir, float3 p);
void affiche_karbre4(karbre k,float3 centre, float3 vdir,float3 vup);
