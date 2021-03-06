#include "karbre.h"

karbre kArbreVide(){
  karbre k=(karbre)malloc(sizeof(karbre_noeud));
  k->vide=-1;
  return k;
}
karbre kFils(int ieme, karbre a){
  if (ieme>=K) return NULL;
  return a->fils[ieme];
}
element kRacine (karbre a){
  return a->vide;
}
int kEstVide(karbre a){
  if (a==NULL)
    return 1;
  return 0;
}
void kAfficher(karbre a){
  kAfficher2(a,0,-1);
}

void kAfficher2(karbre a, int prof, int fils){
  int i;
  for( i=0;i<prof;i++)
    printf("\t");
  printf("%d",a->vide);
  
  printf("\n");
  for( i=0; i<K; i++){
    if(a->fils[i]!=NULL)
      kAfficher2(a->fils[i],prof+1,i);
  }
}

karbre kConsArbre(element e,...){
  int i;
  karbre k=kArbreVide();
  
  va_list params; // pointeur liste param
  va_start(params, e); // init pointeur avec le dernier param

  k->vide=e;
  
  for ( i = 0 ; i < K ; i++) {
    // récupérer le paramètre suivant de la liste
    karbre valeur_suiv=va_arg(params,karbre);
    k->fils[i]=valeur_suiv;
  }
  va_end(params); // fermeture de la liste
	
  return k;
}

karbre copie_karbre(karbre k){
  // explicite
  karbre kopie=kArbreVide();
  if (k==NULL) return NULL;
  if (k->vide==-1)
    return kopie;
  if (k->vide==0){
    kopie->vide=0;
    return kopie;
  }
  if (k->vide==1){
    kopie->vide=1;
    return kopie;
  }
  return kConsArbre(-2,
		    copie_karbre(k->fils[0]),
		    copie_karbre(k->fils[1]),
		    copie_karbre(k->fils[2]),
		    copie_karbre(k->fils[3]),
		    copie_karbre(k->fils[4]),
		    copie_karbre(k->fils[5]),
		    copie_karbre(k->fils[6]),
		    copie_karbre(k->fils[7])
		    );

}
