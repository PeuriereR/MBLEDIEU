#include "karbre.h"
#include "float3.h"
#include "draw.h"
#include "construction_karbre.h"

#define LONGUEUR_MAP 1005
#define LARGEUR_MAP 1005
#define VIT_MAX 20
int pppp=1;

// 1005
karbre karbre8;
int r_1,r_2,r_3;
  int seuil;
// Variables detection collision
float r_sphere_joueur;

// Variables décors
int nuages;

float3 tab_arbre[500];
float tab_lait[200][2];
//float tab_decors[500][500];
float ** tab_decors;
int rand_seed;
int nuages_toggle=1;
int auto_scroll_toggle=1;
int arbre=1;
int wouah=0;

int cote_projectile;
int taille_projectile;
// Variables d'animation

float angle=0;
float angle_tangage=0;

//touches
int key_q, key_d, key_s, key_z, key_m, key_p;//,key_haut,key_bas,key_droite,key_gauche;
int key_haut,key_bas,key_droite,key_gauche;
//int key_haut,key_gauche;
//int abc=0;
/* V_POS: position du joueur */
float3 V_POS;
/* S_VIT : vitesse du joueur */
float S_VIT;
float3 p2;
float3 p1_hyperC,p2_hyperC;
float3 p1_hyperC2,p2_hyperC2;

float3 V_DIR;
float3 V_90;
float3 V_UP;
float3 V_UP_INIT;
float3 V_90_INIT;
float3 tab_proj[50][3];

int clamp_min_max(int n, int min, int max){
  if (n<min) return min;
  if (n>=max) return max;
  return n;
}


float clamp_min_max_f(float n, float min, float max){
  if (n<min) return min;
  if (n>=max) return max;
  return n;
}
int clamp(int n){
  return clamp_min_max(n,0,997);
}

float mapFloat(float nombre, float actuelInf, float actuelSup, float cibleInf, float cibleSup){
  float pourcentage = (nombre/(actuelSup-actuelInf));
  return cibleInf+pourcentage*(cibleSup-cibleInf);
}

void aplanir(){
  // indices de parcours du tableau
  int i,j;
  // indices relatifs aux cases adjacentes à la case actuelle (gauche,milieu,droite || haut,centre,bas)
  int g,d,h,b,c,m;
  int max_i=LONGUEUR_MAP;
  int max_j=LARGEUR_MAP;
  for ( i = 0 ; i < max_i ; i++){
    for ( j = 0 ; j < max_j ; j++){
      //printf("jii\n");
      //printf("%f",tab_decors[(i-1)%max_i][(j-1)%max_j]);
      g=i-1;
      d=i+1;
      h=j-1;
      b=j+1;
      m=i;
      c=j;

      if (g<0) g = max_i-1;
      if (d>=max_i) d = 0;
      if (h<0) h = max_j-1;
      if (b>=max_j) b = 0;
      //      printf("%d %d %d %d %d %d \n",h,c,b,g,m,d);
      tab_decors[i][j]=(tab_decors[g][h]+
			tab_decors[m][h]+
			tab_decors[d][h]+
			tab_decors[g][c]+
			tab_decors[d][c]+
			tab_decors[g][b]+
			tab_decors[m][b]+
			tab_decors[d][b])/8;
		        
    }
  }
  // printf("%d %d %f\n ",i,j,tab_decors[i][j]);
  
}



void init_arbres(){
  /* 200 arbres random, 29 bosquets de 1+9 arbres */
  
  int i,j;
  for(i=0; i<200; i++){
    tab_arbre[i].x=rand()%990+1;
    tab_arbre[i].y=rand()%990+1;
    tab_arbre[i].z=1; /* TYPE = ARBRE, on utilise tab_decors pour la hauteur*/

  }
  int indice=200;
  for(i=0; i<29; i++){// 10*15
    int randx=rand()%990+1;
    int randy=rand()%990+1;
    tab_arbre[indice].x=randx;
    tab_arbre[indice].y=randy;
    indice++;
    for(j=0;j<9;j++){
      int signe=rand()%2-1;
      if (signe==0) signe=1;
      tab_arbre[indice].x=clamp_min_max(randx+signe*rand()%30,10,990);
      signe=rand()%2-1;
      if (signe==0) signe=1;
      tab_arbre[indice].y=clamp_min_max(randy+signe*rand()%30,10,990);
         indice++;
    }
    
  }
  arbre++;
  
}


