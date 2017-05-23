#include "karbre.h"
#include "float3.h"
#include "draw.h"
#include "construction_karbre.h"
#include "util.h"

#define LARGEUR_MAP 1000 // X <==> i 
#define LONGUEUR_MAP 1000 // Y <==> j

#define VIT_MAX 5
#define VIT_MIN 1
#define MUN_MAX 50
#define VIE_MAX 100
int barre_mur=1;

/* Variables start truc jaune */
float start_ajout_x;
float start_ajout_y;
int start=0;
int first_pass=1;
float3 tab_ennemi[10];
int tab_chute_ennemi[10][3];
// 0 - est en train de tomber
// 1 - angle de chute
// 2 - vitesse



karbre karbre8;
int r_1,r_2,r_3;
int seuil;
// Variables detection collision
float r_sphere_joueur;
int COL_DET;
int ARR;
// Variables décors
int nuages;

float3 tab_arbre[500];
float scale_arbre[500];
float tab_lait[200][2];
//float tab_decors[500][500];
float ** tab_decors;
int rand_seed;
int nuages_toggle=1;
int auto_scroll_toggle=1;
int arbre=1;

int cote_projectile;
int taille_projectile;
// Variables d'animation

int angle=0;
float angle_tangage=0;

//touches
int key_q, key_d, key_s, key_z, key_m, key_p;//,key_haut,key_bas,key_droite,key_gauche;
int key_haut,key_bas,key_droite,key_gauche;
//int key_haut,key_gauche;
//int abc=0;

int dans_vaisseau=1;
int a_portee_vaisseau=1;
/* V_CAM: position de la caméra*/
float3 V_OBJ_CAM;
/* V_POS: position du joueur-vaisseau */
float3 V_POS;
/* S_VIT : vitesse du joueur-vaisseau */
float S_VIT;


/* V_POS: position du joueur-a pied */
float3 V_POS_P;
/* S_VIT : vitesse du joueur-a pied */
float S_VIT_P;

float vie;
int mun;
float3 p2;
float3 p1_hyperC,p2_hyperC;
float3 p1_hyperC2,p2_hyperC2;

float3 V_DIR;
float3 V_90;
float3 V_UP;

float3 V_DIR_P;
float3 V_90_P;
float3 V_UP_P;

float3 V_UP_INIT;
float3 V_90_INIT;
float3 tab_proj[50][3];
float3 V_EYE;



void ennemis(float3 centre){
  int i;
  for( i=0;i<10;i++){
    
    if (tab_ennemi[i].y==-1){
      tab_ennemi[i].x=centre.x;
      tab_ennemi[i].y=centre.y;
      tab_ennemi[i].z=centre.z;
      tab_chute_ennemi[i][2]=((float)(rand()%30)/(float)10)+1;//vitesse
      tab_chute_ennemi[i][1]=0;
      tab_chute_ennemi[i][0]=0;
      return;
    }
  }
}



void intersection_munition(){
  float3 m1_lait;
  float3 m2_lait;
  for(int i=0; i<200; i++){
    m1_lait=init_float3(tab_lait[i][0]-2,tab_lait[i][1]-2,tab_decors[(int)tab_lait[i][0]][(int)tab_lait[i][1]]-2);
    m2_lait=init_float3(tab_lait[i][0]+2,tab_lait[i][1]+2,tab_decors[(int)tab_lait[i][0]][(int)tab_lait[i][1]]+2);
    affiche_cube(m1_lait,m2_lait);
    
    if (dans_cube(V_POS,m1_lait,m2_lait)
      || dans_cube(init_float3(V_POS.x+10*V_DIR.x,
			       V_POS.y+10*V_DIR.y,
			       V_POS.z+10*V_DIR.z),m1_lait,m2_lait)
      || dans_cube(init_float3(V_POS.x+10*V_90.x,
			       V_POS.y+10*V_90.y,
			       V_POS.z+10*V_90.z),m1_lait,m2_lait)
      
      || dans_cube(init_float3(V_POS.x-10*V_90.x,
			       V_POS.y-10*V_90.y,
			       V_POS.z-10*V_90.z),m1_lait,m2_lait)
      
      || dans_cube(init_float3(V_POS.x+10*V_UP.x,
			       V_POS.y+10*V_UP.y,
			       V_POS.z+10*V_UP.z),m1_lait,m2_lait)

	){
      printf("INTERSLAIT\n");
      //ICI +MUNITION
      //FAIRE DISPARAITRE LA MUNITION
      tab_lait[i][0]=-1;
      mun=clamp_min_max_f(mun+5,0,50);

    }
    
  }

  if (INTERS_TRONC==1){
    S_VIT=0;
    INTERS_TRONC=0;
    vie-=30;
  }

}

