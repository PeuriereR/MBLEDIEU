#include "stdio.h"
#include "stdlib.h"
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

float3 f3_add_f3(float3 f1, float3 f2){
  float3 f  ;
  f.x = f1.x+f2.x;
  f.y = f1.y+f2.y;
  f.z = f1.z+f2.z;
  return f;
}

float3 div_float3(float3 f,float n){
  if (n == 0) {
    fprintf(stderr,"div_float3: division par zero\n");
    exit(-1);
  }
  return mul_float3(f,1/n);
}

float distance_point(float3 p1, float3 p2){
  return sqrt(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2)+pow(p2.z-p1.z,2));
}

float norme_vecteur(float3 v1){
  return sqrt(pow(v1.x,2)+pow(v1.y,2)+pow(v1.z,2));
}


float produit_scalaire(float3 p1, float3 p2){
  return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}
float norme(float3 f){
  float3 origine=init_float3(0,0,0);
  return distance_point(f,origine);
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

  float norme_v2 = norme(v2);
  float3 v2_norme = div_float3(v2,norme_v2);
  float3 v;
  float3 pv=produit_vectoriel(v2_norme,v1);
  float ps =produit_scalaire(v1,v2_norme);
  
  v.x=  cos(angle*M_PI/180) *v1.x
    + (1-cos(angle*M_PI/180)) * ps * v2_norme.x
    + (sin(angle*M_PI/180)) * pv.x;

  v.y=  cos(angle*M_PI/180) *v1.y
    + (1-cos(angle*M_PI/180)) * ps * v2_norme.y
    + (sin(angle*M_PI/180)) * pv.y;

  v.z=  cos(angle*M_PI/180) *v1.z
    + (1-cos(angle*M_PI/180)) * ps * v2_norme.z
    + (sin(angle*M_PI/180)) * pv.z;

  return v;
}

float3 milieu_cube(float3 f1, float3 f2){
  float3 mil;
  mil=init_float3((f1.x+f2.x)/2,(f1.y+f2.y)/2,(f1.z+f2.z)/2);
  return mil;
}
