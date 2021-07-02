#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>




/*___________ Générer une matrice aléatoire ___________*/

void generate_Matrix(double **matrix, int n) { 
    
    for ( int i = 0; i < n; i ++) {

        for (int j = 0; j < n; j++) {
          matrix[i][j]= rand()%100;
        }
    } 
       
}

/*___________ Inisialiser un vecteur ___________ */

void init_vector(double *vector, int n, double value) { 
    
 
        for(int j=0;j<n;j++)
            vector[j]=value;
}

/*___________ inisialiser une matrice ___________ */

void init_matrix(double **mat, double n, double v)
{ 

  for(int i=0;i<n;i++)
    for(int j=0;j<n;j++)
      mat[i][j]=v;
}

/*___________ Afficher une matrice ___________ */

void affiche_matrix(double **A,int m){ 

    printf ("\n     _________  matrice A _________\n" );
    for(int i=0;i<m-1;i++) {

        for(int j=0;j<m;j++){
                  
            printf("%f  ",A[i][j]);
        }
        printf("\n");
      }
 }

/*___________ Calculer le produit de deux vecteurs ___________ */

double prod_scalaire(double *vector1,double *vector2,int n){
    
    double result=0;


    for(int i=0;i<n;i++){

        result += vector1[i]*vector2[i];
    }
    return result;
}

 /*___________ Calculer le produit vecteur scalaire  ___________ */

  double* prod_vector_scal(double *vector, double scal, int n){
    
    double* result=malloc(n*sizeof(double));
    init_vector(result, n, 0); 

    for(int j=0;j<n;j++)
      
      result[j]=vector[j]*scal;
        
    return result;

  }

/*___________ Calculer la division vecteur scalaire ___________ */

double* dev_vecteur_scal(double *vector,double scal,int n){ 

    return (prod_vector_scal(vector , 1/scal, n));   
}

/*___________ Multiplication vector *Matrix ___________*/

double* prod_mtrix_vector(double **matrix,double *vector,int n){
    
    int i,j;
    double *result;

    result=(double*)malloc(n*sizeof(double));
    init_vector(result, n, 0);

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){

            result[i] += matrix[i][j] * vector[j];
       }
    }
    return result;
        
}

/*___________ Calcule la norme euclidienne d'un vecteur ___________ */

double norm(double *vector,int n){  
    return sqrt(prod_scalaire(vector,vector,n));
} 


/*___________ Lecture de fichie ___________ */


void read_data(char *filename, double ** matricx, int n){
      
      FILE *FL = fopen(filename,"r");
    
      for ( int i = 0; i < n; i ++)
        for (int j = 0; j < n; j++)
            fscanf(FL, "%lf", &matricx[i][j]);

               //printf(" Lecture de A terminee\n");
      fclose(FL);
  }

/*___________ Créer un fichier et écrire des données dans un fichier ___________ */

void write_into_file (FILE* fichier,char * filename,double value, double* vector,int n, double elapsed_time){
    
    fichier = fopen(filename,"w"); 

   
      if (fichier != NULL){

        fprintf(fichier,"\n ______________________  For the size of matrix %d the results are :______________________ \n",n);
        fprintf(fichier,"\n");
        fprintf(fichier,"\n **********  Execution time of methode is : **********   \n");
        fprintf(fichier,"\n Time = %f seconds \n", elapsed_time);
        fprintf(fichier,"\n **********  Dominant eigenvalue **********   \n");
        fprintf(fichier,"\n Eigenvalue = %f \n",value);
        fprintf(fichier,"\n **********  Eigenvector associated **********   \n");
        fprintf(fichier,"\n");
        fprintf(fichier,"\n");


        for(int i=0;i<n;i++) {
            fprintf(fichier, "%lf ", vector[i]);
        }
        
        fprintf(fichier,"\n");         
    }
    


 }