int intersection_proj_ennemi(float3 proj[3]){
  int i;
  for(i=0; i<10; i++){
    if(distance_point(proj[1],proj[1])/2 + distance_point(init_float3(tab_ennemi[i].x-6*tab_chute_ennemi[i][2],tab_ennemi[i].y-6*tab_chute_ennemi[i][2],tab_ennemi[i].z-6*tab_chute_ennemi[i][2]),init_float3(tab_ennemi[i].x+6*tab_chute_ennemi[i][2],tab_ennemi[i].y+6*tab_chute_ennemi[i][2],tab_ennemi[i].z+6*tab_chute_ennemi[i][2]))/2 >
       distance_point(milieu_cube(proj[1],proj[2]),tab_ennemi[i])){
      tab_chute_ennemi[i][0]=1;
      return 1;
    }

  }
  return 0;
}

void intersection_ennemi_ennemi(){
  int i,j;
  for(i=0; i<10; i++){
    
    for(j=1; j<10; j++){
      // -5 valeur arbitraire pour ne pas avoir de gros écarts;
      if(distance_point(init_float3(tab_ennemi[i].x-6*tab_chute_ennemi[i][2],tab_ennemi[i].y-6*tab_chute_ennemi[i][2],tab_ennemi[i].z-6*tab_chute_ennemi[i][2]),init_float3(tab_ennemi[i].x+6*tab_chute_ennemi[i][2],tab_ennemi[i].y+6*tab_chute_ennemi[i][2],tab_ennemi[i].z+6*tab_chute_ennemi[i][2]))/2 +
	 distance_point(init_float3(tab_ennemi[(i+j)%10].x-6*tab_chute_ennemi[(i+j)%10][2],tab_ennemi[(i+j)%10].y-6*tab_chute_ennemi[(i+j)%10][2],tab_ennemi[(i+j)%10].z-6*tab_chute_ennemi[(i+j)%10][2]),init_float3(tab_ennemi[(i+j)%10].x+6*tab_chute_ennemi[(i+j)%10][2],tab_ennemi[(i+j)%10].y+6*tab_chute_ennemi[(i+j)%10][2],tab_ennemi[(i+j)%10].z+6*tab_chute_ennemi[(i+j)%10][2]))/2 >
	 distance_point(tab_ennemi[i],tab_ennemi[(i+j)%10])+10){
	tab_chute_ennemi[i][0]=1;
	tab_chute_ennemi[(i+j)%10][0]=1;
      }
    }
    /* Collision avec nous, On decale de 10*V90 ou on traverse*/
    if(distance_point(init_float3(tab_ennemi[i].x-6*tab_chute_ennemi[i][2],tab_ennemi[i].y-6*tab_chute_ennemi[i][2],tab_ennemi[i].z-6*tab_chute_ennemi[i][2]),init_float3(tab_ennemi[i].x+6*tab_chute_ennemi[i][2],tab_ennemi[i].y+6*tab_chute_ennemi[i][2],tab_ennemi[i].z+6*tab_chute_ennemi[i][2]))/2 >
       distance_point(tab_ennemi[i],V_POS)){
      tab_chute_ennemi[i][0]=1;
      V_POS.x-=30*V_DIR.x;
      V_POS.y-=30*V_DIR.y;
      V_POS.z-=30*V_DIR.z;
      vie-=5;
    }
  }
}
void start_anim(){
  
  if (start_ajout_x<=LARGEUR_MAP/2){

    glBegin(GL_QUADS);
    glVertex3f(LARGEUR_MAP/2-20-start_ajout_x,LONGUEUR_MAP/2-20-start_ajout_y,-100);
    glVertex3f(LARGEUR_MAP/2-20-start_ajout_x,LONGUEUR_MAP/2+20+start_ajout_y,-100);
    glVertex3f(LARGEUR_MAP/2-20-start_ajout_x,LONGUEUR_MAP/2+20+start_ajout_y,800);
    glVertex3f(LARGEUR_MAP/2-20-start_ajout_x,LONGUEUR_MAP/2-20-start_ajout_y,800);

    glVertex3f(LARGEUR_MAP/2-20-start_ajout_x,LONGUEUR_MAP/2+20+start_ajout_y,-100);
    glVertex3f(LARGEUR_MAP/2+20+start_ajout_x,LONGUEUR_MAP/2+20+start_ajout_y,-100);
    glVertex3f(LARGEUR_MAP/2+20+start_ajout_x,LONGUEUR_MAP/2+20+start_ajout_y,800);
    glVertex3f(LARGEUR_MAP/2-20-start_ajout_x,LONGUEUR_MAP/2+20+start_ajout_y,800);
     
    glVertex3f(LARGEUR_MAP/2-20-start_ajout_x,LONGUEUR_MAP/2-20-start_ajout_y,-100);
    glVertex3f(LARGEUR_MAP/2+20+start_ajout_x,LONGUEUR_MAP/2-20-start_ajout_y,-100);
    glVertex3f(LARGEUR_MAP/2+20+start_ajout_x,LONGUEUR_MAP/2-20-start_ajout_y,800);
    glVertex3f(LARGEUR_MAP/2-20-start_ajout_x,LONGUEUR_MAP/2-20-start_ajout_y,800);
     
    glVertex3f(LARGEUR_MAP/2+20+start_ajout_x,LONGUEUR_MAP/2-20-start_ajout_y,-100);
    glVertex3f(LARGEUR_MAP/2+20+start_ajout_x,LONGUEUR_MAP/2+20+start_ajout_y,-100);
    glVertex3f(LARGEUR_MAP/2+20+start_ajout_x,LONGUEUR_MAP/2+20+start_ajout_y,800);
    glVertex3f(LARGEUR_MAP/2+20+start_ajout_x,LONGUEUR_MAP/2-20-start_ajout_y,800);

    glEnd();
    if (start==1){
      
  if (first_pass){
    vie= VIE_MAX;
    mun = MUN_MAX;
    first_pass = 0;
  }
      start_ajout_x+=((float)LARGEUR_MAP/(float)LONGUEUR_MAP)*5;
      start_ajout_y+=((float)LONGUEUR_MAP/(float)LARGEUR_MAP)*5;
    }
  }
  else{
    start=2;

    
    ennemis(init_float3(rand()%LARGEUR_MAP,0,150+rand()%150));
    //active collision
    //active le jeu


  }
}


