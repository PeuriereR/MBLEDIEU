#include "karbre.h"
#include "float3.h"
#include "draw.h"
#include "construction_karbre.h"
#include "util.h"

/* PARAMETRES */

#define LARGEUR_MAP 500 // X <==> i 
#define LONGUEUR_MAP 1000 // Y <==> j
#define NB_MUNITIONS 500
#define NB_ENNEMIS 10
#define NB_MONTAGNES 150
#define NB_ARBRES 500
#define ARBRE_R 200
#define NB_BOSQUET 30 // 
#define NB_ARBRE_BOSQUET 9
/* /!\ NB_ARBRES= ARBRE_R + NB_BOSQUET*(NB_ARBRE_BOSQUET+1) /!\ */

#define VIT_MAX 5
#define VIT_MIN 1
#define MUN_MAX 50
#define VIE_MAX 100

int barre_mur=1;
// rouge variable de fin
float rouge=0;

/* Variables start jaune */
float start_ajout_x;
float start_ajout_y;
int start=0;
int first_pass=1;

int seuil;
// Variables detection collision
float3 pt_inters;
float r_sphere_joueur;
int COL_DET;
int ARR;

// Variables décors
int nuages;
karbre karbre8;

float3* tab_arbre;
float* scale_arbre;
float ** tab_lait;
float ** tab_decors;
float3* tab_ennemi;
int** tab_chute_ennemi;
// 0 - est en train de tomber
// 1 - angle de chute
// 2 - vitesse


float3 diff_pos_pos_p;
float norme_diff_pos_pos_p ;

// table de projectiles 
float3 tab_proj[50][3];


int rand_seed;
int nuages_toggle=1;
int auto_scroll_toggle=1;
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
int score;

float3 V_DIR;
float3 V_90;
float3 V_UP;

float3 V_DIR_P;
float3 V_90_P;
float3 V_UP_P;

float3 V_UP_INIT;
float3 V_90_INIT;
float3 V_EYE;


void fin(){
  if(vie<=0){
    rouge=0.8;
    karbre8=NULL;
  }
}