void init_lait(){
  int i;
  for(i=0; i<200; i++){
    tab_lait[i][0]=rand()%700+100;
    tab_lait[i][1]=rand()%700+100;
  }
}

void dessin_lait(){
  int i;
  for(i=0; i<200; i++){
    glPushMatrix();
    glTranslatef(tab_lait[i][0],tab_lait[i][1],tab_decors[(int)tab_lait[i][0]][(int)tab_lait[i][1]]);
    glScalef(2,2,2);
    dessin_carton_lait();
    glPopMatrix();
  }
}

void dessin_arbre(){
  int i;
  for(i=0; i<250; i++){
    glPushMatrix();
    glTranslatef(tab_arbre[i].x,tab_arbre[i].y,tab_decors[(int)tab_arbre[i].x][(int)tab_arbre[i].y]);
    glScalef(3,3,3);
    dessin_arbre2();
    glPopMatrix();
  }
}


void dessin_grille(){
  int bS_Larg=LARGEUR_MAP;
  int bS_Long=LONGUEUR_MAP;
  float c;
  int pas = 2;
  int p;
  int i,j;
  //  t=0;
  //  glColor3f(0.2,0.7 ,0.45 );
  //  for (int i = borne; i < -borne; i++ ){
  //afficher_axes();

  //Animation nuages
  p=rand()%100;
  if ( p < 85)
  nuages +=3;
  if (nuages>bS_Long) nuages=0;

  if (!nuages_toggle)
    nuages=0;
  
  for ( j = 0; j < bS_Long-pas; j+=pas ){

    glBegin(GL_TRIANGLE_STRIP);
    for ( i = 0; i < bS_Larg-pas; i+=pas ){

      // changement couleurs( nuages )

      //printf("(i+nuages)%(LARGEUR_MAP-1): %d  \n",(i+nuages)%(LARGEUR_MAP-1));
      // printf("(j+nuages)%(LONGUEUR_MAP-1) : %d\n",(j+nuages)%(LONGUEUR_MAP-1));
      
      c = mapFloat(tab_decors[(i+nuages)%(LARGEUR_MAP-1)][(j+nuages)%(LONGUEUR_MAP-1)],0,200,0,1);
      glColor3f(c,c,c);
      // glColor3f(.5,.5,.5);


      glVertex3f(i, j, tab_decors[i][j]);
      glVertex3f(i, j+pas, tab_decors[i][j+pas]);
      glVertex3f(i+pas, j, tab_decors[i+pas][j]);

      // changement couleurs (nuages )
      c = mapFloat(tab_decors[(i+nuages)%(LARGEUR_MAP-1)][(j+nuages)%(LONGUEUR_MAP-1)],0,200,0,1);
      glColor3f(c,c,c);
      //  glColor3f(.8,.8,.8);
      
      glVertex3f(i, j+pas, tab_decors[i][j+pas]);
      glVertex3f(i+pas, j, tab_decors[i+pas][j]);
      glVertex3f(i+pas, j+pas, tab_decors[i+pas][j+pas] );

      
    }
    glEnd();
    /*
      for ( i =borneInf; i < borneSup; i+=pas ){

      dessin_carton_lait(i,j,10,.5);
      }*/
  }

  // afficher_axes();
}

void projectile(float3 direction, float3 f){
  int i;
  float3 m1;
  float3 m2;
  m1=init_float3(f.x-taille_projectile+mul_float3(V_90,cote_projectile).x,f.y-taille_projectile+mul_float3(V_90,cote_projectile).y,f.z-taille_projectile+mul_float3(V_90,cote_projectile).z);
  m2=init_float3(f.x+taille_projectile+mul_float3(V_90,cote_projectile).x,f.y+taille_projectile+mul_float3(V_90,cote_projectile).y,f.z+taille_projectile+mul_float3(V_90,cote_projectile).z);

  cote_projectile*=-1;
  for( i=0;i<50;i++){
    
    if (tab_proj[i][0].x==-5000){
      tab_proj[i][0]=direction;
      tab_proj[i][1]=m1;
      tab_proj[i][2]=m2;
      break;
    }
  }
}

  


