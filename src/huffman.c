#include "define.h"
#include "global.h"
#include "huffman.h"

/*======*/
/* INIT */
/*======*/

/* initialisation du tableau des occurrences */
void huffman_initialise_TabOcurrences()
{ 
  int i;
  for (i=0;i<MAX_SYMBOLES;i++) TabOcurrences[i]=0;
}

/* --- initialise le dictionnaire --- */
void huffman_initialise_Dictionnaire()
{ 
  int i;
  for (i=0;i<MAX_SYMBOLES;i++)
    { 
      Dictionnaire[i].code_binaire=0;
      Dictionnaire[i].nb_bits=0;
    }
}

/* --- initialise le tableau des arbres --- */
void huffman_initialise_TA(struct arbre_huffman *TA[])
{ 
  int i,j=0 ;
  struct arbre_huffman *arbre ;

  for (i=0;i<MAX_SYMBOLES;i++) TA[i]=NULL;
  for (i=0;i<MAX_SYMBOLES;i++)
    { 
      if (TabOcurrences[i]!=0)
	{
	  arbre = (struct arbre_huffman *) malloc(sizeof(struct arbre_huffman));
	  arbre->gauche=NULL; arbre->droite=NULL;
	  arbre->symbole=i  ; arbre->code_binaire=0;
	  arbre->nb_bits=0  ; arbre->occurrence=TabOcurrences[i];
	  TA[j++]=arbre;
	}
    }
}

/* --- construction de l'arbre binaire  --- */
void huffman_construire_arbre_binaire()
{
  int i;
  struct arbre_huffman *arbre_gauche, *arbre_droite, *nouvel_arbre;
  struct arbre_huffman *TA[MAX_SYMBOLES];

  /* on travaille sur une copie de TabArbres */
  for(i=0;i<MAX_SYMBOLES;i++) TA[i]=TabArbres[i];
  while (TA[1] != NULL)
  {
    /* on fusionne les arbres 0 et 1 */
    /* le résultat est rangé en 0    */
    arbre_gauche=TA[0];
    arbre_droite=TA[1];
    nouvel_arbre = (struct arbre_huffman *) malloc(sizeof(struct arbre_huffman));
    nouvel_arbre->occurrence = arbre_gauche->occurrence+arbre_droite->occurrence;
    nouvel_arbre->gauche=arbre_gauche;
    nouvel_arbre->droite=arbre_droite;
    nouvel_arbre->symbole=0;
    TA[0]=nouvel_arbre;
    /* on décale la case 1 à la fin et on l'affecte à NULL */
    i=1;
    while ((i<MAX_SYMBOLES-1) && (TA[i] != NULL)) {TA[i]=TA[i+1]; i++ ;}
    TA[i]=NULL;
    /* on trie le tableau */
    tri_TA_occurrences(TA);
  }
  Racine=TA[0];
}

/*======*/
/* TOOL */
/*======*/

/* --- procédure de tri par occurrences--- */
void tri_TA_occurrences(struct arbre_huffman *TA[])
{
  unsigned int i=0, j, val;
  struct arbre_huffman *arbre;
  
  /* boucle de traitement */
  while ((i<MAX_SYMBOLES) && (TA[i] != NULL))
  {
    arbre=TA[i];
    val=arbre->occurrence;
    /* boucle de déplacement des éléments de gauche plus petits */
    j=i;
    while ((j>0) && (val<(TA[j-1]->occurrence))) {TA[j]=TA[j-1]; j--;}
    /* insertion de la valeur à sa place */
    TA[j]=arbre;
    i++;
  }
}

void affiche_syntaxe(char commande[])
{
  printf("Compression   : %s -c fichier\n",commande);
  printf("Décompression : %s -d fichier\n",commande);
}

/*=============*/
/* COMPRESSION */
/*=============*/

/* ---- création du code par un parcours préfixé de l'arbre --- */
void huffman_creer_code(struct arbre_huffman *arbre,unsigned int code, unsigned int niveau)
{
  /* détection d'une feuille */
  if ((arbre->gauche == NULL)&&(arbre->droite == NULL))
    {
      arbre->nb_bits=niveau;
      arbre->code_binaire=code;
      /* on mémorise le code dans le Dictionnaire */
      Dictionnaire[arbre->symbole].code_binaire=code;
      Dictionnaire[arbre->symbole].nb_bits=niveau;
      NbSymboles++;
      if (niveau > MaxNbBits)
	MaxNbBits=niveau;
    }
  else
    {
      /* nouvel appel avec ajout d'un 0 à droite du code */
      huffman_creer_code(arbre->gauche,(code<<1),niveau+1);
      /* nouvel appel avec ajout d'un 1 à droite du code */
      huffman_creer_code(arbre->droite,(code<<1)+1,niveau+1);
    }
}

