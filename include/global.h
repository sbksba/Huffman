#ifndef GLOBAL_H
#define GLOBAL_H

unsigned int NbOctFichierAComp=0;
unsigned int NbOctFichierADecomp=0;
unsigned int NbOctEntete=0;
unsigned int NbOctDonnees=0;
unsigned int NbSymboles=0;
unsigned int MaxOcurrences=0;
unsigned int MaxNbBits=0;
unsigned int TabNbBits=0;
unsigned int TabOcurrences[MAX_SYMBOLES];
struct arbre_huffman *TabArbres[MAX_SYMBOLES], *Racine=NULL;
struct dico Dictionnaire[MAX_SYMBOLES];

#endif
