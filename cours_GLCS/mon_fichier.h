/*----------------------------------- 1.2/ Ecriture locale avec zones fantomes -------------------------- */

void file_avec_ghost_zones(hid_t file_id, int dsize[2], double cur[dsize[0]][dsize[1]], double next[dsize[0]][dsize[1]], int psize[2],int pcoord[2], char name[100]) {

	hsize_t dims [2] = {dsize[0], dsize[1]};

	//creation d'un data space pour le dataset
	hid_t mspace_id = H5Screate_simple(2, dims, NULL);

	//la creation de dataset 
	hid_t dset_id = H5Dcreate(file_id , name, H5T_IEEE_F64LE, mspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, mspace_id, H5S_ALL , H5P_DEFAULT, cur);

	//fermer les dataset
	H5Dclose(dset_id);
	//fermer les data space
	H5Sclose(mspace_id);
}


/*----------------------------------- 1.3 Ecriture locale sans zones fantomes -------------------------- */

void file_sans_ghost_zones(hid_t file_id, int dsize[2], double cur[dsize[0]][dsize[1]], double next[dsize[0]][dsize[1]], int psize[2],int pcoord[2], char name[100]) {

	hsize_t dims [2] = {dsize[0], dsize[1]};
	hsize_t dims2 [2] = {dsize[0]-2, dsize[1]-2};

	//creation d'un data spac pour le dataset
	hid_t mspace_id = H5Screate_simple(2, dims, NULL);
	hid_t fspace_id = H5Screate_simple(2, dims2, NULL);
	hsize_t start[2] = {1, 1};
	hsize_t strids[2] = {dims[1]-2, dims[0]-2};
	H5Sselect_hyperslab(mspace_id, H5S_SELECT_SET, start, NULL, strids, NULL);

	//la creation de dataset
	hid_t dset_id = H5Dcreate(file_id , name, H5T_IEEE_F64LE, fspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, mspace_id, H5S_ALL , H5P_DEFAULT, cur);

	//fermer les dataset
	H5Dclose(dset_id);
	//fermer les data space
	H5Sclose(fspace_id);
	H5Sclose(mspace_id);
}

/*----------------------------------------------------- 1.4 Ecriture prallèles ------------------------------------------------- */

void write_file_hdf5(hid_t file_id, int dsize[2], double cur[dsize[0]][dsize[1]], int psize[2], int pcoord[2], char name[100]) {

	//transfert de données vers un collectif commun 
	hid_t plist2_id = H5Pcreate(H5P_DATASET_XFER);
  	H5Pset_dxpl_mpio(plist2_id, H5FD_MPIO_COLLECTIVE);

	//Reservation d'une zone mémoire pour recopier les donnes 
	hsize_t dims [2] = {dsize[0],dsize[1]};
	hid_t mspace_id = H5Screate_simple(2, dims, NULL);
	hsize_t start[2] = {1, 1};
	hsize_t strids[2] = {dims[0]-2,dims[1]-2};
	H5Sselect_hyperslab(mspace_id, H5S_SELECT_SET, start, NULL, strids , NULL);

	//La creation du data space dans le fichier pour le dataset
	hsize_t dims2 [2] = {(dsize[0]-2)*psize[0], (dsize[1]-2)*psize[1]};
	hid_t fspace_id = H5Screate_simple(2, dims2, NULL);
	hsize_t start_f[2] = {pcoord[0]*(dsize[0]-2), pcoord[1]*(dsize[1]-2)};
	hsize_t strids_f[2] = {dims[0]-2,dims[1]-2};
	H5Sselect_hyperslab(fspace_id, H5S_SELECT_SET, start_f, NULL, strids_f, NULL);

	//la creation de dataset
	hid_t dset_id = H5Dcreate(file_id, name, H5T_IEEE_F64LE, fspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, mspace_id, fspace_id, plist2_id, &cur[0][0]);


	//fermer les dataset
	H5Dclose(dset_id);
	//fermer les data space
	H5Sclose(fspace_id);
	H5Sclose(mspace_id);

}


/*---------------------------------------------------- partie 3 -------------------------------------------- */ 

 //creation de tableau contiguous
int malloc2D(double ***array, int n, int m) {
     int i;
     /* allocate the n*m contiguous items */
     double *p = malloc(n*m*sizeof(double));
         /* allocate the row pointers into the memory */
     (*array) = malloc(n*sizeof(double*));


     /* set up the pointers into the contiguous memory */
     for (i=0; i<n; i++)
        (*array)[i] = &(p[i*m]);
        return 0;
}



























