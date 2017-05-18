#include "karbre.h"
#include "float3.h"
#define BLEU 1
#define GRIS 2
// Variables detection collision
float r_sphere_joueur;

// Variables décors
int nuages;
float tab_arbre[500][2];
float tab_decors[500][500];
int rand_seed;

int arbre=1;

// Variables d'animation

float angle=0;
float angle_tangage=0;

float r;


//touches
int key_q, key_d, key_s, key_z, key_m, key_p;

/* CUBE */
float3 p1; 
float3 p2;
float3 p1_hyperC,p2_hyperC;
float3 p1_hyperC2,p2_hyperC2;

float3 V_DIR;
float3 V_90;
float3 V_UP;

float3 tab_proj[50][3];



void couleur(int c){
  if (c==GRIS)
    glColor3f(0.9, 0.9, 0.9);
  if (c==BLEU)
    glColor3f(0.2, 0.2, 0.9);
  

}

void affiche_cube_plein(float3 p1, float3 p2, float opacity){
  
  glBegin(GL_QUADS);
  
  // face dessous
  
  glColor4f(1,1,0,opacity);
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p2.x,p1.y,p1.z);
  glVertex3f(p2.x,p1.y,p2.z);
  glVertex3f(p1.x,p1.y,p2.z);

  // face dessus
  
  glColor4f(1,0.5,0,opacity);
  glVertex3f(p1.x,p2.y,p1.z);
  glVertex3f(p2.x,p2.y,p1.z);
  glVertex3f(p2.x,p2.y,p2.z);
  glVertex3f(p1.x,p2.y,p2.z);

  // face devant
  
  glColor4f(1,0,1,opacity);
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p2.x,p1.y,p1.z);
  glVertex3f(p2.x,p2.y,p1.z);
  glVertex3f(p1.x,p2.y,p1.z);

  // face derriere
  
  glColor4f(1,0,0,opacity);
  glVertex3f(p2.x,p1.y,p2.z);
  glVertex3f(p1.x,p1.y,p2.z);
  glVertex3f(p1.x,p2.y,p2.z);
  glVertex3f(p2.x,p2.y,p2.z);

  //face gauche
  
  glColor4f(0,1,0,opacity);
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p1.x,p1.y,p2.z);
  glVertex3f(p1.x,p2.y,p2.z);
  glVertex3f(p1.x,p2.y,p1.z);
  
  //face droite
  
  glColor4f(0,0,1,opacity);
  glVertex3f(p2.x,p1.y,p1.z);
  glVertex3f(p2.x,p1.y,p2.z);
  glVertex3f(p2.x,p2.y,p2.z);
  glVertex3f(p2.x,p2.y,p1.z);
  
  glEnd();
  

}

int clamp_min_max(int n, int min, int max){
  if (n<min) return min;
  if (n>=max) return max;
  return n;
}
int clamp(int n){
  return clamp_min_max(n,0,497);
}

float mapFloat(float nombre, float actuelInf, float actuelSup, float cibleInf, float cibleSup){
  float pourcentage = (nombre/(actuelSup-actuelInf));
  return cibleInf+pourcentage*(cibleSup-cibleInf);
}

void aplanir(){
  int i;
  int j;
    
  for ( i = 0 ; i < 500 ; i++){
    for ( j = 0 ; j < 500 ; j++){
      tab_decors[i][j]=(tab_decors[clamp(i-1)][clamp(j-1)]+
			tab_decors[clamp( i )][clamp(j-1)]+
			tab_decors[clamp(i+1)][clamp(j-1)]+
			tab_decors[clamp(i-1)][clamp( j )]+
			tab_decors[clamp(i+1)][clamp( j )]+
			tab_decors[clamp(i-1)][clamp(j+1)]+
			tab_decors[clamp( i )][clamp(j+1)]+
			tab_decors[clamp(i+1)][clamp(j+1)])/8;

	
    }
    // printf("%d %d %f\n ",i,j,tab[i][j]);

  }

}