/* --- calcul des occurrences des symboles dans le fichier source --- */
int huffman_calcule_occurrences(char NomFichier[])
{
  int i, code_retour=0, resultat_lecture;
  unsigned char octet;
  FILE *fsource;

  huffman_initialise_TabOcurrences();
  fsource=fopen(NomFichier,"r");
  if (fsource == NULL) code_retour=SOURCE_NON_TROUVEE ;
  else
    {
      while (!feof(fsource))
	{
	  resultat_lecture=fscanf(fsource,"%c",&octet);
	  if (resultat_lecture != EOF)
	    {
	      TabOcurrences[octet]++;
	      NbOctFichierAComp++;
	    }
	}
      fclose(fsource);
      /* mémorise le nombre maximum d'occurrences */
      for (i=0;i<MAX_SYMBOLES;i++)
	if (TabOcurrences[i]>MaxOcurrences)
          MaxOcurrences=TabOcurrences[i];
    }
  return code_retour;
}

int huffman_compression(char NomFichierSource[])
{
  int code_retour=0;
  char NomFichierCible[MAX_NOM];
  FILE *fsource, *fcible;

  code_retour=huffman_calcule_occurrences(NomFichierSource);
  if(code_retour != SOURCE_NON_TROUVEE)
    { 
      huffman_initialise_Dictionnaire();
      huffman_initialise_TA(TabArbres);
      tri_TA_occurrences(TabArbres);
      huffman_construire_arbre_binaire();
      huffman_creer_code(Racine,0,0);
      /* génération du nom du fichier cible */
      strcpy(NomFichierCible,NomFichierSource);
      strcat(NomFichierCible,".huff");
      fsource=fopen(NomFichierSource,"r");
      fcible=fopen(NomFichierCible,"w");
      if (fcible == NULL) code_retour=CIBLE_NON_GENEREE;
      else
	{
	  huffman_ecrire_entete(fcible);
	  huffman_ecrire_codage(fsource,fcible);
	  printf("Taille du fichier à compresser : %9d octets\n",NbOctFichierAComp);
	  printf("Taille du fichier compressé    : %9d octets\n",NbOctFichierADecomp);
	  printf("    Taille de l'en-tête du fichier compressé : %9d octets\n",NbOctEntete);
	  printf("    Taille des données du fichier compressé  : %9d octets\n",NbOctDonnees);
	  printf("Taux de compression : %4.1f %%\n",100*(1-((float)NbOctFichierADecomp/(float)NbOctFichierAComp)));
	  fclose(fcible);
	}
      fclose(fsource);
    }
  return code_retour;
}

