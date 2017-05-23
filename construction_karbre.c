#include "construction_karbre.h"
#include "draw.h"

int dans_cube(float3 p,float3 m1,float3 m2){
  if(p.x >= m1.x && p.x <= m2.x
     && p.y >= m1.y && p.y <= m2.y
     && p.z >= m1.z && p.z <= m2.z)
    return 1;
  return 0;
}


int intersection_arbre_vaisseau(float3 pt_vaisseau,elem_coord arbre, float3 v_dir, float3 v_up, float3 v_90){
  float3 m1_quadri_intersec_arbre;
  float3 m2_quadri_intersec_arbre;
  
  if (arbre.type==1){
    m1_quadri_intersec_arbre=init_float3(
					 arbre.coord.x-2-3*arbre.scale,
					 arbre.coord.y-2-3*arbre.scale,
					 arbre.coord.z
					 );

    m2_quadri_intersec_arbre=init_float3(
					 arbre.coord.x+2+3*arbre.scale,
					 arbre.coord.y+2+3*arbre.scale,
					 arbre.coord.z+12+12*arbre.scale
					 );
  }
  if (arbre.type==2){
    m1_quadri_intersec_arbre=init_float3(
					 arbre.coord.x-1-1*arbre.scale,
					 arbre.coord.y-1-1*arbre.scale,
					 arbre.coord.z-5
					 );
    m2_quadri_intersec_arbre=init_float3(
					 arbre.coord.x+1+1*arbre.scale,
					 arbre.coord.y+1+1*arbre.scale,
					 arbre.coord.z+3+3*arbre.scale
					 );
  }
  
  
  if (dans_cube(pt_vaisseau,m1_quadri_intersec_arbre,m2_quadri_intersec_arbre)
      || dans_cube(init_float3(pt_vaisseau.x+10*v_dir.x,
			       pt_vaisseau.y+10*v_dir.y,
			       pt_vaisseau.z+10*v_dir.z),m1_quadri_intersec_arbre,m2_quadri_intersec_arbre)
      || dans_cube(init_float3(pt_vaisseau.x+10*v_90.x,
			       pt_vaisseau.y+10*v_90.y,
			       pt_vaisseau.z+10*v_90.z),m1_quadri_intersec_arbre,m2_quadri_intersec_arbre)
      
      || dans_cube(init_float3(pt_vaisseau.x-10*v_90.x,
			       pt_vaisseau.y-10*v_90.y,
			       pt_vaisseau.z-10*v_90.z),m1_quadri_intersec_arbre,m2_quadri_intersec_arbre)
      
      || dans_cube(init_float3(pt_vaisseau.x+10*v_up.x,
			       pt_vaisseau.y+10*v_up.y,
			       pt_vaisseau.z+10*v_up.z),m1_quadri_intersec_arbre,m2_quadri_intersec_arbre)

      )
    return 1; // collision
  return 0;
}


int cube_intersection_tab(float3* tab_coord,int taille_coord,float** tab_decors,float3 m1,float3 m2){
  /*******************************************************************

TAILLE DE TABLEAUUUUUUUUUUUUUUUUU EN PARAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAM


  *****************************************************************************/
  
  int i;
  float3 coord;
  int nb_elem=0;
  int indice;
  int e;
  /* cette boucle est horrible */
  /* TAILLE DU TABLEAU TAB COORD */
  for(i=0; i<taille_coord; i++){
    coord=init_float3(tab_coord[i].x,tab_coord[i].y,tab_decors[(int)tab_coord[i].x][(int)tab_coord[i].y]);
    e=dans_cube(coord,m1,m2);
    if(e==1){
      nb_elem++;
      indice=i;
    }
    if (nb_elem>1)
      return -2;
  }
  if (nb_elem==0)
    return -1; //on veut garder l'indice 0
  //nb_elem=1
  return indice;
}