void dessin_arbre2(){
  /* glScalef a passer en param
     + translate */
  glColor3f(.8,.8,.8);
  glBegin(GL_QUADS);

  /* PIED*/
  glVertex3f(1,1,-15);
  glVertex3f(-1,1,-15);
  glVertex3f(-1,1,3);
  glVertex3f(1,1,3);

  glVertex3f(1,1,-15);
  glVertex3f(1,-1,-15);
  glVertex3f(1,-1,3);
  glVertex3f(1,1,3);

  glVertex3f(1,-1,-15);
  glVertex3f(-1,-1,-15);
  glVertex3f(-1,-1,3);
  glVertex3f(1,-1,3);

  glVertex3f(-1,-1,-15);
  glVertex3f(-1,1,-15);
  glVertex3f(-1,1,3);
  glVertex3f(-1,-1,3);

  glEnd();
  
  /*DESSUS */
  
  glColor3f(.2,.25,.2);
  glBegin(GL_TRIANGLES);

  glVertex3f(4,4,3);
  glVertex3f(-4,4,3);
  glVertex3f(0,0,15);

  glVertex3f(4,4,3);
  glVertex3f(4,-4,3);
  glVertex3f(0,0,15);

  glVertex3f(4,-4,3);
  glVertex3f(-4,-4,3);
  glVertex3f(0,0,15);

  glVertex3f(-4,-4,3);
  glVertex3f(-4,4,3);
  glVertex3f(0,0,15);

  glEnd();

  glColor3f(.1,.1,.1);
  glLineWidth(1.5);
  glBegin(GL_LINES);
  /* lignes arbre */
  /* vertical */
  glVertex3f(4,4,3);
  glVertex3f(0,0,15);
  
  glVertex3f(-4,4,3);
  glVertex3f(0,0,15);

  
  glVertex3f(4,-4,3);
  glVertex3f(0,0,15);

  
  glVertex3f(-4,-4,3);
  glVertex3f(0,0,15);
  /* horizontal*/
  
  glVertex3f(4,4,3);
  glVertex3f(-4,4,3);
  
  glVertex3f(4,4,3);
  glVertex3f(4,-4,3);

  glVertex3f(4,-4,3);
  glVertex3f(-4,-4,3);

  glVertex3f(-4,-4,3);
  glVertex3f(-4,4,3);
  

  /* ligne cube */

  
  glVertex3f(1,1,-15);
  glVertex3f(1,1,3);

  glVertex3f(1,-1,-15);
  glVertex3f(1,-1,3);

  glVertex3f(-1,-1,-15);
  glVertex3f(-1,-1,3);

  glVertex3f(-1,1,-15);
  glVertex3f(-1,1,3);

  glEnd();
  glLineWidth(1);

}

void dessin_arbre(){
  int i,j;
  if (arbre==1){
    for(i=0; i<200; i++){
      tab_arbre[i][0]=rand()%490+1;
      tab_arbre[i][1]=rand()%490+1;

    }
    int indice=100;
    for(i=0; i<30; i++){// 10*15
      int randx=rand()%490+1;
      int randy=rand()%490+1;
      tab_arbre[indice][0]=randx;
      tab_arbre[indice][1]=randy;
      indice++;
      for(j=0;j<20;j++){
	int signe=rand()%2-1;
	if (signe==0) signe=1;
	tab_arbre[indice][0]=clamp_min_max(randx+signe*rand()%30,10,490);
	signe=rand()%2-1;
	if (signe==0) signe=1;
	tab_arbre[indice][1]=clamp_min_max(randy+signe*rand()%30,10,490);
	indice++;
      }
    }
    arbre++;
  }
  else{

    for(i=0; i<250; i++){
	
      glPushMatrix();
      glTranslatef(tab_arbre[i][0],tab_arbre[i][1],tab_decors[(int)tab_arbre[i][0]][(int)tab_arbre[i][1]]);
      glScalef(0.5,.5,.5);
      dessin_arbre2();
      glPopMatrix();
    }

  }
}

