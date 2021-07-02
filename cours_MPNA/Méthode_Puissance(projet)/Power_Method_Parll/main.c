#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<omp.h>

#include"fonctionParll.h"

#define DIM  100


typedef struct powrIter{
    int size;
    double eigenvalue;
    double *eigenvector;
}PowrIter;

typedef PowrIter* powrIter;

/* ___________ Méthode de la Puissance Itérée ___________ */

// powrIter est une structure qui contient le vecteur propre associé à sa valeur propre

powrIter power_method(double **A,int n)
{
  powrIter val=NULL;

  int iter=0;
  int IMAX= 10000;
    double tol=1.e-7;
    double eigenvalue_old=1;
  
  double *x;
  
  // Allocation dynamique des vecteurs 
 
  val=malloc(sizeof(powrIter));
  val->eigenvector=malloc(n*sizeof(double));
  x=malloc(n*sizeof(double));
    
  // Initialisation du vecteur
  val->size=n;
  val->eigenvalue=0;

  // Calcule le vecteur normal pour initialisation
  init_vector(val->eigenvector, n, 0);   
  val->eigenvector[0]=1;

  while(iter<IMAX && fabs(val->eigenvalue - eigenvalue_old)> tol){
          
        eigenvalue_old = val->eigenvalue;

        x = prod_mtrix_vector(A, val->eigenvector, n);

        dev_vecteur_scal(x,norm(x,n), n, val->eigenvector);

        val->eigenvalue = prod_scalaire(x,val->eigenvector,n);

        iter++;

        if (iter==IMAX){
        
            fprintf(stderr,"Convergence error in the iterated power method \n");
            return NULL;
        }
  }
  
  return val;
}

/* ___________ Méthode de  Déflation___________ */

powrIter deflation_method(double **A, double value, double* vector, int n){
    
    int i,j;
    double **B;
    powrIter val2=NULL;

    B=malloc(n*sizeof(double*));
    for(int i=0;i<n;i++) {
        B[i]=malloc(n*sizeof(double));
    }
    #pragma omp parallel for default (none) shared (A, value, vector,n,B) private (i,j)

    for(i=0;i<n;i++){
    for(j=0;j<n;j++){
    
      B[i][j] = A[i][j] - value * vector[i]*vector[j];
    }
  }
  val2= power_method(B,n);
  return val2;
}  


int main(int argc, char**argv){

   // int DIM =atoi(argv[1]);
    FILE* fichier1 = NULL;
    FILE* fichier2 = NULL;
    

    double ** A=malloc(DIM*sizeof(double*));

    for(int i=0;i<DIM;i++) {
        A[i]=malloc(DIM*sizeof(double));
    }
    
    //_________ Data : lecture de fichie ou générer une matrice aléatoiree _______

    /*Read file from command line */
    
    /*char nom_fichier[30];
    strcpy(nom_fichier, argv[1]);
 
    read_data(nom_fichier, A, DIM);
    //affiche_matrix(A,DIM);

    /* random mtrix */
    generate_Matrix(A, DIM);
    //affiche_mat(A,DIM);

    double start = omp_get_wtime(); 
    powrIter val=power_method(A,DIM);
    double stop = omp_get_wtime(); 
  
    double elapsed_time = stop - start;

    double start2 = omp_get_wtime(); 
    powrIter val2=deflation_method(A,val->eigenvalue, val->eigenvector, DIM);
    double stop2 = omp_get_wtime(); 

    double elapsed_time2 = stop2- start2;

    save_results(fichier1,"Result_Power_Mth.dat",val->eigenvalue,  val->eigenvector,DIM,elapsed_time);
    save_results(fichier2,"Result_deflation_Meth.dat",val2->eigenvalue,  val2->eigenvector,DIM,elapsed_time2);

    printf("\n les résultats dans le fichier : Result_Power_Mth.dat  Result_deflation_Meth.dat   \n");

    /*libérer de la mémoire allouée dynamiquement*/ 
	 
    for(int i=0;i<DIM;i++){
		  free(A[i]);
	  }

	  free(A);
    free(val);
    free(val2);
	  free(val->eigenvector);
    free(val2->eigenvector);
	
  return 0;

}

