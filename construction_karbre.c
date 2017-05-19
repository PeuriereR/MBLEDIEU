#include "construction_karbre.h"

elem_coord get_elem_dans_cube(float3* tab_coord,float** tab_decors,float3 m1,float3 m2){
  int i;
  elem_coord e;
  for(i=0; i<500; i++){
    if (tab_coord[i].x > m1.x && tab_coord[i].x < m2.x
	&& tab_coord[i].y > m1.y && tab_coord[i].y < m2.y
	&& tab_decors[(int)tab_coord[i].x][(int)tab_coord[i].y] > m1.z && tab_decors[(int)tab_coord[i].x][(int)tab_coord[i].y] < m2.z){
      e.coord.x=tab_coord[i].x;
      e.coord.y=tab_coord[i].y;
      e.coord.z=tab_decors[(int)tab_coord[i].x][(int)tab_coord[i].y];
      e.type=tab_coord[i].z;
    }

  }
  return e;


}
elem_coord elem_dans_cube(float3 coord,float** tab_decors,float3 m1,float3 m2){
  //sizeof(float3)==24
  int i;
  elem_coord e;
  e.type=-1;
  if (coord.x > m1.x && coord.x < m2.x
      && coord.y > m1.y && coord.y < m2.y
      && tab_decors[(int)coord.x][(int)coord.y] > m1.z && tab_decors[(int)coord.x][(int)coord.y] < m2.z){
    e.coord.x=coord.x;
    e.coord.y=coord.y;
    e.coord.z=tab_decors[(int)coord.x][(int)coord.y];
    e.type=coord.z;
  }
  return e;
}    


int cube_intersection_tab(float3* tab_coord,float** tab_decors,float3 m1,float3 m2){
  /*******************************************************************

TAILLE DE TABLEAUUUUUUUUUUUUUUUUU EN PARAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAM


  *****************************************************************************/
  
  int i;
  
  int nb_elem=0;
  elem_coord e;
  /* cette boucle est horrible */
  for(i=0; i<500; i++){
    e=elem_dans_cube(tab_coord[i],tab_decors,m1,m2);
    if(e.type!=-1)
      nb_elem++;
  }
  return nb_elem;
}