/* --- écriture de l'en-tête --- */
void huffman_ecrire_entete(FILE *fcible)
{
  static unsigned int nb_octets_occ=1,nb_octets_code_binaire=1,res=0;
  unsigned int i,j,k,occurrence_a_ecrire, code_binaire_a_ecrire;
  unsigned int taille_fichier_source, taille_unsigned_int;
  unsigned char tab_octets[8];
  /*struct arbre_huffman *arbre;*/

  /* 3 octets=écriture des infos nécessaires à la lecture de l'en-tête */
  /* premier octet = le nombre de symboles sauvegardés : 256 symboles  */
  /*      0 à 255 pour indiquer 1 à 256 symboles                       */
  /*      (on retire -1 pour éviter le passage à 0 pour la valeur 256) */
  fprintf(fcible,"%c",NbSymboles-1);
  NbOctEntete++;
  
  /* deuxième octet = nombre d'octets utilisés (nb_octets_occ)     */
  /*                  pour écrire la valeur occurrence d'un symbole*/
  res=MaxOcurrences;
  while ((res=res/256) !=0) nb_octets_occ++;
  fprintf(fcible,"%c",nb_octets_occ);
  NbOctEntete++;

  /* troisième octet = nombre d'octets utilisés pour écrire */
  /*                   le code binaire d'un symbole         */
  res=MaxNbBits;
  nb_octets_code_binaire=(MaxNbBits/8)+1;
  fprintf(fcible,"%c",nb_octets_code_binaire);
  NbOctEntete++;

  /* 4 octets = écriture de la taille du fichier */
  /*            source en nombre d'octets        */
  taille_fichier_source=NbOctFichierAComp;
  taille_unsigned_int=sizeof(unsigned int);
  for(i=0;i<taille_unsigned_int;i++)
    {
      j=taille_unsigned_int-i-1;
      tab_octets[j]=taille_fichier_source;
      /* décalage d'un octet à droite */
      taille_fichier_source=taille_fichier_source>>8; 
    }
  for(i=0;i<taille_unsigned_int;i++)
    {
      fprintf(fcible,"%c",tab_octets[i]);
      NbOctEntete++;
    }
  for (k=0;k<NbSymboles;k++)
    {
      /* pour chaque élément de TabArbres on écrit :                     */
      /* -un octet pour le symbole                                       */
      /* -nb_octets_occ octets pour la valeur de l'occurrence du symbole */
      /* -nb_octets_code_binaire octets pour le code binaire du symbole  */ 
      
      /* -- écriture du symbole sur un octet -- */
      fprintf(fcible,"%c",TabArbres[k]->symbole);
      NbOctEntete++;

      /* -- écriture du nombre d'occurrences sur nb_octets_occ octets -- */
      occurrence_a_ecrire=TabArbres[k]->occurrence;
      for(i=0;i<nb_octets_occ;i++)
	{
	  j=nb_octets_occ-i-1;
	  tab_octets[j]=occurrence_a_ecrire;
	  /* décalage d'un octet à droite */
	  occurrence_a_ecrire=occurrence_a_ecrire>>8; 
	}
      for(i=0;i<nb_octets_occ;i++)
	{
	  fprintf(fcible,"%c",tab_octets[i]);
	  NbOctEntete++;
	}
      
      /* --écriture du code_binaire sur nb_octets_code_binaire octets-- */
      code_binaire_a_ecrire=TabArbres[k]->code_binaire;
      for(i=0;i<nb_octets_code_binaire;i++)
	{
	  j=nb_octets_code_binaire-i-1;
	  tab_octets[j]=code_binaire_a_ecrire;
	  /* décalage d'un octet à droite */
	  code_binaire_a_ecrire=code_binaire_a_ecrire>>8;
	}
      for(i=0;i<nb_octets_code_binaire;i++)
	{
	  fprintf(fcible,"%c",tab_octets[i]);
	  NbOctEntete++;
	}
      
      /* -- écriture du nombre de bits sur un octet -- */
      fprintf(fcible,"%c",TabArbres[k]->nb_bits);
      NbOctEntete++;
    }
}

/* --- écriture du codage --- */
int huffman_ecrire_codage(FILE *fsource,FILE *fcible)
{
  unsigned int code_binaire_symbole, nb_bits_symbole, nb_bits_unsigned_int;
  unsigned char octet, octet_a_ecrire;
  int resultat_lecture,nb_bits_tampon=0; /*resultat_ecriture*/
  unsigned int tampon=0;
  
  nb_bits_unsigned_int=sizeof(unsigned int)*8;
  while((resultat_lecture=fscanf(fsource,"%c",&octet))!=EOF)
  {
    code_binaire_symbole=Dictionnaire[octet].code_binaire;
    nb_bits_symbole=Dictionnaire[octet].nb_bits;
    
    /* on positionne le code_binaire_symbole afin  */
    /* de l'ajouter à la fin du tampon             */
    /* étape 1 : on déplace les valeurs binaires complètement à gauche */
    code_binaire_symbole=(code_binaire_symbole<<(nb_bits_unsigned_int-nb_bits_symbole));
    /* étape 2 : on déplace les valeurs binaires   */
    /*           à droite de nb_bits_tampon        */
    code_binaire_symbole=(code_binaire_symbole>>nb_bits_tampon);

    /* on range le code binaire à la fin du tampon */
    tampon=tampon|code_binaire_symbole;
    nb_bits_tampon=nb_bits_tampon+nb_bits_symbole;
    
    /* on regarde si le tampon contient plusieurs octets de données */
    while ((nb_bits_tampon/8)>0)
    {
      /* alors on écrit un octet dans le fichier */
      octet_a_ecrire=(tampon>>(nb_bits_unsigned_int-8));
      fprintf(fcible,"%c",octet_a_ecrire);
      NbOctDonnees++;
      /* on retire l'octet du tampon, on met à jour son nombre de bits */
      tampon=tampon<<8;
      nb_bits_tampon=nb_bits_tampon-8;
    }
  }
  /* si le tampon contient encore des bits significatifs, on les écrit */
  if (nb_bits_tampon !=0)
  {
    octet_a_ecrire=(tampon>>(nb_bits_unsigned_int-8));
    fprintf(fcible,"%c",octet_a_ecrire);
    NbOctDonnees++;
    tampon=tampon<<nb_bits_tampon;
    nb_bits_tampon=0;
    NbOctFichierADecomp++;
  }
  NbOctFichierADecomp=NbOctEntete+NbOctDonnees;
  return EXIT_SUCCESS;
}

