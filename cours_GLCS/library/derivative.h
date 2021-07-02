#ifndef __DERIVATIVE_H__
#define __DERIVATIVE_H__



#include "hdf5.h"
#define FILE "heat.h5"

double**  read1_file_hdf5(hsize_t dsize[2],hid_t  file_id , int num_group);
double**  read2_file_hdf5(hsize_t dsize[2],hid_t  file_id , int num_group);
double** Derivative(double** previous, double** last, int dsize[2], int rank , int size);
void write_data(hsize_t dsize[2], double** dt,int nb,int rank , int size, MPI_Comm post_comm);

#endif
