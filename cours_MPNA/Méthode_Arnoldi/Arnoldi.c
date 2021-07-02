#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>
#include<string.h>
#include <omp.h>




/*______ Générer une matrice aléatoire __________ */

void generate_Matrix(double matrix[], int rows, int cols) { 
      
      #pragma omp parallel for
      for(int i=0;i<rows*cols;i++)
          matrix[i]=rand()%100;        
}


/*______  inisialiser un vecteur __________ */

void init_vector(double *vector, int n, double value) { 
      #pragma omp parallel for
      for(int j=0;j<n;j++)
              vector[j]=value;
}

/*______ Produit scalaire __________ */
double prod_scalair(double *v1,double *v2,int n){

      double value=0;
       #pragma omp parallel for reduction(+:value)
      for(int i=0;i<n;i++) {     
            
            value+=v1[i]*v2[i];

          }
return value;
}

/*__________ Multiplication vector *Matrix __________*/

/*______ prod_matrix_vector __________ */

void dot(double matrix[],double vector[],int n,double result[]){
      
     #pragma omp parallel for
     
     for(int k=0; k<n; k++)
      {
          int i = k * n;
          for(int j=i,l=0; j <i+n, l<n; j++,l++)
              result[k] += matrix[j] * vector[l];
      }       
      
}

/*______ Normaliser un vecteur __________ */

double norm(double v[],int n){  
        
      return sqrt (prod_scalair(v, v,n));
} 

/*___________ Lecture de fichie ___________ */
void LectureMatrice(char *filename, double matrix[], int n){
      
      FILE *FL = fopen(filename,"r");
    
      for ( int i = 0; i < n; i ++)
        for (int j = 0; j < n; j++)
            fscanf(FL, "%lf", &matrix[i*n+j]);

              printf(" Lecture de matrice terminee\n");
      fclose(FL);
}
/*___________ Copy la mtrice A dans fichier ___________ */

void savematrice (FILE* fichier,char * filename, double matrice [], int n){
    
        fichier = fopen(filename,"w"); 
 
        if (fichier != NULL){
        
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                  fprintf(fichier,"%f\t",matrice [i*n+j]);
                }
            }
      }
 } 
/*___________ Créer un fichier et écrire des données dans un fichier ___________ */

void save_result (FILE* fichier,char * filename,double **Vm,double Hm[],int m,double t){
    
    fichier = fopen(filename,"w"); 

        if (fichier != NULL){

            fprintf (fichier,"     _________ la matrice Vm _________\n" );
            for(int i=0;i<m-1;i++) {
                for(int j=0;j<m;j++){
                  fprintf(fichier,"%f  ",Vm[i][j]);
                }
                fprintf(fichier,"\n");
            }
            
            fprintf (fichier,"\n");
            fprintf (fichier,"    _________ la matrice Hm _________\n" );
            for(int i=0;i<m-1;i++){
                for(int j=0;j<m;j++){
                  fprintf(fichier,"%f  ",Hm[i*m-1+j]);
                }
                fprintf(fichier,"\n");
            }
            fprintf(fichier,"\n the program took %f s  to execute  \n", t);
        }
}

/*______ Méthode d'Arnoldi __________ */

void Arnoldie(double matrix[],double vector[],int m,int n,double Hm[],double *Vm[]){
  
        double *w=(double*) malloc(sizeof(double*)*n);
        for (int i=0; i<n; i++) {
            
           Vm[0][i]=vector[i]/(norm(vector,n));
        }
        for (int k=0; k<m-1; k++) {

            dot(matrix,Vm[k],n,w);

            for (int j=0; j<=k; j++) {
              
                  Hm[j*(m-1)+k]=prod_scalair(w,Vm[j],n);

                  for(int i=0;i<n;i++){
	                 
                        w[i]=w[i]-Hm[j*(m-1)+k]*Vm[j][i];
                  }
            }
            
            Hm[(k+1)*(m-1)+k]=norm(w,n);
    
            if (m != k-1){
                  for (int i=0; i<n; i++) 
                        
                        Vm[k+1][i]=w[i]/Hm[(k+1)*(m-1)+k];    
            }
      }
}

/*_______ Fonction principale ________*/

int main(int argc,char* argv[]){ 
      
 
      int n= atoi(argv[1]);

      const int m= n/100;
      FILE* fichier = NULL;
     
      double* A;
      double *v;
      double *Hm;
      double **Vm;

      A = (double*) malloc(sizeof(double*)*n*n);
      v = (double*) malloc(sizeof(double*)*n);
      Hm = (double*) malloc(sizeof(double*)*n*(n-1)); 
      Vm= (double**) malloc(sizeof(double*) * m);
      
      for(int i=0; i<m; i++)
          Vm[i] = (double*) malloc(sizeof(double) * n);
     
      init_vector (v,n,1);
     

        generate_Matrix (A,n,n);
        /*char nom_fichier[ =" matriceA.data"];
        /savematrice(fichier,"matriceA.data", A, n);*/
  

        /*char nom_fichier[30];
        strcpy(nom_fichier, argv[2]);
        //LectureMatrice(nom_fichier, A, n);*/

      /*________Arnoldi _____________*/

      double start = omp_get_wtime(); 
      Arnoldie(A,v,m,n,Hm,Vm);
      double stop = omp_get_wtime();
      double elapsed_time = stop - start;
    
      save_result (fichier,"Result_Arnoldi_mthod.data",Vm,Hm, m, elapsed_time);

      printf ("\n les résultats dans le fichier : Result_Arnoldi_mthod.dat \n ");
      printf("\n the program took %f s  to execute  \n", elapsed_time);

free(A);
free(v);

return 0;
}