void ennemis(float3 centre){
  //Cree un ennemi si possible
  int i;
  for( i=0;i<NB_ENNEMIS;i++){
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
  // Permet de recuperer les munitions au sol
  float3 m1_lait;
  float3 m2_lait;
  int i;
  for(i=0; i<NB_MUNITIONS; i++){
    m1_lait=init_float3(tab_lait[i][0]-2,tab_lait[i][1]-2,tab_decors[(int)tab_lait[i][0]][(int)tab_lait[i][1]]-2);
    m2_lait=init_float3(tab_lait[i][0]+2,tab_lait[i][1]+2,tab_decors[(int)tab_lait[i][0]][(int)tab_lait[i][1]]+2);
    affiche_cube(m1_lait,m2_lait);
    
    if (dans_cube(V_POS_P,m1_lait,m2_lait)
	|| dans_cube(init_float3(V_POS_P.x+10*V_DIR.x,
				 V_POS_P.y+10*V_DIR.y,
				 V_POS_P.z+10*V_DIR.z),m1_lait,m2_lait)
	|| dans_cube(init_float3(V_POS_P.x+10*V_90.x,
				 V_POS_P.y+10*V_90.y,
				 V_POS_P.z+10*V_90.z),m1_lait,m2_lait)
      
	|| dans_cube(init_float3(V_POS_P.x-10*V_90.x,
				 V_POS_P.y-10*V_90.y,
				 V_POS_P.z-10*V_90.z),m1_lait,m2_lait)
      
	|| dans_cube(init_float3(V_POS_P.x+10*V_UP.x,
				 V_POS_P.y+10*V_UP.y,
				 V_POS_P.z+10*V_UP.z),m1_lait,m2_lait)

	){
      if (!dans_vaisseau){
	// On fait disparaitre la munition
	tab_lait[i][0]=-1;
	// On recharge
	mun=clamp_min_max_f(mun+5,0,50);
      }
    }
  }

  // Intersection avec les troncs, variable levee dans construction_karbre.c
  if (INTERS_TRONC==1){
    if (dans_vaisseau && S_VIT > 2){
      // Collision > on stoppe la vitesse
      S_VIT=0;
      INTERS_TRONC=0;
      // Pour ne pas se heurter 2 fois de suite au meme arbre
      if(distance_point(V_POS,pt_inters)>100){
	vie-=30;
      }
      pt_inters=V_POS;      
    }
  }
}

int intersection_proj_ennemi(float3 proj[3]){
  // Collision entre projectiles et les cubes ennemis
  int i;
  for(i=0; i<NB_ENNEMIS; i++){
    if(distance_point(proj[1],proj[1])/2 + distance_point(init_float3(tab_ennemi[i].x-6*tab_chute_ennemi[i][2],tab_ennemi[i].y-6*tab_chute_ennemi[i][2],tab_ennemi[i].z-6*tab_chute_ennemi[i][2]),init_float3(tab_ennemi[i].x+6*tab_chute_ennemi[i][2],tab_ennemi[i].y+6*tab_chute_ennemi[i][2],tab_ennemi[i].z+6*tab_chute_ennemi[i][2]))/2 >
       distance_point(milieu_cube(proj[1],proj[2]),tab_ennemi[i])){
      tab_chute_ennemi[i][0]=1;
      return 1;
    }
  }
  return 0;
}

void intersection_ennemi_ennemi(){
  // Deux ennemis si ils se touchent se font tomber
  int i,j;
  for(i=0; i<NB_ENNEMIS; i++){
    for(j=1; j<NB_ENNEMIS; j++){
      // 6 coefficient arbitraire pour pallier la vitesse par la taille
      if(distance_point(init_float3(tab_ennemi[i].x-6*tab_chute_ennemi[i][2],tab_ennemi[i].y-6*tab_chute_ennemi[i][2],tab_ennemi[i].z-6*tab_chute_ennemi[i][2]),init_float3(tab_ennemi[i].x+6*tab_chute_ennemi[i][2],tab_ennemi[i].y+6*tab_chute_ennemi[i][2],tab_ennemi[i].z+6*tab_chute_ennemi[i][2]))/2 +
	 distance_point(init_float3(tab_ennemi[(i+j)%10].x-6*tab_chute_ennemi[(i+j)%10][2],tab_ennemi[(i+j)%10].y-6*tab_chute_ennemi[(i+j)%10][2],tab_ennemi[(i+j)%10].z-6*tab_chute_ennemi[(i+j)%10][2]),init_float3(tab_ennemi[(i+j)%10].x+6*tab_chute_ennemi[(i+j)%10][2],tab_ennemi[(i+j)%10].y+6*tab_chute_ennemi[(i+j)%10][2],tab_ennemi[(i+j)%10].z+6*tab_chute_ennemi[(i+j)%10][2]))/2 >
	 distance_point(tab_ennemi[i],tab_ennemi[(i+j)%10])+10){
	// On les fait tomber
	tab_chute_ennemi[i][0]=1;
	tab_chute_ennemi[(i+j)%10][0]=1;
      }
    }
    /* Collision avec nous, On decale de -30*V_DIR */
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
void mur_ennemi(){
  int i;
  /* mur apparition des ennemis */
  glBegin(GL_LINES);
  
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

  /* mur de brouillard de disparition des ennemis */
  glColor4f(0.1,0.1,0.2,0.5);
  glBegin(GL_QUADS);
  
  glVertex3f(0,LONGUEUR_MAP,0);
  glVertex3f(LARGEUR_MAP,LONGUEUR_MAP,0);
      
  glVertex3f(LARGEUR_MAP,LONGUEUR_MAP,300);
  glVertex3f(0,LONGUEUR_MAP,300);

  glEnd();
}
void start_anim(){
  
  if (start_ajout_x<=LARGEUR_MAP/2){
    /* Zone jaune de lancement du jeu */
    glColor4f(1,1,0,0.7);
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
	// Initialisation
	vie= VIE_MAX;
	mun = MUN_MAX;
	score=0;
	first_pass = 0;
      }
      // On aggrandit la zone jaune
      start_ajout_x+=((float)LARGEUR_MAP/(float)LONGUEUR_MAP)*5;
      start_ajout_y+=((float)LONGUEUR_MAP/(float)LARGEUR_MAP)*5;
    }
  }
  else{
    start=2;
    // rouge!= <=> fin du jeu
    if (rouge==0){
      
      mur_ennemi();
      // On cree un ennemi si possible
      ennemis(init_float3(rand()%LARGEUR_MAP,0,150+rand()%150));
      // On applique la recuperation des munitions
      intersection_munition();

    }

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
  
}



void init_arbres(){
  /* 200 arbres random, 30 bosquets de 1+9 arbres pour 500 arbres */
  
  int i,j;
  /* Arbres random */
  for(i=0; i<ARBRE_R; i++){
    tab_arbre[i].x=rand()%(LARGEUR_MAP-20)+10;
    tab_arbre[i].y=rand()%(LONGUEUR_MAP-20)+10;
    tab_arbre[i].z=1; /* TYPE = ARBRE, on utilise tab_decors pour la hauteur*/
    scale_arbre[i]=(float)(rand()%20)/(float)10;
  }
  
  int indice=ARBRE_R;

  /* Bosquets autour d'un arbre */
  for(i=0; i<NB_BOSQUET; i++){
    // arbre central
    int randx=rand()%(LARGEUR_MAP-10)+5;
    int randy=rand()%(LONGUEUR_MAP-10)+5;
    tab_arbre[indice].x=randx;
    tab_arbre[indice].y=randy;
    tab_arbre[indice].z=1;
    scale_arbre[indice]=(float)(rand()%20)/(float)10;
    indice++;
    for(j=0;j<((NB_ARBRES-ARBRE_R)/NB_BOSQUET)-1;j++){
      // arbres du bosquet
      int signe=rand()%2-1;
      if (signe==0) signe=1;
      tab_arbre[indice].x=clamp_min_max(randx+signe*rand()%30,10,LARGEUR_MAP-10);
      signe=rand()%2-1;
      if (signe==0) signe=1;
      tab_arbre[indice].y=clamp_min_max(randy+signe*rand()%30,10,LONGUEUR_MAP-10);
      tab_arbre[indice].z=1;
      scale_arbre[indice]=(float)(rand()%20)/(float)10;
      indice++;
    }
  }
}


void init_lait(){
  /* lait <=> munitions */
  /* On les initialise un minimum de maniere centree sur la map */
  int centreX = LARGEUR_MAP / 4;
  int centreY = LONGUEUR_MAP / 4;
  int i;
  for(i=0; i<NB_MUNITIONS; i++){
    tab_lait[i][0]=rand()%(LARGEUR_MAP-centreX)+centreX/2;
    tab_lait[i][1]=rand()%(LONGUEUR_MAP-centreY)+centreY/2;
  }
}

void dessin_lait(){
  int i;
  if (rouge!=0) return;
  for(i=0; i<NB_MUNITIONS; i++){
    if (tab_lait[i][0]==-1){
      /* Si une case vide: nouvelle munition ailleurs */
      int centreX = LARGEUR_MAP / 4;
      int centreY = LONGUEUR_MAP / 4;
      tab_lait[i][0]=rand()%(LARGEUR_MAP-centreX)+centreX/2;
      tab_lait[i][1]=rand()%(LONGUEUR_MAP-centreY)+centreY/2;
    }
    /* On affiche la munition */
    glPushMatrix();
    glTranslatef(tab_lait[i][0],tab_lait[i][1],tab_decors[(int)tab_lait[i][0]][(int)tab_lait[i][1]]);
    glScalef(2,2,2);
    dessin_carton_lait();
    glPopMatrix();
  }
}

void dessin_arbre(){
  /* Fonction inutile, remplacee par le 8arbre */
  int i;
  for(i=0; i<NB_ARBRES; i++){
    glPushMatrix();
    glTranslatef(tab_arbre[i].x,tab_arbre[i].y,tab_decors[(int)tab_arbre[i].x][(int)tab_arbre[i].y]);
    glScalef(3,3,3);
    dessin_arbre2();
    glPopMatrix();
  }
}


void dessin_grille(){
  /* Fonction dessinant le sol (et les nuages) */
  int bS_Larg=LARGEUR_MAP;
  int bS_Long=LONGUEUR_MAP;
  float c;
  int pas = 2;
  int p;
  int i,j;

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
      
      c = mapFloat(tab_decors[(i+nuages)%(LARGEUR_MAP-1)][(j+nuages)%(LONGUEUR_MAP-1)],0,200,0,1);
      glColor3f(c,c,c);

      glVertex3f(i, j, tab_decors[i][j]);
      glVertex3f(i, j+pas, tab_decors[i][j+pas]);
      glVertex3f(i+pas, j, tab_decors[i+pas][j]);

      // changement couleurs (nuages )
      c = mapFloat(tab_decors[(i+nuages)%(LARGEUR_MAP-1)][(j+nuages)%(LONGUEUR_MAP-1)],0,200,0,1);
      glColor3f(c,c,c);
      
      glVertex3f(i, j+pas, tab_decors[i][j+pas]);
      glVertex3f(i+pas, j, tab_decors[i+pas][j]);
      glVertex3f(i+pas, j+pas, tab_decors[i+pas][j+pas] );
      
    }
    glEnd();
  }
}