karbre cons_arbre2(float3* tab_coord, float** tab_decors,float3 m1, float3 m2, int n){
  //0 vide, 1 plein, -1 init -2 noeud interne
  karbre ktmp=kArbreVide();
  int nb_elem_dans_cube=cube_intersection_tab(tab_coord,tab_decors,m1,m2);
  if(nb_elem_dans_cube==0){
    /* aucun element dans le cube */
    ktmp->vide=0;
    return ktmp;
  }
  else if (nb_elem_dans_cube==1){
    /* un element dans le cube > mettre coordonnees + type */
    ktmp->vide=1;
    ktmp->elem=get_elem_dans_cube(tab_coord,tab_decors,m1,m2);
    // NOUVELLE FONCTION elem_dans_cube(tab_coord,tab_decors,m1,m2);
    // METTRE ELEMENT ICI
    return ktmp;
  }
  if (n==0){
    ktmp->vide=0;
    return ktmp;
  }

  /* Cas general: 8 cubes a faire
     8----7
     /|   /|  
     4-|--3 |
     | 5--|-6
     |/   |/ 
     1----2         m2
     /
     m1_i, 2_i   m1
  */

  /* DEVANT */
  //cube 1: m1 & m2_1  des formules bien compliquees pour rien
  float3 m2_1; m2_1.x=(m1.x+(m2.x-m1.x)/2); m2_1.y=(m1.y+(m2.y-m1.y)/2); m2_1.z=(m1.z+(m2.z-m1.z)/2);

  //cube 2: m1_2 m2_2
  float3 m1_2; m1_2.x=(m1.x+(m2.x-m1.x)/2); m1_2.y=m1.y; m1_2.z=m1.z;
  float3 m2_2; m2_2.x=m2.x; m2_2.y=(m1.y+(m2.y-m1.y)/2); m2_2.z=(m1.z+(m2.z-m1.z)/2);

  //cube 3: m1_3 m2_3
  float3 m1_3; m1_3.x=(m1.x+(m2.x-m1.x)/2); m1_3.y=(m1.y+(m2.y-m1.y)/2); m1_3.z=m1.z;
  float3 m2_3; m2_3.x=m2.x; m2_3.y=m2.y; m2_3.z=(m1.z+(m2.z-m1.z)/2);

  //cube 4: m1_4 m2_4
  float3 m1_4; m1_4.x=m1.x; m1_4.y=(m1.y+(m2.y-m1.y)/2); m1_4.z=m1.z;
  float3 m2_4; m2_4.x=(m1.x+(m2.x-m1.x)/2); m2_4.y=m2.y; m2_4.z=(m1.z+(m2.z-m1.z)/2);

  /* DERRIERE */
  //cube 8: m1_8 m2_8
  float3 m1_8; m1_8.x=m1.x; m1_8.y=(m1.y+(m2.y-m1.y)/2); m1_8.z=(m1.z+(m2.z-m1.z)/2);
  float3 m2_8; m2_8.x=(m1.x+(m2.x-m1.x)/2); m2_8.y=m2.y; m2_8.z=m2.z;

  //cube 7: m1_7 m2
  float3 m1_7; m1_7.x=(m1.x+(m2.x-m1.x)/2); m1_7.y=(m1.y+(m2.y-m1.y)/2); m1_7.z=(m1.z+(m2.z-m1.z)/2);

  //cube 6: m1_6 m2_6
  float3 m1_6; m1_6.x=(m1.x+(m2.x-m1.x)/2); m1_6.y=m1.y; m1_6.z=(m1.z+(m2.z-m1.z)/2);
  float3 m2_6; m2_6.x=m2.x; m2_6.y=(m1.y+(m2.y-m1.y)/2); m2_6.z=m2.z;

  //cube 5: m1_5 m2_5
  float3 m1_5; m1_5.x=m1.x; m1_5.y=m1.y; m1_5.z=(m1.z+(m2.z-m1.z)/2);
  float3 m2_5; m2_5.x=(m1.x+(m2.x-m1.x)/2); m2_5.y=(m1.y+(m2.y-m1.y)/2); m2_5.z=m2.z;
  
  return kConsArbre(-2,
		    cons_arbre2(tab_coord,tab_decors,m1,m2_1,n-1),
		    cons_arbre2(tab_coord,tab_decors,m1_2,m2_2,n-1),
		    cons_arbre2(tab_coord,tab_decors,m1_3,m2_3,n-1),
		    cons_arbre2(tab_coord,tab_decors,m1_4,m2_4,n-1),
		    cons_arbre2(tab_coord,tab_decors,m1_8,m2_8,n-1),
		    cons_arbre2(tab_coord,tab_decors,m1_7,m2,n-1),
		    cons_arbre2(tab_coord,tab_decors,m1_6,m2_6,n-1),
		    cons_arbre2(tab_coord,tab_decors,m1_5,m2_5,n-1)
		    );
}


karbre cons_arbre(float3* tab_coord,float** tab_decors){
  // x y z > centre boule - rayon r
  /*
    for(int i=0; i<500; i++){
    printf("tab %f\n",tab_coord[i].x);

    }*/
  int n=N;
  float3 m1,m2;
  m1=init_float3(0,0,0);
  m2=init_float3(pow(2,n),pow(2,n),pow(2,n));
  
  return cons_arbre2(tab_coord,tab_decors, m1, m2, n);
}


