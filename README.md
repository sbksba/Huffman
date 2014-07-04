Projet : HUFFMAN
================

> include/structs.h    
>     Structures de données du projet   

> include/define.h   
>     Definitions   

> include/global.h    
>     Variables globales    

> src/huffman.c et include/huffman.h      
>     Fonctions implemantant l'algorithme de Huffman        

> src/main.c       
>     Main du projet         

Directives de Makefile
----------------------

> all  
    Compilation de tous les fichiers   

> proper    
    Suppression de tous les fichiers temporaires    

> clean   
    Suppression de tous les binaires, de tous les fichiers objet et de la librairie    
> cleanall    
    Suppression de tous les binaires, de tous les fichiers objet, de la librairie et des dossiers bin, obj et lib    

> runCom   
    Lance le programme en mode compression sur le répertoire test    

> runDec    
    Lance le programme en mode decompression sur le répertoire test   

> bench   
    Lance un test global (runCom et runDec)   

Directives de l'exécutable bin/HUFFMAN
-------------------------------------

> Usage: HUFFMAN [OPTION] nom_fichier    
> Les OPTIONS sont    
> -c : compression.   
> -d : decompression.   

Etat d'avancement
-----------------

> src/huffman.c   - OK   
> src/main.c      - OK

Normes utilisées
----------------

> ANSI   

Licence
-------

## Licence

(Licence MIT)

Copyright © sbksba

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