void mur_ennemi(){
  int i;
  /* mur apparition */
  glBegin(GL_LINES);
  if (start==2){
    glColor4f(0,1,1,1);
    for(i=0; i<LARGEUR_MAP; i=i+10){
      glVertex3f(i,0,0);
      glVertex3f(i,0,300);
    }
    glVertex3f(0,0,barre_mur);
    glVertex3f(LARGEUR_MAP,0,barre_mur);
    barre_mur++;
    barre_mur= barre_mur%300;


  
    glEnd();
    
    glColor4f(0.1,0.1,0.2,0.5);
    glBegin(GL_QUADS);
    glVertex3f(0,LONGUEUR_MAP,0);
    glVertex3f(LARGEUR_MAP,LONGUEUR_MAP,0);
      
    glVertex3f(LARGEUR_MAP,LONGUEUR_MAP,300);
    glVertex3f(0,LONGUEUR_MAP,300);

    glEnd();
  

    /* mur disparition */
  }


}

int clamp_min_max(int n, int min, int max){
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
  int max_i=LARGEUR_MAP;
  int max_j=LONGUEUR_MAP;
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
    tab_arbre[i].x=rand()%(LARGEUR_MAP-20)+10;
    tab_arbre[i].y=rand()%(LONGUEUR_MAP-20)+10;
    tab_arbre[i].z=1; /* TYPE = ARBRE, on utilise tab_decors pour la hauteur*/
    scale_arbre[i]=(float)(rand()%20)/(float)10;

  }
  int indice=200;
  for(i=0; i<29; i++){// 10*15
    int randx=rand()%(LARGEUR_MAP-10)+5;
    int randy=rand()%(LONGUEUR_MAP-10)+5;
    tab_arbre[indice].x=randx;
    tab_arbre[indice].y=randy;
    tab_arbre[indice].z=1;
    scale_arbre[indice]=(float)(rand()%20)/(float)10;
    indice++;
    for(j=0;j<9;j++){
      int signe=rand()%2-1;
      if (signe==0) signe=1;
      tab_arbre[indice].x=clamp_min_max(randx+signe*rand()%30,10,LARGEUR_MAP-10);
      signe=rand()%2-1;
      if (signe==0) signe=1;
      tab_arbre[indice].y=clamp_min_max(randy+signe*rand()%30,10,LONGUEUR_MAP-10);
      indice++;
      tab_arbre[indice].z=1;
      scale_arbre[indice]=(float)(rand()%20)/(float)10;
    }
    
  }
  arbre++;
  
}