void affiche_karbre2(karbre k,float3 m1, float3 m2){
  //0 vide, 1 plein, -1 init -2 noeud interne
  if(k->vide==1){
    glPushMatrix();
    glTranslatef(k->elem.coord.x,k->elem.coord.y,k->elem.coord.z);
    glScalef(0.5,.5,.5);
    dessin_arbre2();
    glPopMatrix();
  }
  if(k->vide==-2){
    /* DEVANT */
    //cube 1: m1 & m2_1  des formules bien compliquees pour rien
    float3 m2_1; m2_1.x=(m1.x+(m2.x-m1.x)/2); m2_1.y=(m1.y+(m2.y-m1.y)/2); m2_1.z=(m1.z+(m2.z-m1.z)/2);

    //cube 2: m1_2 m2_2
    float3 m1_2; m1_2.x=(m1.x+(m2.x-m1.x)/2); m1_2.y=m1.y; m1_2.z=m1.z;
    float3 m2_2; m2_2.x=m2.x; m2_2.y=(m1.y+(m2.y-m1.y)/2); m2_2.z=(m1.z+(m2.z-m1.z)/2);

    //cube 3: m1_3 m2_3
    float3 m1_3; m1_3.x=(m1.x+(m2.x-m1.x)/2); m1_3.y=(m1.y+(m2.y-m1.y)/2); m1_3.z=m1.z;
    float3 m2_3; m2_3.x=m2.x; m2_3.y=m2.y; m2_3.z=(m1.z+(m2.z-m1.z)/2);

    //cube 4: m1_4 m2_4
    float3 m1_4; m1_4.x=m1.x; m1_4.y=(m1.y+(m2.y-m1.y)/2); m1_4.z=m1.z;
    float3 m2_4; m2_4.x=(m1.x+(m2.x-m1.x)/2); m2_4.y=m2.y; m2_4.z=(m1.z+(m2.z-m1.z)/2);

    /* DERRIERE */
    //cube 8: m1_8 m2_8
    float3 m1_8; m1_8.x=m1.x; m1_8.y=(m1.y+(m2.y-m1.y)/2); m1_8.z=(m1.z+(m2.z-m1.z)/2);
    float3 m2_8; m2_8.x=(m1.x+(m2.x-m1.x)/2); m2_8.y=m2.y; m2_8.z=m2.z;

    //cube 7: m1_7 m2
    float3 m1_7; m1_7.x=(m1.x+(m2.x-m1.x)/2); m1_7.y=(m1.y+(m2.y-m1.y)/2); m1_7.z=(m1.z+(m2.z-m1.z)/2);

    //cube 6: m1_6 m2_6
    float3 m1_6; m1_6.x=(m1.x+(m2.x-m1.x)/2); m1_6.y=m1.y; m1_6.z=(m1.z+(m2.z-m1.z)/2);
    float3 m2_6; m2_6.x=m2.x; m2_6.y=(m1.y+(m2.y-m1.y)/2); m2_6.z=m2.z;

    //cube 5: m1_5 m2_5
    float3 m1_5; m1_5.x=m1.x; m1_5.y=m1.y; m1_5.z=(m1.z+(m2.z-m1.z)/2);
    float3 m2_5; m2_5.x=(m1.x+(m2.x-m1.x)/2); m2_5.y=(m1.y+(m2.y-m1.y)/2); m2_5.z=m2.z;
 
    affiche_karbre2(k->fils[0],m1, m2_1);
    affiche_karbre2(k->fils[1],m1_2,m2_2);
    affiche_karbre2(k->fils[2],m1_3,m2_3);
    affiche_karbre2(k->fils[3],m1_4,m2_4);
    affiche_karbre2(k->fils[4],m1_8,m2_8);
    affiche_karbre2(k->fils[5],m1_7,m2);
    affiche_karbre2(k->fils[6],m1_6,m2_6);
    affiche_karbre2(k->fils[7],m1_5,m2_5);
  }
}
void affiche_karbre(karbre k){
  if (k==NULL){
    fprintf(stderr,"K NUL\n");
    return;
  }
    
  float3 m1=init_float3(0,0,0);
  float3 m2=init_float3(pow(2,N),pow(2,N),pow(2,N));
  //affiche_karbre2(k, m1, m2);
}



