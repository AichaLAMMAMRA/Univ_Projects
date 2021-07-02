###Cours : Génie logiciels

#heat.c
------------

Ce fichier contient le programme qui permet de realiser une simulation a base d'equation de chaleur avec deux options. in situ et in transit. 
Dans la premiere l ensemble des processus arrete le calcul et envoie les données pour un pas de temps donné en paramatre. les resultats stockées dans diag.h5. Dans la deuxieme un certain nombre de processus fait l envoie en meme temps que les autres continues le calcul. Les resultats toujours stockées dans diag.h5. 

# library :
------------------
- Ce dossier contient les fichiers pour calculer la moyenne et la derivée. 
- C'est deux sont calculer en parallel apres envoyer au proc zero qui fait l'ecriture. 

Il suffit de passer le pas de temps choisis en parametre. 

- mean.c : calcul la moyenne selon x, puis selon y puis la moyenne globale. le resultat est ecrit dans diags.h5.

- derivative.c : calcul la difference entre deux pas de temps données. le resultat est ecrit dans diags.h5.

chaque groupe dans diags.h5 contient des groupes avec les datasets: mean_x, mean_y, mean et derivative.
A la fin, le fichier **(diags.h5)** contient un certain nombre de groupes dépendant du nombre de pas de temps choisis, et chaque groupe 

#Compialtion
------------

Il suffit d'avoir l'outil make pour compiler le projet en utilisant la commande : 

>make

#Execution
------------------------------------

Pour excuter le programme il faut le lancer avec les parametre choisit comme suit : 

>mpirun -np 6 ./heat 100 100 100 -t (ou -s) 50 100

Avec (par ordre) :

-np 6 est le nombre de prox

100 est le nb d'itération


100 100 la taille du dataset

-s pour in situ et -t pour in transit

si -s : 50 100 pas de temps

si -t : 50 nombre de proc et 100 pas de temps.


Les résultats seront dans **(diags.h5)**

# Remarque :

in transit ne fonctionne pas correctement. il s'arrete a 99. 


# Visualisation des résultats :
------------------------------

La commande suivante permet de visualiser: 

>h5dump diag.h5




# Projet fait par :
---------
LAMMAMRA Aicha
Boudaoud Mohand ouali
Ghenai Mohamed 


