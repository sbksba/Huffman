DOCUMENTATION DE LA BIBLIOTHEQUE HUFFMAN
========================================


STUCTURES DE DONNEES
--------------------

__Structure de l'arbre de Huffman__	


    struct arbre_huffman {
        unsigned char symbole;   
        unsigned int occurence;   
        unsigned int code_binaire;              
        unsigned int nb_bits;           
        struct arbre_huffman *gauche, *droite;           
    };

__Structure du dictionnaire__	

    struct dico{
        unsigned int code_binaire;
        unsigned int nb_bits;
    };


DEFINITIONS
-----------

> SOURCE_NON_TROUVEE -1    
> CIBLE_NON_GENEREE -2    
> MAX_SYMBOLES 256    
> MAX_OCCURENCES 255    
> MAX_NOM 255    
> GAUCHE 0    
> DROITE 1    

VARIABLES GLOBALES
------------------

> unsigned int NbOctFichierAComp=0;    
> unsigned int NbOctFichierADecomp=0;    
> unsigned int NbOctEntete=0;    
> unsigned int NbOctDonnees=0;    
> unsigned int NbSymboles=0;    
> unsigned int MaxOcurrences=0;    
> unsigned int MaxNbBits=0;    
> unsigned int TabNbBits=0;    
> unsigned int TabOcurrences[MAX_SYMBOLES];    
> struct arbre_huffman *TabArbres[MAX_SYMBOLES], *Racine=NULL;    
> struct dico Dictionnaire[MAX_SYMBOLES];    
 
HUFFMAN.H
-----------

> _Initialisation d'une liste chainée._  
    `Chemin* nouveauChemin(int x, int y);`

__INIT__      

> _Initialisation du tableau des occurrences._  
    `void huffman_initialise_TabOccurences();`

> _Initialisation du dictionnaire._  
    `void huffman_initialise_Dictionnaire();`

> _Initialisation du tableau des arbres._  
    `void huffman_initialise_TA(struct arbre_huffman *TA[]);`

> _Contruction de l'arbre binaire._  
    `void huffman_construire_arbre_binaire();`


__TOOL__     

> _Tri l'arbre de Huffman par occurences._  
    `void tri_TA_occurrences(struct arbre_huffman *TA[]);`

> _Affiche de la syntaxe._  
    `void affiche_syntaxe(char commande[]);`

__COMPRESSION__      

> _Création du code par un parcours préfixé de l'arbre._  
    `void huffman_creer_code(struct arbre_huffman *arbre, unsigned int code, unsigned int niveau);

> _Fonction de compression._  
    `int huffman_compression(char NomFichier[]);

> _Ecriture de l'en-tête._  
    `void huffman_ecrire_entete(FILE *fcible);

> _Ecriture du codage._  
    `int huffman_ecrire_codage(FILE *fsource, FILE *fcible);

> _Calcul des occurences des symboles dans le fichier source._  
    `int huffman_calcule_occurences(char nomfichier[]); 

__DECOMPRESSION__     

> _Fonction de décompression._  
    `int huffman_decompression(char NomFichierSource[]);`

> _Lecture de l'en-tête._  
    `void huffman_lire_entete(FILE *fentree);`

> _Lecture du codage._  
    `int huffman_lire_codage(FILE *fsource, FILE *fcible);`

> _Calcul de la taille du fichier._  
    `unsigned int huffman_calcule_taille_fichier(FILE *fentree);`