void dessin_carton_lait(float x_, float y_, float z_, float size){
  // x_, y_, z_ sont les coordonnées du centre de l'objet
  // size est le facteur d'échelle

  float x,y,z;
  float cote=1;
  float haut=1.3;
  float haut2=1.5;
  float d_cote;
  cote*=size;
  haut*=size;
  haut2*=size;
  d_cote=cote/2;
  x=x_-d_cote;
  y=y_-d_cote;
  z=z_-d_cote;

 
  glBegin(GL_QUADS);

  // face inférieure
  couleur(GRIS);
  glVertex3f(x     , y     , z     );
  glVertex3f(x+cote, y     , z     );
  glVertex3f(x+cote, y+cote, z     );
  glVertex3f(x     , y+cote, z     );


  // face avant 
  couleur(GRIS);
  glVertex3f(x     , y     , z     );
  glVertex3f(x+cote, y     , z     );
  glVertex3f(x+cote, y     , z+haut);
  glVertex3f(x     , y     , z+haut);


  // face arrière
  couleur(GRIS);
  glVertex3f(x     , y+cote, z     );
  glVertex3f(x+cote, y+cote, z     );
  glVertex3f(x+cote, y+cote, z+haut);
  glVertex3f(x     , y+cote, z+haut);

  
  // face droite
  couleur(GRIS);
  glVertex3f(x+cote, y     , z     );
  glVertex3f(x+cote, y+cote, z     );
  glVertex3f(x+cote, y+cote, z+haut);
  glVertex3f(x+cote, y     , z+haut);

  
  // face gauche
  couleur(GRIS);  
  glVertex3f(x     , y     , z     );
  glVertex3f(x     , y+cote, z     );
  glVertex3f(x     , y+cote, z+haut);
  glVertex3f(x     , y     , z+haut);

  // pan toit 1
  couleur(BLEU);  
  glVertex3f(x     , y       , z+haut);
  glVertex3f(x+cote, y       , z+haut);
  glVertex3f(x+cote, y+d_cote, z+haut2);
  glVertex3f(x     , y+d_cote, z+haut2);

  // pan toit 2  
  couleur(BLEU);  
  glVertex3f(x     , y+cote  , z+haut);
  glVertex3f(x+cote, y+cote  , z+haut);
  glVertex3f(x+cote, y+d_cote, z+haut2);
  glVertex3f(x     , y+d_cote, z+haut2);
  
  glEnd();

}


