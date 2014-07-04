#ifndef STRUCT_H
#define STRUCT_H

/***********************************/
/* Structure de l'arbre de Huffman */
/***********************************/
struct arbre_huffman
{
  unsigned char symbole;
  unsigned int occurrence;
  unsigned int code_binaire;
  unsigned int nb_bits;
  struct arbre_huffman *gauche, *droite;
};

/*****************************/
/* Structure du dictionnaire */
/*****************************/
struct dico
{
  unsigned int code_binaire;
  unsigned int nb_bits;
};

#endif
