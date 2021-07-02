
/* Les prototypes des fonctions du fichier FonctionSeq.c */


double norm(double *vector,int n);
void affiche_matrix(double **A,int m);
void generate_Matrix(double **matrix, int n);
void init_matrix(double **mat, double n, double v);
void init_vector(double *vector, int n, double value);
void read_data(char *filename, double ** matricx, int n);
double prod_scalaire(double *vector1,double *vector2,int n);
double *prod_mtrix_vector(double **matrix,double *vector,int n);
double* dev_vecteur_scal(double *vector,double scal,int n, double *result);
double* prod_vector_scal(double *vector, double scal, int n,double* result);
void save_results(FILE* fichier,char * filename,double value, double* vector,int n,double elapsed_time);
















