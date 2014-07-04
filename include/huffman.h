#ifndef HUFFMAN_H
#define HUFFMAN_H

/*======*/
/* INIT */
/*======*/
void huffman_initialise_TabOccurences();
void huffman_initialise_Dictionnaire();
void huffman_initialise_TA(struct arbre_huffman *TA[]);
void huffman_construire_arbre_binaire();

/*======*/
/* TOOL */
/*======*/
void tri_TA_occurrences(struct arbre_huffman *TA[]);
void affiche_syntaxe(char commande[]);

/*=============*/
/* COMPRESSION */
/*=============*/
void huffman_creer_code(struct arbre_huffman *arbre, unsigned int code, unsigned int niveau);
int huffman_compression(char NomFichier[]);
void huffman_ecrire_entete(FILE *fcible);
int huffman_ecrire_codage(FILE *fsource, FILE *fcible);
int huffman_calcule_occurences(char nomfichier[]); 

/*===============*/
/* DECOMPRESSION */
/*===============*/
int huffman_decompression(char NomFichierSource[]);
void huffman_lire_entete(FILE *fentree);
int huffman_lire_codage(FILE *fsource, FILE *fcible);
unsigned int huffman_calcule_taille_fichier(FILE *fentree); 

#endif