void projectile(float3 direction, float3 f){
  /* 'Cree' un projectile f avec la direction associee dans le tableau (si possible) */
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
  /* Gere les entrees claviers grace aux variables inputs key_X */
  
  if (dans_vaisseau){ // Input quand on est dans le vaiseau
    if (key_s == 1) {
      
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
      V_DIR_P=rodrigues(-0.8,V_DIR_P,V_UP_INIT);
      V_90_P=produit_vectoriel(V_UP_P,V_DIR_P);      
    }
  
    if (key_gauche == 1) {
      V_DIR_P=rodrigues(0.8,V_DIR_P,V_UP_INIT);
      V_90_P=produit_vectoriel(V_UP_P,V_DIR_P);      
    }
    

  }
  glutPostRedisplay();
}


void affichage(){
  /* Fonction d'affichage principale */
  int i;


  diff_pos_pos_p.x=V_POS.x-V_POS_P.x;
  diff_pos_pos_p.y=V_POS.y-V_POS_P.y;
  diff_pos_pos_p.z=V_POS.z-V_POS_P.z;

  norme_diff_pos_pos_p = norme_vecteur(diff_pos_pos_p);
  if (norme_diff_pos_pos_p < 30 ) {
    a_portee_vaisseau = 1;
  }
  else a_portee_vaisseau = 0;

  
  gestion_input();

  glClearColor(0.1+rouge,0.1,0.2,0.2);
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

  // On verifie si on a pas perdu
  fin();
  
  if (!auto_scroll_toggle)
      V_POS = f3_add_f3(V_POS,mul_float3(V_DIR,S_VIT));

  // 'Copie' pour le lookat hors du vaisseau
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

  /* Actuallisation et collision des projectiles */
  for( i=0; i<50; i++){
    if(tab_proj[i][0].x!=-5000){
      if (distance_point(tab_proj[i][1],V_POS)>1000){
	// Si il est trop loin il disparait
	tab_proj[i][0].x=-5000;
      }
      else{
	
	if(intersection_proj_ennemi(tab_proj[i])==1){
	  tab_proj[i][0].x=-5000;
	  if(rouge==0) score++;
	}
	else{
	  tab_proj[i][1].x+=5*tab_proj[i][0].x;
	  tab_proj[i][1].y+=5*tab_proj[i][0].y;
	  tab_proj[i][1].z+=5*tab_proj[i][0].z;
       
	  tab_proj[i][2].x+=5*tab_proj[i][0].x;
	  tab_proj[i][2].y+=5*tab_proj[i][0].y;
	  tab_proj[i][2].z+=5*tab_proj[i][0].z;

	  affiche_cube_plein(tab_proj[i][1],tab_proj[i][2],1);
	  // Gravite artificielle
	  tab_proj[i][0].z-=0.01;
	}
      }
    }
  }

  /* Affichage et mise a jour des ennemis */
  for( i=0; i<NB_ENNEMIS; i++){
    if(tab_ennemi[i].y!=-1){
      if (tab_ennemi[i].y>=LONGUEUR_MAP+100 || tab_ennemi[i].z<0){
	// Si trop loin, il disparait 
	tab_ennemi[i].y=-1;
      }
      else{
	// On le fait avancer
	tab_ennemi[i].y+=tab_chute_ennemi[i][2];
	glPushMatrix();
	if(tab_chute_ennemi[i][0]==1){ // Si il chute
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
    
  glColor4f(1,0,0,0.5);
  glVertex3f( V_POS.x,V_POS.y,V_POS.z);
  glVertex3f( V_POS.x+V_DIR.x*10,V_POS.y+V_DIR.y*10,V_POS.z+V_DIR.z*10);

  glColor4f(0,1,0,0.5);
  glVertex3f(V_POS.x-V_90.x*5,V_POS.y-V_90.y*5,V_POS.z-V_90.z*5);
  glVertex3f(V_POS.x+V_90.x*5,V_POS.y+V_90.y*5,V_POS.z+V_90.z*5);

  glColor4f(0,0,1,0.5);
  glVertex3f( V_POS.x,V_POS.y,V_POS.z);
  glVertex3f( V_POS.x+V_UP.x*10,V_POS.y+ V_UP.y*10,V_POS.z+ V_UP.z*10);

  glColor4f(0,1,1,1);

  if (!dans_vaisseau){
    float3 h;
    h.x=1;
    h.y=1;
    h.z=1;
    affiche_cube(V_POS_P,f3_add_f3(V_POS_P,h));

    
    if (key_m) {
      dessin_boussole(V_POS_P, V_POS);
    }
  }

  glEnd();


  /* Affichage de l'arbre avec le clipping */
  affiche_karbre_clipping(karbre8,V_POS_P,V_DIR_P,V_UP_P,V_90_P);

  /* Affichage des munitions */
  dessin_lait();

  // Verification de depart
  if(V_POS.x>LARGEUR_MAP/2-20 && V_POS.x<LARGEUR_MAP/2+20
     && V_POS.y>LONGUEUR_MAP/2-20 && V_POS.y<LONGUEUR_MAP/2+20 && start==0)
    start=1;
  start_anim();

  
  intersection_ennemi_ennemi();
  dessin_grille();

  /* Mise a jour du pt_inters pour la collision avec les troncs */
  if(distance_point(V_POS,pt_inters)>100)
    pt_inters=init_float3(-5000,-5000,-500);
  
  /* Dessin de l'interface */ 
  if (start!=0) {
    
    /* Dessin jauge de vie */
    dessin_jauge(5,5,130,50,vie,BLEU,GRIS,ROUGE);
    
    /* Dessin des munitions restantes */
    dessin_munitions(150,5,130,50,mun,MUN_MAX,NOIR,GRIS);
    
    /* Affichage du score */
    dessin_score(620,750,160,50,score,BLANC,NOIR,BLEU);
    
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
    key_m=toggle(key_m);
  }
  if (key == 32){ // touche espace
    if (mun > 0 && dans_vaisseau ){
      projectile(V_DIR,V_POS);
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
    if (dans_vaisseau && COL_DET && S_VIT==0.0){
      modif=toggle(modif);
    }
    if (!dans_vaisseau && a_portee_vaisseau && !modif){
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
  
  // 150 est la hauteur max
  
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

  if ( (tab_lait = malloc (sizeof(float *) * NB_MUNITIONS )) == NULL ) {
    printf("Plus de mémoire :( \n");
    exit (-1);
  }
  else
    for (i=0; i< NB_MUNITIONS;i++) {
      if (( tab_lait[i]= malloc( sizeof(float) * 2)) == NULL ) {
	printf("Plus de mémoire :( \n");
	exit (-1);	
      }
    }

  if ( (tab_arbre = malloc (sizeof(float3) * NB_ARBRES )) == NULL ) {
    printf("Plus de mémoire :( \n");
    exit (-1);
  }
  if ( (scale_arbre = malloc (sizeof(float) * NB_ARBRES )) == NULL ) {
    printf("Plus de mémoire :( \n");
    exit (-1);
  }
  if ( (tab_ennemi = malloc (sizeof(float3) * NB_ENNEMIS )) == NULL ) {
    printf("Plus de mémoire :( \n");
    exit (-1);
  }


  if ( (tab_chute_ennemi = malloc (sizeof(int *) * NB_ENNEMIS )) == NULL ) {
    printf("Plus de mémoire :( \n");
    exit (-1);
  }
  else
    for (i=0; i<NB_ENNEMIS;i++) {
      if (( tab_chute_ennemi[i]= malloc( sizeof(int) * 3)) == NULL ) {
	printf("Plus de mémoire :( \n");
	exit (-1);	
      }
    }

  cote_projectile=-5;
  taille_projectile=1;
  vie=100;
  mun = 50;
  score=0;
  r_sphere_joueur=5;
  COL_DET=0;
  ARR=0;
  float3 init= init_float3(-5000,-5000,-5000);
  for( i=0;i<50;i++)
    tab_proj[i][0]=init;

  /* Initialisation des key */
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

  /* Initialisation des vecteurs */
  V_POS = init_float3(0,0,150);
  V_DIR = init_float3(1,0,0);
  V_90 = init_float3(0,1,0);
  V_UP = init_float3(0,0,1);

  //  V_90_INIT = V_90;
  V_UP_INIT = V_UP;
  
  /* Initialisation des décors */
  
  nuages=0;
    
  for ( i = 0 ; i < LARGEUR_MAP ; i++){
    for ( j = 0 ; j < LONGUEUR_MAP ; j++){
      rand_seed=rand()%30+1;
      tab_decors[i][j]=mapFloat((rand()/rand_seed)%5,0,5,-10,20);
    }
  }

  for ( i =0 ; i<NB_MONTAGNES ; i++)
    mountain(rand()%LARGEUR_MAP,rand()%LONGUEUR_MAP,rand()%150+30);
  
  for (  i = 0 ; i < 28; i++)   aplanir();

  init_arbres();
  init_lait();

  /* Construction du 8arbre */
  karbre8=cons_arbre(tab_arbre,scale_arbre,NB_ARBRES,tab_decors);
  
  for (i=0; i<NB_ENNEMIS; i++){
    tab_ennemi[i]=init_float3(-5000,-1,-5000);
    tab_chute_ennemi[i][0]=0;
    tab_chute_ennemi[i][1]=0;
  }
  pt_inters=init_float3(-5000,-5000,-5000);

  
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE|GLUT_DEPTH);
  glutInitWindowSize(1920,1080);
  glutCreateWindow("FIGHT MILK");
  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  glEnable(GL_DEPTH_TEST);

  /* Brouillard */

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
  
  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyUp);
  glutDisplayFunc(affichage);
  glutSpecialFunc(specialInput);
  glutSpecialUpFunc(specialInputUp);
  
  //gestionnaire GLUT

  glutMainLoop();
  
  return EXIT_SUCCESS;
}