void gestion_input(){
  if (key_s == 1) {
    /*
    V_POS.z-=V_DIR.z;
    V_POS.x-=V_DIR.x;
    V_POS.y-=V_DIR.y;
    */
    S_VIT--;
    if (S_VIT < 0 )
      S_VIT = 0;
  }
  if (key_z == 1){
 
    // MAJ DU CUBE

    /*
    V_POS.x+=V_DIR.x;
    V_POS.z+=V_DIR.z;
    V_POS.y+=V_DIR.y;
    */
    S_VIT++;
    if (abs(S_VIT)>VIT_MAX)
      S_VIT=VIT_MAX;

  }
    
  if (key_q == 1){
      /* FORMULE DE RODRIGUES */
    /* Rotation autour de l'axe DIR du VECTEUR_90*/
    V_90=rodrigues(-.8,V_90,V_DIR);
    /* On reactualise le V_UP */
    V_UP=produit_vectoriel(V_DIR,V_90);
 
  }
  if (key_d == 1){
    /* Rotation autour de l'axe DIR du VECTEUR_90*/
    V_90=rodrigues(.8,V_90,V_DIR);
    /* On reactualise le V_UP */
    V_UP=produit_vectoriel(V_DIR,V_90);
  }
  
  if (key_bas == 1){
    key_haut=0;
    V_DIR=rodrigues(-0.8,V_DIR,V_90);
    V_UP=produit_vectoriel(V_DIR,V_90);
  }
  
  if (key_haut == 1){
    key_bas = 0;
    V_DIR=rodrigues(0.8,V_DIR,V_90);
    V_UP=produit_vectoriel(V_DIR,V_90);
  }
  
  if (key_droite == 1) {
    key_gauche=0;
    V_DIR=rodrigues(-0.8,V_DIR,V_UP);
    V_90=produit_vectoriel(V_UP,V_DIR);
  }
  
  if (key_gauche == 1) {
    key_droite=0;
    V_DIR=rodrigues(0.8,V_DIR,V_UP);
    V_90=produit_vectoriel(V_UP,V_DIR);
  }


  //  printf("%d\n",key_haut);
  glutPostRedisplay();
}

void affichage_pt(float3 p1){
  printf("point: x=%f y=%f z=%f\n",p1.x,p1.y,p1.z);
}

