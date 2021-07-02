#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"
#include "mean.h"
#define FILE "heat.h5"

int* get_of_name(hid_t  f_id , hsize_t *nobj)
{
  char group_nom[100];

  //ouvrir
  hid_t  grp = H5Gopen(f_id, "/" , H5P_DEFAULT);
  H5Gget_num_objs(grp, nobj);

  //recuperer
  int *resultat ; resultat = malloc(*nobj*sizeof(int));


  for (int i = 0; i < *nobj; i++) 
  {
   H5Gget_objname_by_idx(grp, (hsize_t)i, group_nom, 100);
   resultat[i] = atoi(group_nom);
  }
    return resultat;
}

double**  read_dataset_hdf5(hsize_t dsize[2], hid_t f_id, int num_group)
{
  // lecture
   char dataset_name[100];

   sprintf(dataset_name, "/%d/last", num_group);
   hid_t  dataset_file_id = H5Dopen2(f_id, dataset_name , H5P_DEFAULT);
   hid_t fspace_id = H5Dget_space(dataset_file_id);

   H5Sget_simple_extent_dims(fspace_id , dsize , NULL);

   double **current;

   current = (double**) malloc(dsize[0]*sizeof(double*));
   current[0] = (double*)malloc(dsize[1]*dsize[0]*sizeof(double) );

   for (int i=1; i < dsize[0]; i++)
   {
     current[i] = current[0]+i*dsize[1];
   }

   herr_t status = H5Dread(dataset_file_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &current[0][0]);

   status = H5Dclose(dataset_file_id);
   status = H5Sclose(fspace_id);


   return current;
}


double* x_mean(double** data, int dsize[2] , int rang , int size , MPI_Comm post_comm){

double * local_x;
int debut_i;
int fin_i ;
int count;
double * x_mean ;

//allocation
if (rang == 0) 
{
  x_mean = malloc(sizeof(double) * dsize[0]);
}

  if (rang < (dsize[0] % size)) 
  {
      local_x = malloc(sizeof(double) * ((dsize[0] / size) + 1));
      debut_i =   (rang * (dsize[0]/size) + rang );
      fin_i = debut_i +(dsize[0] / size) + 1 ;
      count = ((dsize[0]/size)+1);
  }
  else 
  {
      local_x = malloc(sizeof(double) * (dsize[0]/size));
      debut_i = (rang * (dsize[0]/size) + (dsize[0]%size) );
      fin_i = debut_i +(dsize[0]/size);
      count = (dsize[0]/size);
  }

//calcul
  double somme;

  for (int i = 0; i < fin_i-debut_i ; i++)
  {
   somme = 0;
   for (int j = 0; j < dsize[1]; j++)
       {
         somme = somme + data[debut_i+i][j];
       }
   local_x[i] = somme / dsize[1];
  }

// proc 0 recoit  tout
  MPI_Gather(local_x, count , MPI_DOUBLE,x_mean, count , MPI_DOUBLE, 0, post_comm);

  if (rang == 0) 
  {
  return x_mean ;
  }
}

double* y_mean(double** current, int dsize[2], int rang, int size , MPI_Comm post_comm)
{
  double * local_y;
  int debut_i;
  int fin_i ;
  int count;
  double * y_mean ;
  double tmp;

//Allocation par zero
  if (rang == 0) 
  {
    y_mean = malloc(sizeof(double) * dsize[1]);
  }

  if (rang < (dsize[1]%size)) 
  {
        local_y = malloc(sizeof(double) * ((dsize[1]/size)+1));
        debut_i =  (rang * (dsize[1] / size) + rang );
        fin_i = debut_i +(dsize[1] / size) + 1 ;
        count = ((dsize[1] / size)+1);
    }
    else 
    {
        local_y = malloc(sizeof(double) * (dsize[1] / size));
        debut_i = (rang * (dsize[1] / size) + (dsize[1] % size) );
        fin_i = debut_i + (dsize[1] / size);
        count = (dsize[1] / size);
    }

  for (size_t i = 0; i < fin_i - debut_i; i++) {
    tmp = 0;
    for (size_t j = 0; j < dsize[0]; j++) {
        tmp += current[j][debut_i + i];

    }
    local_y[i] = tmp / dsize[0];
  }
// zero recoit tout
  MPI_Gather(local_y, count , MPI_DOUBLE,y_mean, count , MPI_DOUBLE, 0,  post_comm);

  if (rang == 0) 
  {
  return y_mean ;
  }

}


double mean(double* meanXY, int dsize, int rang ) 
{
  // moyenne
  if (rang == 0) 
  {
    double mean = 0;
    for (size_t i = 0; i < dsize ; i++) 
    {
      mean += meanXY[i];
    }

    return mean / dsize;
  }

}

void write_mean(double* V1 , double* V2, double mean ,int dsize[2], int nb, int rang )
{
  // ecriture par zero
  if (rang == 0) 
  {

	 hid_t f_id, dataspace_id1, dataspace_id2,dataspace_id3;
	 herr_t status;
   hid_t dataset_id1, dataset_id2, dataset_id3;

   // open or create

   f_id = H5Fcreate("diags.h5", H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);

   if (f_id < 0) 
   {
    f_id = H5Fopen("diags.h5", H5F_ACC_RDWR, H5P_DEFAULT);
   }

   char nom_groupe[100];
   sprintf(nom_groupe, "%d", nb);
   hid_t group_id = H5Gcreate(f_id,nom_groupe, H5P_DEFAULT,H5P_DEFAULT, H5P_DEFAULT);


  hsize_t dims1[1] = {dsize[0]};
  dataspace_id1 = H5Screate_simple(1, dims1, NULL);

  char name[100];
	sprintf(name, "/%s/%s", nom_groupe, "x_mean");
  dataset_id1 = H5Dcreate(f_id, name , H5T_IEEE_F64LE, dataspace_id1, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id1, H5T_IEEE_F64LE, H5S_ALL, H5S_ALL,H5P_DEFAULT, &V1[0] );

  hsize_t dims2[1] = {dsize[1]};
  dataspace_id2 = H5Screate_simple(1, dims2, NULL);
  sprintf(name, "/%s/%s", nom_groupe, "Y_mean");
  dataset_id2 = H5Dcreate(f_id, name, H5T_IEEE_F64LE, dataspace_id2, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id2, H5T_IEEE_F64LE, H5S_ALL, H5S_ALL,H5P_DEFAULT, &V2[0] );


  dataspace_id3 = H5Screate( H5S_SCALAR );
  sprintf(name, "/%s/%s", nom_groupe, "mean");
  dataset_id3 = H5Dcreate(f_id, name, H5T_IEEE_F64LE, dataspace_id3, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id3, H5T_IEEE_F64LE, H5S_ALL, H5S_ALL,H5P_DEFAULT, &mean );

// fermeture
  status = H5Dclose(dataset_id1);
  status = H5Dclose(dataset_id2);
  status = H5Dclose(dataset_id3);
  status = H5Sclose(dataspace_id1);
  status = H5Sclose(dataspace_id2);
  status = H5Sclose(dataspace_id3);
  status = H5Gclose(group_id);
	remove("heat.h5");
  status = H5Fclose(f_id);
}

}
