#include "float3.h"
#include <math.h>


float3 init_float3(float x, float y, float z){
  float3 f;
  f.x=x;
  f.y=y;
  f.z=z;
  return f;

}

float3 mul_float3(float3 f,float n){
  float3 f_mul;
  f_mul.x = f.x*n;
  f_mul.y = f.y*n;
  f_mul.z = f.z*n;
  return f_mul;
}

float distance_point(float3 p1, float3 p2){
  return sqrt(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2)+pow(p2.z-p1.z,2));
}

float produit_scalaire(float3 p1, float3 p2){
  return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}

float3 produit_vectoriel(float3 v1, float3 v2){
  float3 pv;
  pv.x= v1.y*v2.z - v1.z*v2.y;
  pv.y= v1.z*v2.x - v1.x*v2.z;
  pv.z= v1.x*v2.y - v1.y*v2.x;
  return pv;
}

float3 rodrigues(float angle,float3 v1, float3 v2){
  /* Application de la formule de Rodrigues (Rotation vectorielle) */
  /* Rotation de v1 autour de v2 d'un angle angle */
  
  float3 v;
  float3 pv=produit_vectoriel(v2,v1);
  float ps =produit_scalaire(v1,v2);
  
  v.x=  cos(angle*M_PI/180) *v1.x
    + (1-cos(angle*M_PI/180)) * ps * v2.x
    + (sin(angle*M_PI/180)) * pv.x;

  v.y=  cos(angle*M_PI/180) *v1.y
    + (1-cos(angle*M_PI/180)) * ps * v2.y
    + (sin(angle*M_PI/180)) * pv.y;

  v.z=  cos(angle*M_PI/180) *v1.z
    + (1-cos(angle*M_PI/180)) * ps * v2.z
    + (sin(angle*M_PI/180)) * pv.z;

  return v;
}