karbre cons_arbre2(float3* tab_coord,float* tab_scale,int taille_coord,float** tab_decors,float3 m1, float3 m2, int n){
  //0 vide, 1 plein, -1 init -2 noeud interne
  karbre ktmp=kArbreVide();
  int nb_elem_dans_cube=cube_intersection_tab(tab_coord,taille_coord,tab_decors,m1,m2);
  if(nb_elem_dans_cube==-1){
    /* aucun element dans le cube */
    ktmp->vide=0;
    return ktmp;
  }
  else if (nb_elem_dans_cube>0){
    /* un element dans le cube > nb_elem_dans_cube est son indice dans tab_coord */
    ktmp->vide=1;
    ktmp->elem.coord.x=tab_coord[nb_elem_dans_cube].x;
    ktmp->elem.coord.y=tab_coord[nb_elem_dans_cube].y;
    ktmp->elem.coord.z=tab_decors[(int)tab_coord[nb_elem_dans_cube].x][(int)tab_coord[nb_elem_dans_cube].y];
    ktmp->elem.type=tab_coord[nb_elem_dans_cube].z;
    ktmp->elem.scale=tab_scale[nb_elem_dans_cube];
    //=get_elem_dans_cube(tab_coord,tab_decors,m1,m2);
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
		    cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors,m1,m2_1,n-1),
		    cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors,m1_2,m2_2,n-1),
		    cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors,m1_3,m2_3,n-1),
		    cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors,m1_4,m2_4,n-1),
		    cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors,m1_8,m2_8,n-1),
		    cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors,m1_7,m2,n-1),
		    cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors,m1_6,m2_6,n-1),
		    cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors,m1_5,m2_5,n-1)
		    );
}


karbre cons_arbre(float3* tab_coord,float* tab_scale,int taille_coord,float** tab_decors){
  // x y z > centre boule - rayon r
  /*
    for(int i=0; i<500; i++){
    printf("tab %f\n",tab_coord[i].x);

    }*/
  int n=N;
  float3 m1,m2;
  m1=init_float3(0,0,0);
  m2=init_float3(pow(2,n),pow(2,n),pow(2,n));
  
  return cons_arbre2(tab_coord,tab_scale,taille_coord,tab_decors, m1, m2, n);
}


void affiche_karbre_simple2(karbre k,float3 m1, float3 m2){
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
 
    affiche_karbre_simple2(k->fils[0],m1, m2_1);
    affiche_karbre_simple2(k->fils[1],m1_2,m2_2);
    affiche_karbre_simple2(k->fils[2],m1_3,m2_3);
    affiche_karbre_simple2(k->fils[3],m1_4,m2_4);
    affiche_karbre_simple2(k->fils[4],m1_8,m2_8);
    affiche_karbre_simple2(k->fils[5],m1_7,m2);
    affiche_karbre_simple2(k->fils[6],m1_6,m2_6);
    affiche_karbre_simple2(k->fils[7],m1_5,m2_5);
  }
}
void affiche_karbre_simple(karbre k){
  if (k==NULL){
    fprintf(stderr,"K NUL\n");
    return;
  }
    
  //float3 m1=init_float3(0,0,0);
  // float3 m2=init_float3(pow(2,N),pow(2,N),pow(2,N));
  //affiche_karbre_simple2(k, m1, m2);
}



