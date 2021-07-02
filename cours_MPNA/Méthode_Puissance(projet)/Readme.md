
# Projet MPNA : la méthode de Puissance itérée #
*____


## Pogramme  écrit en langage c permet de trouver : 
  
    * le vecteur propre associer à la plus grande valeur propre d'une matrice A donnée en utilisée la méthode de Puissance itérée.
    * le vecteur propre associer à la deuxième  plus grande valeur propre d'une matrice A donnée en en utilisée la méthode de Déflation.



## Projet contient trois dossiers :
  * Power_Method_Seq : contien le code séquentiel.

        fonctions.c :contient toutes les fonctions séquentielle.
		fonction.h
		main.c: permet de générer le vecteur propre asocié a la plus grande valeur propre.
		        permet de générer le vecteur propre asocié a la deuxième  plus grande valeur propre.

  * Power_Method_Parll : contien le code Parallèle (OpenMP).

        fonctions.c :contient toutes les fonctions parallèle.
		fonction.h
		main.c: permet de générer le vecteur propre asocié a la plus grande valeur propre.
		        permet de générer le vecteur propre asocié a la deuxième  plus grande valeur propre.
  
  * testMatrices :

        contient les  fichier des matrice et sa valuer propre (en dernier ligne ) , où le  nom de fichier préfixé par la taille de la matrice .



## Inputs: 
    * generer matrice aléatoire:
	      Input_1 : Taille de la matrice; 

    *  Lecture fichier:
         Input_1 : Taille de la matrice;
	       Input_2 : Fichier de la matrice.



## Compialtion:

    Afin de compiler les différents fichiers, un makefile a était mise en place pour chaque dossier, donc pour réaliser la compilation il suffit de taper la commande :
```bash
>make
````


## Execution:

    Afin d'exécute le programme main, il suffit de taper la commande(pour les deux version du code) : 


#Power_Method_Seq :

```bash
 > ./project <Input_1> <Input_2> 
````
#Power_Method_Parll :

```bash
 >  MP_NUM_THREADS= <nb_thread> ./project <Input_1> <Input_2> 
 ```


## Résultats
----------
valeur propre ,vecteurs propres  associée, temps d'éxecution  pour les deux méthodes voir fichiers : 
    
    * Result_Power_Methode.dat
    * Result_deflation_method.dat

#Auteurs:#

 *Aicha LAMMAMRA
*Date : 09/01/2021






