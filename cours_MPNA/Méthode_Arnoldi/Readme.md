
#Module Projet MPNA : Méthode d'Arnoldi #
*____


## Pogramme  écrit en langage c permet de trouver : 
  
    * le vecteur propre associer à la plus grande valeur propre d'une matrice A donnée en utilisée la méthode de Puissance itérée.
    * le vecteur propre associer à la deuxième  plus grande valeur propre d'une matrice A donnée en en utilisée la méthode de Déflation.



## Projet contient deux fichier  :

    * Arnoldi.c : contient toutes les fonctions et la fonction proncipale "main" qui permet des génerer la matrice Hm et Vm 
    * matricetest.data : Conitent la mtrice pour le test.


## Inputs: 
    * generer matrice aléatoire:
	      Input_1 : Taille de la matrice; 

    *  Lecture fichier:
         Input_1 : Taille de la matrice;
	       Input_2 : Fichier de la matrice.



## Compialtion:

    Afin de compiler les différents fichiers, un makefile a était mise en place pour chaque dossier, donc pour réaliser la compilation il suffit de taper la commande :
```bash
> gcc -fopenmp Arnoldi.c -o  Arnoldi -lm 
````

## Execution:

    Afin d'exécute le programme , il suffit de taper la commande: 


```bash
 >  MP_NUM_THREADS= <nb_thread> ./Arnoldi <Input_1> <Input_2> 
 ```
  

## Résultats
----------
Mtrice Hm , Vm et le  temps d'éxecution ,voir fichiers : 
    
    * Result_Arnoldi_mthod.data
    

#Auteurs:#

 *Aicha LAMMAMRA
*Date : 09/01/2021