void affichage(){
  int i;
  /*
    angle_tangage=angle_tangage+0.2;
    if (abs(angle_tangage)>=360)
    angle_tangage=0;


    angle=angle+0.2;
    
    if (abs(angle)>=360)
    angle=0;
  */

  // Detection des collisions basique
  /*
    if((p1.z+p2.z)/2<=tab_decors[(int)(p1.x+p2.x)/2][(int)(p1.y+p2.y)/2])
    printf("x:%f y:%f z:%f\n",(p1.x+p2.x)/2,(p1.y+p2.y)/2,(p1.z+p2.z)/2);
    else printf("\n");
  */
  gestion_input();

  // float3 centre_cube=milieu_cube(p1,p2);

  glClearColor(0.1,0.1,0.2,0.2);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  glFrustum(-10,10,-10,10,10,10000);
  //gluPerspective(100,1,0.1,1000);

  gluLookAt(V_POS.x-30*V_DIR.x +  V_UP.x*8 +r_1,
	    V_POS.y-30*V_DIR.y +  V_UP.y*8 +r_2,
	    V_POS.z-30*V_DIR.z +  V_UP.z*8 +r_3,
	    V_POS.x,
	    V_POS.y,
	    V_POS.z,
	    V_UP.x,
	    V_UP.y,
	    V_UP.z
	    );

  /*
  gluLookAt(centre_cube.x-30*V_DIR.x +  V_UP.x*8 +r_1,
	    centre_cube.y-30*V_DIR.y +  V_UP.y*8 +r_2,
	    centre_cube.z-30*V_DIR.z +  V_UP.z*8 +r_3,
	    centre_cube.x,
	    centre_cube.y,
	    centre_cube.z,
	    V_UP.x,
	    V_UP.y,
	    V_UP.z
	    );

  */
  
  //  gluLookAt(-70,-20,70,10,50,10,0,0,1);


  for( i=0; i<50; i++){
    /* Methode patarasse */
    if(tab_proj[i][0].x!=-5000){
      if (distance_point(tab_proj[i][1],V_POS)>1000){
	tab_proj[i][0].x=-5000;
      }
      else{
	tab_proj[i][1].x+=5*tab_proj[i][0].x;
	tab_proj[i][1].y+=5*tab_proj[i][0].y;
	tab_proj[i][1].z+=5*tab_proj[i][0].z;
       
	tab_proj[i][2].x+=5*tab_proj[i][0].x;
	tab_proj[i][2].y+=5*tab_proj[i][0].y;
	tab_proj[i][2].z+=5*tab_proj[i][0].z;

	affiche_cube_plein(tab_proj[i][1],tab_proj[i][2],1);
	tab_proj[i][0].z-=0.01;
      }
    }
  }
  
  glBegin(GL_LINES);
  
  /*
    glColor4f(0,1,1,0.5);
    glVertex3f( 0,0,0);
    glVertex3f( 100,0,0);*/

  
  glColor4f(1,0,0.5,0.5);
  
  glVertex3f( 0,0,0);
  glVertex3f(V_90.x*100,V_90.y*100,V_90.z*100);
  
  glColor4f(0,1,1,0.5);
  glVertex3f( 0,0,0);
  glVertex3f( V_UP.x*100, V_UP.y*100, V_UP.z*100);
  
  
  glColor4f(1,1,0,0.5);
  glVertex3f( 0,0,0);
  glVertex3f( V_DIR.x*100, V_DIR.y*100, V_DIR.z*100);


    
  // printf("centre_cube.x : %f  ,centre_cube.y  %f  ,centre_cube.z  %f  \n",centre_cube.x,centre_cube.y,centre_cube.z);

  int val = 0;
  /*
  V_POS.x = f3_add_f3(V_POS,mul_float3(V_DIR,S_VIT)).x;
  V_POS.y = f3_add_f3(V_POS,mul_float3(V_DIR,S_VIT)).y;
  V_POS.z = f3_add_f3(V_POS,mul_float3(V_DIR,S_VIT)).z;
  */


  /* Prise en compte de la gravité : si le nez pointe vers le bas : acceleration sinon ralentissement */
  float ps =produit_scalaire (V_UP_INIT,V_DIR);
  //printf("ps: %f\n",ps);
  /*  if (ps  > 0) S_VIT -= ps;
    else
  */
  S_VIT-=ps;

 S_VIT= clamp_min_max_f(S_VIT,0,VIT_MAX);
 //  printf("la vit: %f\n",S_VIT);
  /* Mise à jour du vecteur position */
  V_POS = f3_add_f3(V_POS,f3_add_f3(V_DIR,mul_float3(V_DIR,S_VIT/10)));

  
  //  V_POS.y = f3_add_f3(V_DIR,mul_float(V_DIR,S_VIT)).y;
  ///  V_POS.z = f3_add_f3(V_DIR,mul_float(V_DIR,S_VIT)).z;

  //  V_POS.x = mul_float3(V_DIR,2).x;

  
  //  V_POS=f3_add_f3(V_POS,mul_float3(V_DIR,S_VIT));

  
  glColor4f(1,0,0,0.5);
  glVertex3f( V_POS.x-val,V_POS.y-val,V_POS.z-val);
  glVertex3f( V_POS.x+V_DIR.x*10-val,V_POS.y+V_DIR.y*10-val,V_POS.z+V_DIR.z*10-val);

  glColor4f(0,1,0,0.5);
  glVertex3f(V_POS.x-V_90.x*5,V_POS.y-V_90.y*5,V_POS.z-V_90.z*5);
  glVertex3f(V_POS.x+V_90.x*5,V_POS.y+V_90.y*5,V_POS.z+V_90.z*5);
 
  glColor4f(0,0,1,0.5);
  glVertex3f( V_POS.x-val,V_POS.y-val,V_POS.z-val);
  glVertex3f( V_POS.x+V_UP.x*10-val,V_POS.y+ V_UP.y*10-val,V_POS.z+ V_UP.z*10-val);

  glColor4f(0,1,1,1);

  for(i=0; i<1000; i=i+10){
    glVertex3f(i,0,0);
    glVertex3f(i,0,300);
  }

  
  glVertex3f(0,0,pppp);
  glVertex3f(1000,0,pppp);
      

  pppp++;
  pppp= pppp%300;

  glEnd();


  /* on pourra faire fonction generale avec le .z > type */

  affiche_karbre_clipping(karbre8,V_POS,V_DIR,V_UP);
  
  //dessin_arbre();
  dessin_lait();

  

  glColor4f(1,1,0,0.7);

  glBegin(GL_QUADS);
     glVertex3f(480-wouah,480-wouah,-100);
     glVertex3f(480-wouah,520+wouah,-100);
     glVertex3f(480-wouah,520+wouah,800);
     glVertex3f(480-wouah,480-wouah,800);

     glVertex3f(480-wouah,520+wouah,-100);
     glVertex3f(520+wouah,520+wouah,-100);
     glVertex3f(520+wouah,520+wouah,800);
     glVertex3f(480-wouah,520+wouah,800);
     
     glVertex3f(480-wouah,480-wouah,-100);
     glVertex3f(520+wouah,480-wouah,-100);
     glVertex3f(520+wouah,480-wouah,800);
     glVertex3f(480-wouah,480-wouah,800);
     
     glVertex3f(520+wouah,480-wouah,-100);
     glVertex3f(520+wouah,520+wouah,-100);
     glVertex3f(520+wouah,520+wouah,800);
     glVertex3f(520+wouah,480-wouah,800);

     glEnd();

     if(V_POS.x>480 && V_POS.x<520
	&& V_POS.y>480 && V_POS.y<520)
       wouah+=3;
  
  glEnd();

  
  if (auto_scroll_toggle)
    {
      V_POS.x+=.5*V_DIR.x;
      V_POS.y+=.5*V_DIR.y;
      V_POS.z+=.5*V_DIR.z;

    }

  
  dessin_grille();

  glutSwapBuffers();

}
void keyUp (unsigned char key, int x, int y) {
  if (key == 'q') {
    key_q=0;
  }
  if (key == 'd'){
    key_d=0;
  }
  if (key == 'z'){
    key_z=0;
  }
  if (key == 's'){    
    key_s=0;
  }
  if (key == 'p'){    
    key_p=0;
    if (key_q==2)
      key_q=1;
    if (key_d==2)
      key_d=1;
  }
  if (key == 'm'){    
    key_m=0;
    if (key_q==2)
      key_q=1;
    if (key_d==2)
      key_d=1;
  }
}
void keyPressed (unsigned char key, int x, int y) {
  printf("key : %d\n",key);
  if (key == 'q') {
    key_q=1;
  }
  if (key == 'd'){
    key_d=1;
  }
  if (key == 'z'){
    key_z=1;
  }
  if (key == 's'){    
    key_s=1;
  }
  if (key == 'p'){    
    key_p=1;
  }
  if (key == 'm'){    
    key_m=1;
  }
  if (key == 32){
    projectile(V_DIR,V_POS);
    fprintf(stderr,"VEC: %f %f %f \n",V_DIR.x+cote_projectile,V_DIR.y+cote_projectile,V_DIR.z+cote_projectile);
  }
  if (key=='n'){
    if(nuages_toggle) nuages_toggle=0;
    else nuages_toggle=1;
  }
  
  if (key=='b'){
    if(auto_scroll_toggle) auto_scroll_toggle=0;
    else auto_scroll_toggle=1;
  }

}