/*===============*/
/* DECOMPRESSION */
/*===============*/

int huffman_decompression(char NomFichierSource[])
{
  int code_retour=0; /*resultat_lecture;*/
  /*unsigned char octet;*/
  char NomFichierCible[MAX_NOM];
  FILE *fsource, *fcible;

  /* génération du nom du fichier cible */
  strcpy(NomFichierCible,NomFichierSource);
  strcat(NomFichierCible,".decomp");

  fsource=fopen(NomFichierSource,"r");
  if (fsource == NULL) code_retour=SOURCE_NON_TROUVEE;
  fcible=fopen(NomFichierCible,"w");
  if (fcible == NULL) code_retour=CIBLE_NON_GENEREE;
  
  if ((fsource != NULL) && (fcible != NULL))
    {
      huffman_initialise_TabOcurrences();
      huffman_initialise_TA(TabArbres);
      
      /* on calcule la taille du fichier a décompresser */
      NbOctFichierADecomp=huffman_calcule_taille_fichier(fsource);
      
      /* lecture de l'en-tête du fichier et mise à jour de TabArbres */
      huffman_lire_entete(fsource);
      NbOctDonnees = NbOctFichierADecomp-NbOctEntete;
      
      /* reconstruction de l'arbre binaire  */
      huffman_construire_arbre_binaire();
      
      /* décodage des données compressées */
      huffman_lire_codage(fsource,fcible); 
    }
  fclose(fsource);
  fclose(fcible);
  return code_retour;
}

/* --- lecture de l'en-tête --- */
void huffman_lire_entete(FILE *fentree)
{
  unsigned char nb_symboles, nboctetsocc, nboctetscodebinaire;
  unsigned char symbole, nbbits, un_octet;
  unsigned int nb_octets_occ, nb_octets_code_binaire;
  unsigned int i,j, occurrence, code_binaire, nb_octets_entete=0;
  unsigned int nb_bits, taille_fichier_source, taille_unsigned_int;
  struct arbre_huffman *arbre;

  /* --- lecture des 3 premiers octets --- */
  /* premier octet = nb de symboles à lire, valeurs possibles:0 à 255  */
  /*             (on ajoute 1 pour retrouver la valeur entre 1 et 256) */
  fscanf(fentree,"%c",&nb_symboles); 
  nb_octets_entete++;

  /* attention à ne pas augmenter nb_symboles de type unsigned char */
  /* si sa valeur est 255, nb_symboles passe à 0 et non à 256       */
  NbSymboles=nb_symboles+1;

  /* deuxième octet = nombre d'octets utilisés (nb_octets_occ)    */
  /*                  pour lire la valeur occurrence d'un symbole */
  fscanf(fentree,"%c",&nboctetsocc);
  nb_octets_entete++;
  nb_octets_occ=nboctetsocc;

  /* troisième octet = nombre d'octets utilisés pour lire le */
  /*                   code binaire d'un symbole            */
  fscanf(fentree,"%c",&nboctetscodebinaire);
  nb_octets_entete++;
  nb_octets_code_binaire=nboctetscodebinaire;

  /* --- lecture des 4 octets suivants --- */
  taille_fichier_source=0;
  taille_unsigned_int=sizeof(unsigned int);
  for(i=0;i<taille_unsigned_int;i++)
  {
    fscanf(fentree,"%c",&un_octet);
    nb_octets_entete++;
    taille_fichier_source=(taille_fichier_source<<8)+un_octet;
  }
  NbOctFichierAComp = taille_fichier_source;
  
  /* --- lecture des symboles et de leur information ---              */
  /* pour chaque feuille on lit :                                     */
  /* - un octet pour le symbole                                       */
  /* - nb_octets_occ octets pour la valeur de l'occurrence du symbole */
  /* - nb_octets_code_binaire octets pour le code binaire du symbole  */ 
  /* - puis on range les informations dans TabArbres                  */
  /*k=0;*/
  for(j=0;j<NbSymboles;j++)
  {
    /* -- lecture du symbole sur un octet -- */
    fscanf(fentree,"%c",&symbole);
    nb_octets_entete++;
    /* -- lecture du nombre d'occurrences sur nb_octets_occ octets -- */
    occurrence=0;
    for(i=0;i<nb_octets_occ;i++)
    {
      fscanf(fentree,"%c",&un_octet);
      nb_octets_entete++;
      occurrence=(occurrence<<8)+un_octet;
    }
    /* -- lecture du code_binaire sur nb_octets_code_binaire octets -- */
    code_binaire=0;
    for(i=0;i<nb_octets_code_binaire;i++)
    {
      fscanf(fentree,"%c",&un_octet);
      nb_octets_entete++;
      code_binaire=(code_binaire<<8)+un_octet;
    }
    /* -- lecture du nombre de bits sur un octet -- */
    fscanf(fentree,"%c",&nbbits);
    nb_octets_entete++;
    nb_bits=nbbits;
    /* on met à jour le tableau des occurrences */
    TabOcurrences[symbole]=occurrence;
    /* on range les informations dans TabArbres */
    arbre = (struct arbre_huffman *) malloc(sizeof(struct arbre_huffman));
    arbre->gauche = NULL;
    arbre->droite = NULL;
    arbre->symbole = symbole;
    arbre->code_binaire = code_binaire;
    arbre->nb_bits = nb_bits;
    arbre->occurrence = occurrence;
    TabArbres[j] = arbre;
  }
  NbOctEntete=nb_octets_entete;
}