void init_lait(){
  int centreX = LARGEUR_MAP / 4;
  int centreY = LONGUEUR_MAP / 4;
  int i;
  for(i=0; i<200; i++){
    tab_lait[i][0]=rand()%(LARGEUR_MAP-centreX)+centreX/2;
    tab_lait[i][1]=rand()%(LONGUEUR_MAP-centreY)+centreY/2;
  }
}

void dessin_lait(){
  int i;
  for(i=0; i<200; i++){
    if (tab_lait[i][0]==-1){
      int centreX = LARGEUR_MAP / 4;
      int centreY = LONGUEUR_MAP / 4;
      tab_lait[i][0]=rand()%(LARGEUR_MAP-centreX)+centreX/2;
      tab_lait[i][1]=rand()%(LONGUEUR_MAP-centreY)+centreY/2;
    }
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
  if (dans_vaisseau){ // Input quand on est dans le vaiseau
    if (key_s == 1) {
      /*
	V_POS.z-=V_DIR.z;
	V_POS.x-=V_DIR.x;
	V_POS.y-=V_DIR.y;
      */
      S_VIT--;
      if (COL_DET == 0) {
	if (S_VIT < VIT_MIN ) {
	  S_VIT = VIT_MIN;
	}
      }
      else {
	if (S_VIT < 0 ) {
	  S_VIT = 0;
	}
      }
    }
    if (key_z == 1){
 
      // MAJ DU CUBE

      /*
	V_POS.x+=V_DIR.x;
	V_POS.z+=V_DIR.z;
	V_POS.y+=V_DIR.y;
      */
      S_VIT++;
      if (COL_DET == 1 )
	S_VIT -= .5;
      if (abs(S_VIT)>VIT_MAX)
	S_VIT=VIT_MAX;

    }
    
    if (key_q == 1){
      /* FORMULE DE RODRIGUES */
      /* Rotation autour de l'axe DIR du VECTEUR_90*/
      V_90=rodrigues(-1.2,V_90,V_DIR);
      /* On reactualise le V_UP */
      V_UP=produit_vectoriel(V_DIR,V_90);
 
    }
    if (key_d == 1){
      /* Rotation autour de l'axe DIR du VECTEUR_90*/
      V_90=rodrigues(1.2,V_90,V_DIR);
      /* On reactualise le V_UP */
      V_UP=produit_vectoriel(V_DIR,V_90);
    }
  
    if (key_bas == 1){
      V_DIR=rodrigues(-0.8,V_DIR,V_90);
      V_UP=produit_vectoriel(V_DIR,V_90);
    }
  
    if (key_haut == 1){
      V_DIR=rodrigues(0.8,V_DIR,V_90);
      V_UP=produit_vectoriel(V_DIR,V_90);
    }
  
    if (key_droite == 1) {
      V_DIR=rodrigues(-0.8,V_DIR,V_UP);
      V_90=produit_vectoriel(V_UP,V_DIR);
    }
  
    if (key_gauche == 1) {
      V_DIR=rodrigues(0.8,V_DIR,V_UP);
      V_90=produit_vectoriel(V_UP,V_DIR);
    }

  }
  else { // input hors du vaisseau
    if (key_s == 1) {
      V_POS_P=f3_add_f3(V_POS_P,mul_float3(V_DIR_P,-1));
    }
    if (key_z == 1){
      V_POS_P=f3_add_f3(V_POS_P,V_DIR_P);
    }
    
    if (key_q == 1){
      V_POS_P=f3_add_f3(V_POS_P,mul_float3(V_90_P,1));
    }
    if (key_d == 1){
      V_POS_P=f3_add_f3(V_POS_P,mul_float3(V_90_P,-1));
    }
  
    if (key_bas == 1){
      V_DIR_P=rodrigues(0.8,V_DIR_P,V_90_P);
      V_UP_P=produit_vectoriel(V_DIR_P,V_90_P);      
    }
  
    if (key_haut == 1){
      V_DIR_P=rodrigues(-0.8,V_DIR_P,V_90_P);
      V_UP_P=produit_vectoriel(V_DIR_P,V_90_P);      
    }
  
    if (key_droite == 1) {      
      V_DIR_P=rodrigues(-0.8,V_DIR_P,V_UP_P);
      V_90_P=produit_vectoriel(V_UP_P,V_DIR_P);      
    }
  
    if (key_gauche == 1) {
      V_DIR_P=rodrigues(0.8,V_DIR_P,V_UP_P);
      V_90_P=produit_vectoriel(V_UP_P,V_DIR_P);      
    }

  }
  glutPostRedisplay();
}

void affichage_pt(float3 p1){
  printf("point: x=%f y=%f z=%f\n",p1.x,p1.y,p1.z);
}

void affichage(){
  int i;
  int val = 0;

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

  /*

_________________________
|                        |
|                        |
|                        |               -10
|                        |                ^
|                        |                "
|                        |                "
|           X            |     -10   <=========>  10   Frustum en mode 800 x 800
|                        |                "
|                        |                "
|                        |                v
|                        |               10
|________________________|


Frustum en 1920 x 1080 ?? 

800 --> 20     |   800 --> 20
1920 --> 48    |   1080 --> 27


   */

  
  glFrustum(-24,24,-13,13,20,1000);
  //gluPerspective(100,1,0.1,1000);


  
  if (!auto_scroll_toggle)
    {
      //       printf("vit : %f \n",mul_float3(V_DIR,S_VIT).z);
      /* if (COL_DET == 0) {
	if (S_VIT < VIT_MIN ) {
	  S_VIT = VIT_MIN;
	}
	}*/
  V_POS = f3_add_f3(V_POS,mul_float3(V_DIR,S_VIT));

    }
  if (dans_vaisseau) {
    V_POS_P.x = V_POS.x;
    V_POS_P.y = V_POS.y;
    V_POS_P.z = V_POS.z;

    V_DIR_P.x=V_DIR.x;
    V_DIR_P.y=V_DIR.y;
    V_DIR_P.z=V_DIR.z;

    V_UP_P.x=V_UP.x;
    V_UP_P.y=V_UP.y;
    V_UP_P.z=V_UP.z;

    V_90_P.x=V_90.x;
    V_90_P.y=V_90.y;
    V_90_P.z=V_90.z;
  }
  
  V_OBJ_CAM.x = V_POS_P.x;
  V_OBJ_CAM.y = V_POS_P.y;
  V_OBJ_CAM.z = V_POS_P.z;

    
  V_EYE.x=V_POS_P.x-30*V_DIR_P.x +  V_UP_P.x*8;
  V_EYE.y=V_POS_P.y-30*V_DIR_P.y +  V_UP_P.y*8;
  V_EYE.z=V_POS_P.z-30*V_DIR_P.z +  V_UP_P.z*8;


  /* Gestion des collisions de l'oeil avec le sol*/
   
 if (V_EYE.x > 0 && V_EYE.y > 0 && V_EYE.y < LONGUEUR_MAP && V_EYE.x < LARGEUR_MAP){ // On est bien à l'intérieur de la map
    if (V_EYE.z <=tab_decors[(int)V_EYE.x][(int)V_EYE.y]+1) { // On est au niveau du sol ( ou en-dessous!)
      /* Traitement en cas de collision avec le sol :*/
      V_EYE.z=tab_decors[(int)V_EYE.x][(int)V_EYE.y];
    }
 }
  
  gluLookAt(V_EYE.x,
	    V_EYE.y,
	    V_EYE.z,
	    V_OBJ_CAM.x,
	    V_OBJ_CAM.y,
	    V_OBJ_CAM.z,
	    V_UP.x,
	    V_UP.y,
	    V_UP.z
	    );

  for( i=0; i<50; i++){
    /* Methode patarasse */
    if(tab_proj[i][0].x!=-5000){
      if (distance_point(tab_proj[i][1],V_POS)>1000){
	tab_proj[i][0].x=-5000;
      }
      else{
	
	if(intersection_proj_ennemi(tab_proj[i])==1){
	  tab_proj[i][0].x=-5000;
	  fprintf(stderr,"JINTERSEQUTE\n");
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
  }

  
  for( i=0; i<10; i++){
    /* Methode patarasse */
    if(tab_ennemi[i].y!=-1){
      if (tab_ennemi[i].y>=LONGUEUR_MAP+100 || tab_ennemi[i].z<0){
	tab_ennemi[i].y=-1;
      }
      else{
	tab_ennemi[i].y+=tab_chute_ennemi[i][2];
	glPushMatrix();
	if(tab_chute_ennemi[i][0]==1){
	  tab_ennemi[i].z-=1*((float)tab_chute_ennemi[i][1]/(float)50);
	  glTranslatef(tab_ennemi[i].x,tab_ennemi[i].y,tab_ennemi[i].z);
	  glRotatef(-tab_chute_ennemi[i][1],1,0,0);
	  tab_chute_ennemi[i][1]+=1;
	  glTranslatef(-tab_ennemi[i].x,-tab_ennemi[i].y,-tab_ennemi[i].z);

	}
	
	affiche_cube_plein(init_float3(tab_ennemi[i].x-6*tab_chute_ennemi[i][2],tab_ennemi[i].y-6*tab_chute_ennemi[i][2],tab_ennemi[i].z-6*tab_chute_ennemi[i][2]),init_float3(tab_ennemi[i].x+6*tab_chute_ennemi[i][2],tab_ennemi[i].y+6*tab_chute_ennemi[i][2],tab_ennemi[i].z+6*tab_chute_ennemi[i][2]),1);
	glPopMatrix();
      }
    }
  }
  
  
  /*
    glColor4f(0,1,1,0.5);
    glVertex3f( 0,0,0);
    glVertex3f( 100,0,0);*/

  /*
    glColor4f(1,0,0.5,0.5);
  
    glVertex3f( 0,0,0);
    glVertex3f(V_90.x*100,V_90.y*100,V_90.z*100);
  
    glColor4f(0,1,1,0.5);
    glVertex3f( 0,0,0);
    glVertex3f( V_UP.x*100, V_UP.y*100, V_UP.z*100);
  
  
    glColor4f(1,1,0,0.5);
    glVertex3f( 0,0,0);
    glVertex3f( V_DIR.x*100, V_DIR.y*100, V_DIR.z*100);
  */

  intersection_munition();


  
  /* Mise à jour du vecteur position */

 

  /* Gestion des collisions + gravité joueur */
  if (dans_vaisseau){
  
    if (V_POS.x > 0 && V_POS.y > 0 && V_POS.y < LONGUEUR_MAP && V_POS.x < LARGEUR_MAP){ // On est bien à l'intérieur de la map
      if (V_POS.z <=tab_decors[(int)V_POS.x][(int)V_POS.y]+1) { // On est au niveau du sol ( ou en-dessous!)
	/* Traitement en cas de collision avec le sol :*/
   
	V_POS.z=tab_decors[(int)V_POS.x][(int)V_POS.y];
	if (S_VIT > 2 && ARR==1 ) { /* On perd de la vie si on va trop vite et tant qu'on ne s'est pas arrêté */
	  vie--;
	}
	COL_DET=1;

	S_VIT*=.8;
	if (S_VIT < VIT_MIN ) {
	  ARR = 0 ;
	}
	S_VIT = clamp_min_max_f(S_VIT,0,VIT_MAX); /* Dans tous les cas on réduit la vitesse */
      }
      else {
	/* Traitement du cas courant : vol sans turbulences */
	/* Prise en compte de la gravité : si le nez pointe vers le bas : acceleration sinon ralentissement */
	float ps = produit_scalaire (V_UP_INIT,V_DIR);
	S_VIT -= ps/4;
	S_VIT = clamp_min_max_f(S_VIT,VIT_MIN,VIT_MAX);
	COL_DET=0;
	ARR=1;
      }
    }
  }
  else {
    
    if (V_POS_P.x > 0 && V_POS_P.y > 0 && V_POS_P.y < LONGUEUR_MAP && V_POS_P.x < LARGEUR_MAP){ // On est bien à l'intérieur de la map
      V_POS_P.z=tab_decors[(int)V_POS_P.x][(int)V_POS_P.y]+1;      
    }
  }
  /* Dessin du 'vaisseau ' après mise à jour du vecteur position pour effet vitesse accru */

  
  glBegin(GL_LINES);
    
  val=0;
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

  if (!dans_vaisseau){
    float3 h;
    h.x=1;
    h.y=1;
    h.z=1;
    affiche_cube(V_POS_P,f3_add_f3(V_POS_P,h));
  }

  
  /*
  for(i=0; i<LARGEUR_MAP; i=i+10){
    glVertex3f(i,0,0);
    glVertex3f(i,0,300);
  }

  
  glVertex3f(0,0,pppp);
  glVertex3f(LARGEUR_MAP,0,pppp);
      

  pppp++;
  pppp= pppp%300;
  */
  glEnd();

  mur_ennemi();


  /* on pourra faire fonction generale avec le .z > type */

  affiche_karbre_clipping(karbre8,V_POS,V_DIR,V_UP,V_90);
  
  //dessin_arbre();
  dessin_lait();

  

  glColor4f(1,1,0,0.7);

  
  if(V_POS.x>LARGEUR_MAP/2-20 && V_POS.x<LARGEUR_MAP/2+20
     && V_POS.y>LONGUEUR_MAP/2-20 && V_POS.y<LONGUEUR_MAP/2+20 && start==0)
    start=1;
  start_anim();

  
  glEnd();

  intersection_ennemi_ennemi();
  dessin_grille();



  
  /* Dessin de l'interface */
  if (start!=0) {
    
  /* Dessin jauge de vie */
  dessin_jauge(5,5,130,50,vie,BLEU,GRIS,ROUGE);

  /* Dessin des munitions restantes */
  dessin_munitions(150,5,130,50,mun,MUN_MAX,NOIR,GRIS);
  }
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
  if (key == 32){ // touche espace
    if (mun > 0 ){
      projectile(V_DIR,V_POS);
      //   fprintf(stderr,"VEC: %f %f %f \n",V_DIR.x+cote_projectile,V_DIR.y+cote_projectile,V_DIR.z+cote_projectile);
      mun --;
    }
  }
  if (key=='n'){
    if(nuages_toggle) nuages_toggle=0;
    else nuages_toggle=1;
  }
  
  if (key=='b'){
    if(auto_scroll_toggle) auto_scroll_toggle=0;
    else auto_scroll_toggle=1;
  }
  if (key==13){ // touche entrée
    int modif=0;
    //    printf("%f\n",S_VIT);
    if (dans_vaisseau && COL_DET && S_VIT==0.0){
      //      printf("ça sort !! \n");
      modif=toggle(modif);
    }
    if (!dans_vaisseau && a_portee_vaisseau && !modif){
      //      printf("ça rentre\n");
      modif=toggle(modif);
    }

    if (modif)
      dans_vaisseau=toggle(dans_vaisseau);
    
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
  vie=100;
  mun = 50;
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
  COL_DET=0;
  ARR=0;
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

  //  V_90_INIT = V_90;
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
    mountain(rand()%LARGEUR_MAP,rand()%LONGUEUR_MAP,rand()%150+30);
  
  for (  i = 0 ; i < 28; i++)   aplanir();

  
  init_arbres();

  karbre8=cons_arbre(tab_arbre,scale_arbre,500,tab_decors);
  
  init_lait();

  for (i=0; i<10; i++){
    tab_ennemi[i]=init_float3(-5000,-1,-5000);
    tab_chute_ennemi[10][0]=0;
    tab_chute_ennemi[10][1]=0;
  }
  /*

    fprintf(stderr,"VECT: %f %f %f \n",V_UP.x,V_UP.y,V_UP.z);
    fprintf(stderr,"VEC90: %f %f %f \n",V_90.x,V_90.y,V_90.z);
    fprintf(stderr,"VEC: %f %f %f \n",V_DIR.x,V_DIR.y,V_DIR.z);
  */

  
  glutInit(&argc,argv);
  //Init affichage

  glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE|GLUT_DEPTH);

  //taille & pos
  //glutInitWindowSize(800,800); //ou fullscreen
    // glutInitWindowPosition(50, 50);

  glutInitWindowSize(1920,1080);


  
  glutCreateWindow("FIGHT MILK");
  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  glEnable(GL_DEPTH_TEST);

  /*
  
            glEnable(GL_FOG) ;
            GLfloat fogcolor[4] = {0.1,0.1,0.2,0.2} ;
            GLint fogmode = GL_EXP ;
            glFogi (GL_FOG_MODE, fogmode) ;
            glFogfv(GL_FOG_COLOR, fogcolor) ;
            glFogf(GL_FOG_DENSITY, 0.005) ;
            glFogf(GL_FOG_START, 29.0) ;
            glFogf(GL_FOG_END, 3.0) ;
  
  */
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
