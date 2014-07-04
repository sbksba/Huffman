#include "define.h"
#include "huffman.h"

int main(int argc, char *argv[])
{ 
  int retour;
  char NomFichierAComp[MAX_NOM], NomFichierADecomp[MAX_NOM];
  
  if (argc != 3) affiche_syntaxe(argv[0]);
  else
    { 
      if (!strcmp(argv[1],"-c"))
	{ 
	  strcpy(NomFichierAComp,argv[2]);
	  retour=huffman_compression(NomFichierAComp);
	  if (retour == SOURCE_NON_TROUVEE)
	    printf("Fichier %s non trouvé\n",NomFichierAComp);
	  else if (retour == SOURCE_NON_TROUVEE)
	    printf("Impossible de générer le Fichier %s.huff\n",NomFichierADecomp);
	}
      else if (!strcmp(argv[1],"-d"))
	{
	  strcpy(NomFichierADecomp,argv[2]);
	  retour=huffman_decompression(NomFichierADecomp);
	  if (retour == SOURCE_NON_TROUVEE)
	    printf("Fichier %s non trouvé\n",NomFichierADecomp);
	  else if (retour == SOURCE_NON_TROUVEE)
	    printf("Impossible de générer le Fichier %s.comp\n",NomFichierADecomp);
	}
      else affiche_syntaxe(argv[0]);
    }
  
  return EXIT_SUCCESS;
}
