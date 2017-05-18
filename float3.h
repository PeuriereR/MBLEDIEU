typedef struct{ double x; double y; double z;}float3;

float3 init_float3(float x, float y, float z);
float3 mul_float3(float3 f,float n);
float distance_point(float3 p1, float3 p2);
float produit_scalaire(float3 p1, float3 p2);
float3 produit_vectoriel(float3 v1, float3 v2);
float3 rodrigues(float angle,float3 v1, float3 v2);