/* --- lecture du codage --- */
int huffman_lire_codage(FILE *fsource,FILE *fcible)
{
  unsigned char octet;
  int retour_lecture=0;
  unsigned int tampon=0, temp=0, nb_bits_unsigned_int;
  unsigned int nb_bits_tampon=0, val_bit, masque=1;
  unsigned int nb_octets_a_lire, nb_octets_a_ecrire;
  struct arbre_huffman *noeud_courant;

  nb_bits_unsigned_int=sizeof(unsigned int)*8;
  masque=masque<<(nb_bits_unsigned_int-1);
  nb_octets_a_lire   = NbOctDonnees;
  nb_octets_a_ecrire = NbOctFichierAComp;

  while (nb_octets_a_ecrire>0)
  {
    /* si on peut ajouter au moins un octet au tampon, on le remplit */
    while ( (nb_bits_tampon<(nb_bits_unsigned_int-16)) && (nb_octets_a_lire>0) )
      {
	octet=0;
	retour_lecture=fscanf(fsource,"%c",&octet) ;
	if ( retour_lecture != EOF)
	  {
	    nb_octets_a_lire--;
	    temp   = octet;
	    temp   = (temp<<(nb_bits_unsigned_int-(nb_bits_tampon+8)));
	    tampon = tampon | temp;
	    nb_bits_tampon+=8;
	  }
      }
    noeud_courant = Racine;
    while ((nb_octets_a_ecrire>0) && (!((noeud_courant->gauche==NULL) && (noeud_courant->droite == NULL))))
      {
	val_bit=tampon&masque;
	val_bit=val_bit>>(nb_bits_unsigned_int-1);
	tampon=tampon<<1;
	nb_bits_tampon--;
	if (val_bit == 0) noeud_courant=noeud_courant->gauche;
	else noeud_courant=noeud_courant->droite;
      }
    /* on a atteint le symbole */
    fprintf(fcible,"%c",noeud_courant->symbole);
    nb_octets_a_ecrire--;
  }
  return EXIT_SUCCESS;
}

/* --- calcul de la taille du fichier --- */
unsigned int huffman_calcule_taille_fichier(FILE *fentree)
{
  int taille=0, retour;
  unsigned char octet;

  while (!feof(fentree))
    {
      retour=fscanf(fentree,"%c",&octet);
      if(retour != EOF) taille++;
    }
  rewind(fentree);
  return taille;
}
