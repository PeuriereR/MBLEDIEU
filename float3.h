#ifndef FLOAT3_H
#define FLOAT3_H

typedef struct{ double x; double y; double z;}float3;

float3 init_float3(float x, float y, float z);
float3 mul_float3(float3 f,float n);
float3 f3_add_f3(float3 f1, float3 f2);
float distance_point(float3 p1, float3 p2);
float norme_vecteur(float3 v1);
float produit_scalaire(float3 p1, float3 p2);
float3 produit_vectoriel(float3 v1, float3 v2);
float3 rodrigues(float angle,float3 v1, float3 v2);
float3 milieu_cube(float3 f1, float3 f2);

#endif
