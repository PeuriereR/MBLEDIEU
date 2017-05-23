#include "draw.h"

void dessin_jauge( float coord_x, float coord_y,float largeur, float hauteur, float valeur, int couleur_FG, int couleur_BG, int couleur_vide){

  valeur=clamp_min_max_f(valeur,0,100);
  float width=.05;
  glDisable(GL_DEPTH_TEST);  
  
  glLoadIdentity ();
  gluOrtho2D (0, 800, 0, 800);
  
  glBegin(GL_QUADS);
  /* Dessin de l'arrière-plan*/
  couleur(couleur_BG);
  glVertex2f(coord_x,coord_y);
  glVertex2f(coord_x+largeur,coord_y);
  glVertex2f(coord_x+largeur,coord_y+hauteur);
  glVertex2f(coord_x,coord_y+hauteur);
  /* Dessin de la partie vide*/  
  couleur(couleur_vide);
  glVertex2f(coord_x+width*hauteur,coord_y+width*hauteur); /*HAUTEUR VOLONTAIRE dans le Y pour avoir un trait de meme épaisseur */
  glVertex2f(coord_x+largeur-width*hauteur,coord_y+width*hauteur); /* Idem*/
  glVertex2f(coord_x+largeur-width*hauteur,coord_y+hauteur-width*hauteur);
  glVertex2f(coord_x+width*hauteur,coord_y+hauteur-width*hauteur);
  /* Dessin de la jauge*/  
  couleur(couleur_FG);
  glVertex2f(coord_x+width*hauteur,coord_y+width*hauteur); /*HAUTEUR VOLONTAIRE dans le Y pour avoir un trait de meme épaisseur */
  glVertex2f(coord_x+clamp_min_max_f(largeur*valeur/100,width*hauteur,largeur-width*hauteur),coord_y+width*hauteur); /* Idem*/
  glVertex2f(coord_x+clamp_min_max_f(largeur*valeur/100,width*hauteur,largeur-width*hauteur),coord_y+hauteur-width*hauteur);
  glVertex2f(coord_x+width*hauteur,coord_y+hauteur-width*hauteur);
 

  glEnd();
  
    glEnable(GL_DEPTH_TEST);  /* bad trip simulator si on commente cette ligne !*/

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


void dessin_carton_lait(){
  // x_, y_, z_ sont les coordonnées du centre de l'objet
  // size est le facteur d'échelle

  float x_,y_,z_,x,y,z;
  x_=y_=z_=0;
  float size=1;
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
  couleur(ROUGE);
  glVertex3f(x     , y     , z     );
  glVertex3f(x+cote, y     , z     );
  glVertex3f(x+cote, y+cote, z     );
  glVertex3f(x     , y+cote, z     );


  // face avant 
  couleur(ROUGE);
  glVertex3f(x     , y     , z     );
  glVertex3f(x+cote, y     , z     );
  glVertex3f(x+cote, y     , z+haut);
  glVertex3f(x     , y     , z+haut);


  // face arrière
  couleur(ROUGE);
  glVertex3f(x     , y+cote, z     );
  glVertex3f(x+cote, y+cote, z     );
  glVertex3f(x+cote, y+cote, z+haut);
  glVertex3f(x     , y+cote, z+haut);

  
  // face droite
  couleur(ROUGE);
  glVertex3f(x+cote, y     , z     );
  glVertex3f(x+cote, y+cote, z     );
  glVertex3f(x+cote, y+cote, z+haut);
  glVertex3f(x+cote, y     , z+haut);

  
  // face gauche
  couleur(ROUGE);  
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


void dessin_arbre2(){
  /* glScalef a passer en param
     + translate */
  glColor3f(.8,.8,.8);
  glBegin(GL_QUADS);

  /* PIED*/
  glVertex3f(1,1,-5);
  glVertex3f(-1,1,-5);
  glVertex3f(-1,1,3);
  glVertex3f(1,1,3);

  glVertex3f(1,1,-5);
  glVertex3f(1,-1,-5);
  glVertex3f(1,-1,3);
  glVertex3f(1,1,3);

  glVertex3f(1,-1,-5);
  glVertex3f(-1,-1,-5);
  glVertex3f(-1,-1,3);
  glVertex3f(1,-1,3);

  glVertex3f(-1,-1,-5);
  glVertex3f(-1,1,-5);
  glVertex3f(-1,1,3);
  glVertex3f(-1,-1,3);



  glVertex3f(1,1,-3);
  glVertex3f(-1,1,-3);
  glVertex3f(-1,-1,-3);
  glVertex3f(1,-1,3);

  
  glColor3f(.2,.25,.2);
  glVertex3f(4,4,3);
  glVertex3f(-4,4,3);
  glVertex3f(-4,-4,3);
  glVertex3f(4,-4,3);
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

  
  glVertex3f(1,1,-5);
  glVertex3f(1,1,3);

  glVertex3f(1,-1,-5);
  glVertex3f(1,-1,3);

  glVertex3f(-1,-1,-5);
  glVertex3f(-1,-1,3);

  glVertex3f(-1,1,-5);
  glVertex3f(-1,1,3);

  glEnd();
  glLineWidth(1);

}

void couleur(int c){
  if (c==GRIS)
    glColor3f(0.9, 0.9, 0.9);
  if (c==BLEU)
    glColor3f(0.2, 0.2, 0.9);
  if (c==ROUGE)
    glColor3f(1,0,0);
}


float clamp_min_max_f(float n, float min, float max){
  if (n<min) return min;
  if (n>=max) return max;
  return n;
}
