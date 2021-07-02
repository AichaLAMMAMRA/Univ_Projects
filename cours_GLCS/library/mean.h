#ifndef __MEAN_H__
#define __MEAN_H__

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"
#define FILE "heat.h5"




int* get_group_nb_name(hid_t  file_id, hsize_t *nobj);
double**  read_dataset_hdf5(hsize_t dsize[2], hid_t  file_id, int num_group);
double mean(double* moyenneXY, int dsize, int rank);
double* x_mean(double** data, int dsize[2], int rank, int size, MPI_Comm post_comm);
double* y_mean(double** cur, int dsize[2], int rank, int size, MPI_Comm post_comm);
void write_mean(double* V1, double* V2, double mean, int dsize[2], int nb, int rank );

#endif