void specialInput(int key, int x, int y)
{
  switch(key)
    {
    case GLUT_KEY_UP:
      key_haut=1;
      break;
    case GLUT_KEY_DOWN:
      key_bas=1;
      break;
    case GLUT_KEY_LEFT:
      key_gauche=1;
      break;
    case GLUT_KEY_RIGHT:
      key_droite=1;
      break;
    };
  
}

 
void specialInputUp(int key, int x, int y)
{
  switch(key)
    {
    case GLUT_KEY_UP:
      key_haut=0;
      break;
    case GLUT_KEY_DOWN:
      key_bas=0;
      break;
    case GLUT_KEY_LEFT:
      key_gauche=0;
      break;
    case GLUT_KEY_RIGHT:
      key_droite=0;
      break;
    };
  
}
void mountain(int i_e, int j_e, int largeur){
  int i,j;
  // 150 est la hauteur max > probleme des bords
  
  for ( i = largeur ; i > 0 ;i-- )
    for ( j = largeur ; j > 0 ;j-- ){
      if(tab_decors[clamp_min_max(i_e+i,0,LARGEUR_MAP-1)][clamp_min_max(j_e+j,0,LONGUEUR_MAP-1)]+largeur<150)
	tab_decors[clamp_min_max(i_e+i,0,LARGEUR_MAP-1)][clamp_min_max(j_e+j,0,LONGUEUR_MAP-1)] +=largeur-((i+j)/2);
      if(tab_decors[clamp_min_max(i_e+i,0,LARGEUR_MAP-1)][clamp_min_max(j_e-j,0,LONGUEUR_MAP-1)]+largeur<150)
	tab_decors[clamp_min_max(i_e+i,0,LARGEUR_MAP-1)][clamp_min_max(j_e-j,0,LONGUEUR_MAP-1)] +=largeur-((i+j)/2);
      if( tab_decors[clamp_min_max(i_e-i,0,LARGEUR_MAP-1)][clamp_min_max(j_e-j,0,LONGUEUR_MAP-1)]+largeur<150)
	tab_decors[clamp_min_max(i_e-i,0,LARGEUR_MAP-1)][clamp_min_max(j_e-j,0,LONGUEUR_MAP-1)] +=largeur-((i+j)/2);
      if( tab_decors[clamp_min_max(i_e-i,0,LARGEUR_MAP-1)][clamp_min_max(j_e+j,0,LONGUEUR_MAP-1)]+largeur<150)
	tab_decors[clamp_min_max(i_e-i,0,LARGEUR_MAP-1)][clamp_min_max(j_e+j,0,LONGUEUR_MAP-1)] +=largeur-((i+j)/2);
    }
}
  