void affiche_karbre3(karbre k,float3 m1, float3 m2, float3 vdir, float3 p){
  //0 vide, 1 plein, -1 init -2 noeud interne
  if(k->vide==1){
    float3 v_cube=init_float3(k->elem.coord.x-p.x,k->elem.coord.y-p.y,k->elem.coord.z-p.z);
    if (produit_scalaire(vdir,v_cube)<500 && produit_scalaire(vdir,v_cube)>=0){

      glPushMatrix();
      glTranslatef(k->elem.coord.x,k->elem.coord.y,k->elem.coord.z);
      glScalef(1,1,1);
      dessin_arbre2();
      glPopMatrix();
    }
  }
  if(k->vide==-2){
    /* DEVANT */
    //cube 1: m1 & m2_1  des formules bien compliquees pour rien
    float3 m2_1; m2_1.x=(m1.x+(m2.x-m1.x)/2); m2_1.y=(m1.y+(m2.y-m1.y)/2); m2_1.z=(m1.z+(m2.z-m1.z)/2);

    //cube 2: m1_2 m2_2
    float3 m1_2; m1_2.x=(m1.x+(m2.x-m1.x)/2); m1_2.y=m1.y; m1_2.z=m1.z;
    float3 m2_2; m2_2.x=m2.x; m2_2.y=(m1.y+(m2.y-m1.y)/2); m2_2.z=(m1.z+(m2.z-m1.z)/2);

    //cube 3: m1_3 m2_3
    float3 m1_3; m1_3.x=(m1.x+(m2.x-m1.x)/2); m1_3.y=(m1.y+(m2.y-m1.y)/2); m1_3.z=m1.z;
    float3 m2_3; m2_3.x=m2.x; m2_3.y=m2.y; m2_3.z=(m1.z+(m2.z-m1.z)/2);

    //cube 4: m1_4 m2_4
    float3 m1_4; m1_4.x=m1.x; m1_4.y=(m1.y+(m2.y-m1.y)/2); m1_4.z=m1.z;
    float3 m2_4; m2_4.x=(m1.x+(m2.x-m1.x)/2); m2_4.y=m2.y; m2_4.z=(m1.z+(m2.z-m1.z)/2);

    /* DERRIERE */
    //cube 8: m1_8 m2_8
    float3 m1_8; m1_8.x=m1.x; m1_8.y=(m1.y+(m2.y-m1.y)/2); m1_8.z=(m1.z+(m2.z-m1.z)/2);
    float3 m2_8; m2_8.x=(m1.x+(m2.x-m1.x)/2); m2_8.y=m2.y; m2_8.z=m2.z;

    //cube 7: m1_7 m2
    float3 m1_7; m1_7.x=(m1.x+(m2.x-m1.x)/2); m1_7.y=(m1.y+(m2.y-m1.y)/2); m1_7.z=(m1.z+(m2.z-m1.z)/2);

    //cube 6: m1_6 m2_6
    float3 m1_6; m1_6.x=(m1.x+(m2.x-m1.x)/2); m1_6.y=m1.y; m1_6.z=(m1.z+(m2.z-m1.z)/2);
    float3 m2_6; m2_6.x=m2.x; m2_6.y=(m1.y+(m2.y-m1.y)/2); m2_6.z=m2.z;

    //cube 5: m1_5 m2_5
    float3 m1_5; m1_5.x=m1.x; m1_5.y=m1.y; m1_5.z=(m1.z+(m2.z-m1.z)/2);
    float3 m2_5; m2_5.x=(m1.x+(m2.x-m1.x)/2); m2_5.y=(m1.y+(m2.y-m1.y)/2); m2_5.z=m2.z;

      affiche_karbre3(k->fils[0],m1, m2_1,vdir,p);
      affiche_karbre3(k->fils[1],m1_2,m2_2,vdir,p);
      affiche_karbre3(k->fils[2],m1_3,m2_3,vdir,p);
      affiche_karbre3(k->fils[3],m1_4,m2_4,vdir,p);
      affiche_karbre3(k->fils[4],m1_8,m2_8,vdir,p);
      affiche_karbre3(k->fils[5],m1_7,m2,vdir,p);
      affiche_karbre3(k->fils[6],m1_6,m2_6,vdir,p);
      affiche_karbre3(k->fils[7],m1_5,m2_5,vdir,p);
  }
}


void affiche_karbre4(karbre k,float3 centre, float3 vdir,float3 vup){
  if (k==NULL){
    fprintf(stderr,"K NUL\n");
    return;
  }

  float3 p = init_float3(centre.x-30*vdir.x +  vup.x*8 ,
			 centre.y-30*vdir.y +  vup.y*8 ,
			 centre.z-30*vdir.z +  vup.z*8);
   
  float3 m1=init_float3(0,0,0);
  float3 m2=init_float3(pow(2,N),pow(2,N),pow(2,N));

  affiche_karbre3(k, m1, m2,vdir, p);

}