void affiche_karbre_clipping2(karbre k,float3 m1, float3 m2, float3 vdir,float3 vup,float3 v90, float3 point_clipping, float3 pt_eye, float3 pt_vaisseau){
  //affiche_cube(m1,m2);
  //0 vide, 1 plein, -1 init -2 noeud interne
  if(k->vide==1){
    float3 v_cube=init_float3(k->elem.coord.x-point_clipping.x,k->elem.coord.y-point_clipping.y,k->elem.coord.z-point_clipping.z);
    //angle en radians
    float angle =acos(  (float) produit_scalaire(vdir,v_cube) / (float)(norme_vecteur(vdir)*norme_vecteur(v_cube)) );
    angle= angle * (180/M_PI);

    /*****************************************************
             EXPLICATION CLIPPING VIA 8-ARBRE 
            
            \_______________________/  scal<800
             \                     /
              \                   /   
               \  ZONE VISIBLE   /
                \               /
                 \_____ V_____ /  scal>=0
                  \     |     /
                   \    | 34°/
                    \   |   /
                     \  |  /
                      \ | /
                       \|/

            V le point de vue actuel
              (Ceci en 3D -> Cone)

    ****************************************************/
    
    if (/* clipping sur l'angle */
	angle<40
	/* clipping sur la distance */
	&& produit_scalaire(vdir,v_cube)<800
	/* clipping derriere */
	&& produit_scalaire(vdir,v_cube)>=0
	){
      //affiche_cube(m1,m2);
      /* on peut verifier k->elem.type si on n'a pas que des arbres */
      if(intersection_arbre_vaisseau(pt_vaisseau,k->elem,vdir,vup,v90)){
	if( k->elem.type==1)
	  k->elem.type=2;
	if (k->elem.type==2)
	  INTERS_TRONC=1;
      }
      if(k->elem.type==1){
	glPushMatrix();
	glTranslatef(k->elem.coord.x,k->elem.coord.y,k->elem.coord.z);
	glScalef(1+k->elem.scale,1+k->elem.scale,1+k->elem.scale);
	dessin_arbre2();
	glPopMatrix();
      }
      if(k->elem.type==2){
	glPushMatrix();
	glTranslatef(k->elem.coord.x,k->elem.coord.y,k->elem.coord.z);
	glScalef(1+k->elem.scale,1+k->elem.scale,1+k->elem.scale);
	dessin_tronc();
	glPopMatrix();
      }
    }
  }
  if(k->vide==-2){

    /********************
       8----7
      /|   /|            
     4-|--3 |            
     | 5--|-6        
     |/   |/        
     1----2        


    De base 1=m1 7=m2

  On calcule tous les autres m1 m2 des 8 sous_cubes

    ********************/
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


    /* On stoppe la propagation inutile en verifiant que le scalaire m1.point actuel et m2.point actuel) < 0 ET qu'on ne se trouve pas dans ce cube) */
    
    if (produit_scalaire(init_float3(m1.x-pt_eye.x,m1.y-pt_eye.y,m1.z-pt_eye.z),vdir)>=0
	|| produit_scalaire(init_float3(m2_1.x-pt_eye.x,m2_1.y-pt_eye.y,m2_1.z-pt_eye.z),vdir)>=0
	|| dans_cube(pt_eye,m1,m2_1)==1)
      affiche_karbre_clipping2(k->fils[0],m1, m2_1,vdir, vup, v90, point_clipping,pt_eye,pt_vaisseau);

    if (produit_scalaire(init_float3(m1_2.x-pt_eye.x,m1_2.y-pt_eye.y,m1_2.z-pt_eye.z),vdir)>=0
	|| produit_scalaire(init_float3(m2_2.x-pt_eye.x,m2_2.y-pt_eye.y,m2_2.z-pt_eye.z),vdir)>=0
	|| dans_cube(pt_eye,m1_2,m2_2)==1)
      affiche_karbre_clipping2(k->fils[1],m1_2,m2_2,vdir, vup, v90, point_clipping,pt_eye,pt_vaisseau);

    if (produit_scalaire(init_float3(m1_3.x-pt_eye.x,m1_3.y-pt_eye.y,m1_3.z-pt_eye.z),vdir)>=0
	|| produit_scalaire(init_float3(m2_3.x-pt_eye.x,m2_3.y-pt_eye.y,m2_3.z-pt_eye.z),vdir)>=0
	|| dans_cube(pt_eye,m1_3,m2_3)==1)
      affiche_karbre_clipping2(k->fils[2],m1_3,m2_3,vdir, vup, v90, point_clipping,pt_eye,pt_vaisseau);

    if (produit_scalaire(init_float3(m1_4.x-pt_eye.x,m1_4.y-pt_eye.y,m1_4.z-pt_eye.z),vdir)>=0
	|| produit_scalaire(init_float3(m2_4.x-pt_eye.x,m2_4.y-pt_eye.y,m2_4.z-pt_eye.z),vdir)>=0
	|| dans_cube(pt_eye,m1_4,m2_4)==1)
      affiche_karbre_clipping2(k->fils[3],m1_4,m2_4,vdir, vup, v90, point_clipping,pt_eye,pt_vaisseau);

    if (produit_scalaire(init_float3(m1_8.x-pt_eye.x,m1_8.y-pt_eye.y,m1_8.z-pt_eye.z),vdir)>=0
	|| produit_scalaire(init_float3(m2_8.x-pt_eye.x,m2_8.y-pt_eye.y,m2_8.z-pt_eye.z),vdir)>=0
	|| dans_cube(pt_eye,m1_8,m2_8)==1)
      affiche_karbre_clipping2(k->fils[4],m1_8,m2_8,vdir, vup, v90, point_clipping,pt_eye,pt_vaisseau);

    if (produit_scalaire(init_float3(m1_7.x-pt_eye.x,m1_7.y-pt_eye.y,m1_7.z-pt_eye.z),vdir)>=0
	|| produit_scalaire(init_float3(m2.x-pt_eye.x,m2.y-pt_eye.y,m2.z-pt_eye.z),vdir)>=0
	|| dans_cube(pt_eye,m1_7,m2)==1)
      affiche_karbre_clipping2(k->fils[5],m1_7,m2,vdir, vup, v90, point_clipping,pt_eye,pt_vaisseau);

    if (produit_scalaire(init_float3(m1_6.x-pt_eye.x,m1_6.y-pt_eye.y,m1_6.z-pt_eye.z),vdir)>=0
	|| produit_scalaire(init_float3(m2_6.x-pt_eye.x,m2_6.y-pt_eye.y,m2_6.z-pt_eye.z),vdir)>=0
	|| dans_cube(pt_eye,m1_6,m2_6)==1)
      affiche_karbre_clipping2(k->fils[6],m1_6,m2_6,vdir, vup, v90, point_clipping,pt_eye,pt_vaisseau);

    if (produit_scalaire(init_float3(m1_5.x-pt_eye.x,m1_5.y-pt_eye.y,m1_5.z-pt_eye.z),vdir)>=0
	|| produit_scalaire(init_float3(m2_5.x-pt_eye.x,m2_5.y-pt_eye.y,m2_5.z-pt_eye.z),vdir)>=0
	|| dans_cube(pt_eye,m1_5,m2_5)==1)
      affiche_karbre_clipping2(k->fils[7],m1_5,m2_5,vdir, vup, v90, point_clipping,pt_eye,pt_vaisseau);
  }
}


void affiche_karbre_clipping(karbre k,float3 centre, float3 vdir,float3 vup, float3 v90){
  if (k==NULL){
    fprintf(stderr,"K NUL\n");
    return;
  }

  float3 point_clipping = init_float3(centre.x-150*vdir.x +  vup.x*8 ,
			 centre.y-150*vdir.y +  vup.y*8 ,
			 centre.z-150*vdir.z +  vup.z*8);
  float3 p_eye = init_float3(centre.x-30*vdir.x +  vup.x*8 ,
			 centre.y-30*vdir.y +  vup.y*8 ,
			 centre.z-30*vdir.z +  vup.z*8);
   
  float3 m1=init_float3(0,0,0);
  float3 m2=init_float3(pow(2,N),pow(2,N),pow(2,N));

  affiche_karbre_clipping2(k, m1, m2,vdir, vup, v90, point_clipping,p_eye,centre);

}
