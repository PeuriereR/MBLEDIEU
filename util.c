#include "util.h"
float clamp_min_max_f(float n, float min, float max){
  if (n<min) return min;
  if (n>=max) return max;
  return n;
}

int max(int a, int b){
  if (a<b)
    return b;
  return a;
}

int min(int a, int b){
  if (a<b)
    return a;
  return b;
}

int toggle(int i){
  if (i==0)
    return 1;
  return 0;
}
