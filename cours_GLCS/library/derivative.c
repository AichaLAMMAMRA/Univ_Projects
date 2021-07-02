#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"
#include "derivative.h"
#define FILE "heat.h5"

double** Derivative(double** previous, double** last, int dsize[2], int rang , int size)
{
// calcul 
  int debut_i;
  int fin_i ;
  int count;

// part par proc
  if (rang < (dsize[0] % size)) 
  {
      debut_i =  (rang * (dsize[0] / size) + rang );
      fin_i = debut_i + (dsize[0] / size) + 1 ;
      count = ((dsize[0] / size) + 1);
  }
  else 
  {
      debut_i = (rang * (dsize[0] / size) + (dsize[0] % size) );
      fin_i = debut_i + (dsize[0] / size);
      count = (dsize[0] / size);
  }

  for (int i = debut_i ; i < fin_i; i++)
   {
    for (int j = 0; j < dsize[1]; j++)
     {
       last[i][j] -= previous[i][j];
     }
   }
   return last;
}

double**  read1_file_hdf5(hsize_t dsize[2],hid_t  f_id , int num_group)
{

    // lecture 
   char dataset_nom[100];
   sprintf(dataset_nom, "/%d/last", num_group);

   hid_t  dataset_file_id = H5Dopen2(f_id, dataset_nom , H5P_DEFAULT);
   hid_t fspace_id = H5Dget_space(dataset_file_id);

   H5Sget_simple_extent_dims(fspace_id , dsize , NULL);

  // recuperer current 
   double **current;

   current = (double**) malloc(dsize[0]*sizeof(double*));
   current[0] = (double*)malloc(dsize[1]*dsize[0]*sizeof(double) );


   for (int i=1; i < dsize[0]; i++)
   {
     current[i] = current[0]+i*dsize[1];
   }

   herr_t status = H5Dread(dataset_file_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &current[0][0]);

   // fermeture
   status = H5Dclose(dataset_file_id);
   status = H5Sclose(fspace_id);


   return current;
}

double**  read2_file_hdf5(hsize_t dsize[2],hid_t  f_id , int num_group)
{
  // lecture
   char dataset_nom[100];
   sprintf(dataset_nom, "/%d/previous", num_group);


   hid_t  dataset_file_id = H5Dopen2(f_id, dataset_nom , H5P_DEFAULT);
   hid_t fspace_id = H5Dget_space(dataset_file_id);

   H5Sget_simple_extent_dims(fspace_id , dsize , NULL);

  // recuperer
   double **current;

   current = (double**) malloc(dsize[0]*sizeof(double*));
   current[0] = (double*)malloc(dsize[1]*dsize[0]*sizeof(double) );

   for (int i=1; i < dsize[0]; i++)
   {
     current[i] = current[0]+i*dsize[1];
   }

   herr_t status = H5Dread(dataset_file_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &current[0][0]);

   // fermeture
   status = H5Dclose(dataset_file_id);
   status = H5Sclose(fspace_id);


   return current;
}




void write_data(hsize_t dsize[2], double** dt,int nb,int rang , int size, MPI_Comm post_comm)
{

// ecriture sur disque
  int debut_i;
  int fin_i ;
  int count;

// par proc
  if (rang < (dsize[0] % size)) 
  {
      debut_i =  (rang * (dsize[0]/size) + rang );
      fin_i = debut_i +(dsize[0]/size) + 1 ;
      count = ((dsize[0] / size)+1);
  }
  else 
  {
      debut_i = (rang * (dsize[0]/size) + (dsize[1]%size) );
      fin_i = debut_i +(dsize[0]/size);
      count = (dsize[0] / size);
  }

  hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
  H5Pset_fapl_mpio(plist_id, post_comm, MPI_INFO_NULL);

  hid_t f_id = H5Fopen("diags.h5", H5F_ACC_RDWR, plist_id);

  if (f_id<0) 
  {
  f_id = H5Fcreate("diags.h5", H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
  }

  // ecriture glob
  hid_t plist_id2 = H5Pcreate(H5P_DATASET_XFER);
  H5Pset_dxpl_mpio(plist_id2, H5FD_MPIO_COLLECTIVE);


  //dataspace

  hid_t dataspace_id_fichier = H5Screate_simple(2, dsize, NULL);
  hsize_t start_f[2] = {debut_i,0};
  hsize_t strids_f[2] = {count,dsize[1]};
  H5Sselect_hyperslab(dataspace_id_fichier, H5S_SELECT_SET, start_f, NULL, strids_f, NULL );

  hid_t dataspace_id_memoir = H5Screate_simple(2, dsize, NULL);
  hsize_t start[2] = {debut_i,0};
  hsize_t strids[2] = {count,dsize[1]};
  H5Sselect_hyperslab(dataspace_id_memoir, H5S_SELECT_SET, start, NULL, strids , NULL );


   // groupe
   char name_groupe[100];

   sprintf(name_groupe, "%d", nb);
   hid_t group_id = H5Gcreate(f_id,name_groupe, H5P_DEFAULT,H5P_DEFAULT, H5P_DEFAULT);

   // dataset
   char name[100];

   sprintf(name, "/%s/%s", name_groupe, "derivative");
   hid_t dataset_id = H5Dcreate(f_id, name , H5T_NATIVE_DOUBLE, dataspace_id_fichier, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

   // ecriture
   H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE,dataspace_id_memoir , dataspace_id_fichier,plist_id2, &dt[0][0]);


   herr_t status ;
   
   status = H5Dclose(dataset_id);
   status = H5Sclose(dataspace_id_memoir);
    status = H5Sclose(dataspace_id_fichier);
   status = H5Fclose(f_id);

}