int main(int argc, char**argv){
  //INITIALISATION
  time_t t;
  int i,j;
  cote_projectile=-5;
  taille_projectile=1;
  r_1=r_2=r_3=0;
  srand((unsigned) time(&t));

  if ( (tab_decors = malloc (sizeof(float *) * LARGEUR_MAP )) == NULL ) {
    printf("Plus de mémoire :( \n");
    exit (-1);
  }
  else
    for (i=0; i< LARGEUR_MAP;i++) {
      if (( tab_decors[i]= malloc( sizeof(float) * LONGUEUR_MAP)) == NULL ) {
	printf("Plus de mémoire :( \n");
	exit (-1);	
      }
    }

  for (i=0; i< LARGEUR_MAP;i++) {
    for (j=0; j< LONGUEUR_MAP;j++) {
      tab_decors[i][j]=0;
    }
  }


  r_sphere_joueur=5;
  float3 init= init_float3(-5000,-5000,-5000);
  for( i=0;i<50;i++)
    tab_proj[i][0]=init;
  key_q=0;
  key_d=0;
  key_s=0;
  key_z=0;
  key_m=0;
  key_p=0;

  /*
    key_gauche=0;
    key_droite=0;
    key_haut=0;
    key_droite=0;
  */
  V_POS = init_float3(0,0,0);
  p2 = init_float3(10,10,10);

  V_DIR = init_float3(1,0,0);
  
  V_90 = init_float3(0,1,0);
  V_UP = init_float3(0,0,1);

  // V_90_INIT = V_90;
 V_UP_INIT = V_UP;
  
  // Initialisation décors
  
  nuages=0;
  fprintf(stderr,"VECT: %f %f %f \n",V_UP.x,V_UP.y,V_UP.z);
  fprintf(stderr,"VEC90: %f %f %f \n",V_90.x,V_90.y,V_90.z);
  fprintf(stderr,"VEC: %f %f %f \n",V_DIR.x,V_DIR.y,V_DIR.z);
    
 
  for ( i = 0 ; i < LARGEUR_MAP ; i++){
    for ( j = 0 ; j < LONGUEUR_MAP ; j++){
      rand_seed=rand()%30+1;

      tab_decors[i][j]=mapFloat((rand()/rand_seed)%5,0,5,-10,20);
    }
  }

  /*
  
  fprintf(stderr,"TEEEEEEEEEEEEEEEST\n");
  for ( i = 0 ; i < 1000 ;i++ )
    tab_decors[i][150] = -50;
  */
  fprintf(stderr,"TEEEEEEEEEEEEEEEST2\n");
  
  for ( i =0 ; i<50 ; i++)
  mountain(rand()%1000,rand()%1000,rand()%150+30);
  
  for (  i = 0 ; i < 28; i++)   aplanir();

  
  init_arbres();

  karbre8=cons_arbre(tab_arbre,tab_decors);
  
  init_lait();
  
  /*

    fprintf(stderr,"VECT: %f %f %f \n",V_UP.x,V_UP.y,V_UP.z);
    fprintf(stderr,"VEC90: %f %f %f \n",V_90.x,V_90.y,V_90.z);
    fprintf(stderr,"VEC: %f %f %f \n",V_DIR.x,V_DIR.y,V_DIR.z);
  */

  
  glutInit(&argc,argv);
  //Init affichage

  glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE|GLUT_DEPTH);

  //taille & pos
  glutInitWindowSize(800, 800); //ou fullscreen
  glutInitWindowPosition(50, 50);

  
  glutCreateWindow("FIGHT MILK");
  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  glEnable(GL_DEPTH_TEST);
  //glutMouseFunc glutKeyboardFunc
  
  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyUp);
  glutDisplayFunc(affichage);
  glutSpecialFunc(specialInput);
  glutSpecialUpFunc(specialInputUp);
  
  //gestionnaire GLUT


  glutMainLoop();
  return EXIT_SUCCESS;
}