void dessin_grille(){
  int borneInf = -99;
  int borneSup = 99;

  int bS=497;
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
  if ( p < 85) nuages ++;
  if (nuages>497) nuages=0;

  nuages=0;
  
  for ( j = 0; j < bS; j+=pas ){

    glBegin(GL_TRIANGLE_STRIP);
    for ( i = 0; i < bS; i+=pas ){

      // changement couleurs( nuages )
      c = mapFloat(tab_decors[(i+nuages)%497][(j+nuages)%497],0,5,0,1);
      glColor3f(c,c,c);
      // glColor3f(.5,.5,.5);
      
      glVertex3f(i, j, tab_decors[i][j]);
      glVertex3f(i, j+pas, tab_decors[i][j+pas]);
      glVertex3f(i+pas, j, tab_decors[i+pas][j]);

      // changement couleurs (nuages )
      c = mapFloat(tab_decors[(i+nuages)%497][(j+nuages)%497],0,5,0,1);
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
void affiche_cube(float3 p1, float3 p2){
  

  glLineWidth(1.5);
  glBegin(GL_LINES);
  // face dessous
  
  glColor4f(1,1,0,1);
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p2.x,p1.y,p1.z);

  glVertex3f(p2.x,p1.y,p1.z);
  glVertex3f(p2.x,p1.y,p2.z);
  
  glVertex3f(p2.x,p1.y,p2.z);
  glVertex3f(p1.x,p1.y,p2.z);

  glVertex3f(p1.x,p1.y,p2.z);
  glVertex3f(p1.x,p1.y,p1.z);

  // face dessus
  
  glColor4f(1,0.5,0,1);

  glVertex3f(p1.x,p2.y,p1.z);
  glVertex3f(p2.x,p2.y,p1.z);

  glVertex3f(p2.x,p2.y,p1.z);
  glVertex3f(p2.x,p2.y,p2.z);

  glVertex3f(p2.x,p2.y,p2.z);
  glVertex3f(p1.x,p2.y,p2.z);

  glVertex3f(p1.x,p2.y,p2.z);
  glVertex3f(p1.x,p2.y,p1.z);

  // face devant
  
  glColor4f(1,0,1,1);
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p1.x,p2.y,p1.z);

  
  // face derriere
  
  glColor4f(1,0,0,1);
  glVertex3f(p2.x,p1.y,p2.z);
  glVertex3f(p2.x,p2.y,p2.z);
  
  //face gauche
  
  glColor4f(0,1,0,1);
  glVertex3f(p1.x,p1.y,p2.z);
  glVertex3f(p1.x,p2.y,p2.z);
  
  //face droite
  
  glColor4f(0,0,1,1);
  glVertex3f(p2.x,p1.y,p1.z);
  glVertex3f(p2.x,p2.y,p1.z);
  
  
  glEnd();
  glLineWidth(1);
}

void projectile(float3 direction,float3 m1, float3 m2){
  int i;
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
  /* Solution patarasse pour empecher la destruction des vecteurs */
  if (key_d == 1 && key_p == 1)
    key_d=2;
  
  if (key_q == 1 && key_p == 1)
    key_q=2;
  
  if (key_d == 1 && key_m == 1)
    key_d=2;
  
  if (key_q == 1 && key_m == 1)
    key_q=2;

  
  if (key_s == 1) {

    // MAJ DU CUBE
    
    p1.z-=V_DIR.z;
    p1.x-=V_DIR.x;
    p1.y-=V_DIR.y;
    
    p2.z-=V_DIR.z;
    p2.x-=V_DIR.x;
    p2.y-=V_DIR.y;
  }
  if (key_z == 1){
 
    // MAJ DU CUBE
    
    p1.x+=V_DIR.x;
    p1.z+=V_DIR.z;
    p1.y+=V_DIR.y;
    
    p2.z+=V_DIR.z;
    p2.x+=V_DIR.x;
    p2.y+=V_DIR.y;
  }
    
  if (key_q == 1){
    // ROTATION: Angle de -4
    
    angle=angle+0.2;
    
    if (angle>=360)
      angle=0;

    /* FORMULE DE RODRIGUES */
    /* Rotation autour de l'axe UP du VECTEUR_DIR*/
    V_DIR=rodrigues(.2,V_DIR,V_UP);
    /* On reactualise le V_90 */
    V_90=produit_vectoriel(V_DIR,V_UP);
 
  }
  if (key_d == 1){
    // ROTATION: Angle_tangage
    angle=angle-0.2;
    if (angle<=0)
      angle=360;

    V_DIR=rodrigues(-0.2,V_DIR,V_UP);
    /* On reactualise le V_90 */
    V_90=produit_vectoriel(V_DIR,V_UP);
    
    /*
      if (save_V90.x* V_UP.x+save_V90.y* V_UP.y+ save_V90.z* V_UP.z < cos(M_PI/180)){
      V_90.x=- V_90.x;
      V_90.y=- V_90.y;
      V_90.z=- V_90.z;
      }*/

  }
  if (key_m == 1){
    /*
      p1.z-=0.5;
      p2.z-=0.5;
    
    */
    angle_tangage=angle_tangage-0.2;
    if (angle_tangage<=-360)
      angle_tangage=0;

    float3 save_PV=V_UP;
    
    V_DIR=rodrigues(-0.2,V_DIR,V_90);
    V_UP=produit_vectoriel(V_DIR,V_90);
    /* Pour contrer le renversement du vecteur par double produit vectoriel */
    if (save_PV.x* V_UP.x+save_PV.y* V_UP.y+ save_PV.z* V_UP.z < cos(M_PI/180)){
      V_UP.x=- V_UP.x;
      V_UP.y=- V_UP.y;
      V_UP.z=- V_UP.z;
    }
  
  }
  
  if (key_p == 1){
    /*
      p1.z+=0.5;
      p2.z+=0.5;
    */
    
    angle_tangage=angle_tangage+0.2;
    if (angle_tangage>=360)
      angle_tangage=0;

    float3 save_PV=V_UP;
    
    V_DIR=rodrigues(0.2,V_DIR,V_90);
    V_UP=produit_vectoriel(V_DIR,V_90);/*    
					     /* Pour contrer le renversement du vecteur par double produit vectoriel */
    if (save_PV.x* V_UP.x+save_PV.y* V_UP.y+ save_PV.z* V_UP.z < cos(M_PI/180)){
      V_UP.x=- V_UP.x;
      V_UP.y=- V_UP.y;
      V_UP.z=- V_UP.z;
    }
   
  }
  glutPostRedisplay();
}

void affichage_pt(float3 p1){
  printf("point: x=%f y=%f z=%f\n",p1.x,p1.y,p1.z);
}

void affichage(){
  int i,j;
  /*
    angle_tangage=angle_tangage+0.2;
    if (abs(angle_tangage)>=360)
    angle_tangage=0;


    angle=angle+0.2;
    
    if (abs(angle)>=360)
    angle=0;
  */

  // Detection des collisions basique
  if((p1.z+p2.z)/2<=tab_decors[(int)(p1.x+p2.x)/2][(int)(p1.y+p2.y)/2])
  printf("x:%f y:%f z:%f\n",(p1.x+p2.x)/2,(p1.y+p2.y)/2,(p1.z+p2.z)/2);
  else printf("\n");
  
  gestion_input();

  float3 centre_cube;
  centre_cube.x=(p1.x+p2.x)/2;
  centre_cube.y=(p1.y+p2.y)/2;
  centre_cube.z=(p1.z+p2.z)/2;


  glClearColor(0.1,0.1,0.2,0.2);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  glFrustum(-10,10,-10,10,10,10000);
  //gluPerspective(100,1,0.1,1000);
  gluLookAt(centre_cube.x-30*V_DIR.x +  V_UP.x*8 ,
	    centre_cube.y-30*V_DIR.y +  V_UP.y*8 ,
	    centre_cube.z-30*V_DIR.z +  V_UP.z*8 ,
	    centre_cube.x,
	    centre_cube.y,
	    centre_cube.z,
	    V_UP.x,
	    V_UP.y,
	    V_UP.z
	    );
  /*
    gluLookAt(centre_cube.x-30*V_DIR.x +  V_UP.x*8 ,
    centre_cube.y-30*V_DIR.y +  V_UP.y*8 ,
    centre_cube.z-30*V_DIR.z +  V_UP.z*8 ,
    centre_cube.x+10*V_DIR.x,
    centre_cube.y+10*V_DIR.y,
    centre_cube.z+10*V_DIR.z,
    V_UP.x,
    V_UP.y,
    V_UP.z
    );
  */  
  
  //gluLookAt(-70,-20,70,10,50,10,0,0,1);


  for( i=0; i<50; i++){
    /* Methode patarasse */
    if(tab_proj[i][0].x!=-5000)
      if (distance_point(tab_proj[i][1],p1)>1000)
	tab_proj[i][0].x=-5000;
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


  glEnd();
  
  p1_hyperC2.x=p1.x-((p2.x-p1.x)/4);
  p1_hyperC2.y=p1.y-((p2.y-p1.y)/4);
  p1_hyperC2.z=p1.z-((p2.z-p1.z)/4);
  
  p2_hyperC2.x=p2.x+((p2.x-p1.x)/4);
  p2_hyperC2.y=p2.y+((p2.y-p1.y)/4);
  p2_hyperC2.z=p2.z+((p2.z-p1.z)/4);

  
  p1_hyperC.x=p1.x+((p2.x-p1.x)/4);
  p1_hyperC.y=p1.y+((p2.y-p1.y)/4);
  p1_hyperC.z=p1.z+((p2.z-p1.z)/4);
  
  p2_hyperC.x=p1.x+(3*(p2.x-p1.x)/4);
  p2_hyperC.y=p1.y+(3*(p2.y-p1.y)/4);
  p2_hyperC.z=p1.z+(3*(p2.z-p1.z)/4);

  
  dessin_arbre();

    
  /*
    affichage_pt(p1);
    affichage_pt(p2);
    affichage_pt(p1_hyperC);
    affichage_pt(p2_hyperC);
    fprintf(stderr,"_____________\n");
  */

  
  // ROTATION ET AFFICHAGE DU CUBE
  
  glPushMatrix();
  
  glTranslatef((p1.x+p2.x)/2,(p1.y+p2.y)/2,(p1.z+p2.z)/2);
  glRotatef(angle,V_UP.x,V_UP.y,V_UP.z);
  glRotatef(angle_tangage,V_90.x,V_90.y,V_90.z);
  glTranslatef(-(p1.x+p2.x)/2,-(p1.y+p2.y)/2,-(p1.z+p2.z)/2);
   
  affiche_cube(p1,p2);
  
  glPopMatrix();
  /*
    affiche_cube_plein(p1_hyperC,p2_hyperC,1);

  
    glPushMatrix();
  
    glTranslatef((p1.x+p2.x)/2,(p1.y+p2.y)/2,(p1.z+p2.z)/2);
    glRotatef(angle,V_90.x,V_90.y,V_90.z);
    glRotatef(angle_tangage,V_UP.x,V_UP.y,V_UP.z);
    glTranslatef(-(p1.x+p2.x)/2,-(p1.y+p2.y)/2,-(p1.z+p2.z)/2);
  
    affiche_cube_plein(p1_hyperC2,p2_hyperC2,0.1);
    affiche_cube(p1_hyperC2,p2_hyperC2);
  
    glPopMatrix();


  
  
  
    glPushMatrix();
  
    glTranslatef((p1.x+p2.x)/2,(p1.y+p2.y)/2,(p1.z+p2.z)/2);
    glRotatef(-angle,V_UP.x,V_UP.y,V_UP.z);
    glRotatef(-angle_tangage,V_90.x,V_90.y,V_90.z);
    glTranslatef(-(p1.x+p2.x)/2,-(p1.y+p2.y)/2,-(p1.z+p2.z)/2);
  
    affiche_cube_plein(p1_hyperC,p2_hyperC,1);
  
    glPopMatrix();
  */

  
  
  /* Affichage de la grille */

  /* Ancien sol
     glBegin(GL_QUADS);
  
     for( i=-100; i<100; i=i+10){
     for( j=-90; j<=100; j=j+10){
     glColor4f((float)(i+j)/100,(float)i/100,(float)j/100,1);
     glVertex3f(i+0.5,-j+0.5,0);
     glVertex3f(i+9.5,-j+0.5,0);
     glVertex3f(i+9.5,-j+9.5,0);
     glVertex3f(i+0.5,-j+9.5,0);

     }
     }
  
     glEnd();
  */
  
  /* AXES */
  /*
    glBegin(GL_LINES);
    glColor4f(0,1,1,1);
  
    glVertex3f(0,0,0);
    glVertex3f(0,100,0);
  
    glVertex3f(0,0,0);
    glVertex3f(100,0,0);
  
    glVertex3f(0,0,0);
    glVertex3f(0,0,100);
  
    glEnd();
  */

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
  if (key == 'k'){
    projectile(V_DIR,p1_hyperC,p2_hyperC);
    fprintf(stderr,"VEC: %f %f %f \n",V_DIR.x,V_DIR.y,V_DIR.z);
  }

}

void mountain(int i_e, int j_e, int largeur){
  int i,j;
  for ( i = largeur ; i > 0 ;i-- )
    for ( j = largeur ; j > 0 ;j-- ){
      if(tab_decors[clamp_min_max(i_e+i,0,500)][clamp_min_max(j_e+j,0,500)]+largeur<50)
	tab_decors[clamp_min_max(i_e+i,0,500)][clamp_min_max(j_e+j,0,500)] +=largeur-((i+j)/2);
      if(tab_decors[clamp_min_max(i_e+i,0,500)][clamp_min_max(j_e-j,0,500)]+largeur<50)
	tab_decors[clamp_min_max(i_e+i,0,500)][clamp_min_max(j_e-j,0,500)] +=largeur-((i+j)/2);
      if( tab_decors[clamp_min_max(i_e-i,0,500)][clamp_min_max(j_e-j,0,500)]+largeur<50)
	tab_decors[clamp_min_max(i_e-i,0,500)][clamp_min_max(j_e-j,0,500)] +=largeur-((i+j)/2);
      if( tab_decors[clamp_min_max(i_e-i,0,500)][clamp_min_max(j_e-j,0,500)]+largeur<50)
	tab_decors[clamp_min_max(i_e-i,0,500)][clamp_min_max(j_e+j,0,500)] +=largeur-((i+j)/2);
    }

}
  









  
int main(int argc, char**argv){
  //INITIALISATION

  r_sphere_joueur=5;
  int i,j,y;
  float3 init; init.x=-5000;
  for( i=0;i<50;i++)
    tab_proj[i][0]=init;
  key_q=0;
  key_d=0;
  key_s=0;
  key_z=0;
  key_m=0;
  key_p=0;
  
  p1.x=0;p1.y=0;p1.z=0;
  p2.x=10;p2.y=10;p2.z=10;

  V_DIR.x=1;
  V_DIR.y=0;
  V_DIR.z=0;
  
  V_90.x = 0;
  V_90.y = 1;
  V_90.z = 0;
  
  V_UP.x=0;
  V_UP.y=0;
  V_UP.z=1;

  // Initialisation décors

  nuages=0;

  time_t t;
  srand((unsigned) time(&t));

  for ( i = 0 ; i < 500 ; i++){
    for ( j = 0 ; j < 500 ; j++){
      rand_seed=rand()%30+1;

      tab_decors[i][j]=mapFloat((rand()/rand_seed)%5,0,5,-10,20);
    }
  }

  for ( i = 0 ; i < 500 ;i++ )
    tab_decors[i][150] = -50;
  
  
  for ( i =0 ; i<30 ; i++){
    mountain(rand()%500,rand()%500,30);

  }
  //mountain(5,5,20);
  //mountain(45,45,20);
  // crevasse

  
  for (  i = 0 ; i < 16; i++)   aplanir();

  
  fprintf(stderr,"VECT: %f %f %f \n",V_UP.x,V_UP.y,V_UP.z);
  fprintf(stderr,"VEC90: %f %f %f \n",V_90.x,V_90.y,V_90.z);
  fprintf(stderr,"VEC: %f %f %f \n",V_DIR.x,V_DIR.y,V_DIR.z);

  glutInit(&argc,argv);
  //Init affichage
  glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE|GLUT_DEPTH);

  //taille & pos
  glutInitWindowSize(800, 800); //ou fullscreen
  glutInitWindowPosition(50, 50);

  glutCreateWindow("TP");
  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  glEnable(GL_DEPTH_TEST);
  //glutMouseFunc glutKeyboardFunc
  
  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyUp);
  glutDisplayFunc(affichage);
  //gestionnaire GLUT
  glutMainLoop();


  return EXIT_SUCCESS;
}
