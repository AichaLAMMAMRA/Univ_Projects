#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"
#include "string.h"
#include "monfichier.h"
#include "./library/mean.h"
#include "./library/derivative.h"

#define FILE "heat.h5"
/** A function to initialize the temperature at t=0
 * @param         dsize  size of the local data block (including ghost zones)
 * @param         pcoord position of the local data block in the array of data blocks
 * @param[out] dat    the local data block to initialize
 */
void init(int dsize[2], int pcoord[2], double dat[dsize[0]][dsize[1]]){
	// initialize everything to 0
	for (int yy=0; yy<dsize[0]; ++yy) {
		for (int xx=0; xx<dsize[1]; ++xx) {
			dat[yy][xx] = 0;

		}
	}
	// except the boundary condition at x=0 if our block is at the boundary itself
	if ( pcoord[1] == 0 ) {
		for (int yy=0; yy<dsize[0]; ++yy) {
			dat[yy][0] = 1000000;
		}
	}
}

//--------------------------------------------  La fonction in situ ---------------------------------------------
void in_sit(int dsize[2], double cur[dsize[0]][dsize[1]], double next[dsize[0]][dsize[1]],
            int psize[2], int rank, int size, int argument, MPI_Comm post_comm, int color){

  int dsize1[2] = {(dsize[0]-2)*psize[0],(dsize[1]-2)*psize[1]};
  int dsize2[2] = {(dsize[0]-2),(dsize[1]-2)};

  //Allocation d'un buffer afin de stocker le Cur
  double **curr;
  malloc2D(&curr, dsize1[0], dsize1[1]);

  //Allocation d'un buffer afinafin de stocker local_Cur
  double **local_curr;
  malloc2D(&local_curr, dsize2[0], dsize2[1]);

  //copier le next dans le local_cur
  for (size_t i = 0; i < dsize2[0]; i++) {
    for (size_t j = 0; j < dsize2[1]; j++) {
      local_curr[i][j] = next[i+1][j+1];
    }
  }

  int starts[2]   = {0,0};
  int sizes[2]    = {dsize1[0],dsize1[1]};
  int subsizes[2] = {dsize1[0]/psize[0] ,dsize1[1]/psize[1]};
  
  MPI_Datatype type, subarrtype;
  MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_DOUBLE, &type);
  MPI_Type_create_resized(type, 0, dsize1[1]/psize[1]*sizeof(double), &subarrtype);
  MPI_Type_commit(&subarrtype);


  double *globalptr=NULL;   globalptr = &(curr[0][0]);

  //le nombre d'element à transferer 
  int sendcounts[psize[0]*psize[1]];
  int displs[psize[0]*psize[1]];

  for (int i=0; i<psize[0]*psize[1]; i++)
  {
      sendcounts[i] = 1;
  }

  //Calcul le pas de stockage :
  int disp = 0;
  for (int i=0; i<psize[0]; i++) {
      for (int j=0; j<psize[1]; j++) {
          displs[i*psize[1]+j] = disp;
          disp += 1;
      }
      disp += ((dsize1[0]/psize[0])-1)*psize[1];
  }

  for (size_t i = 0; i < size; i++) {
    MPI_Gatherv(&(local_curr[0][0]), dsize1[0]*dsize1[1]/(psize[0]*psize[1]),  MPI_DOUBLE,
                 globalptr, sendcounts, displs, subarrtype,i,  post_comm);
  }

  // Calcul de la moyenne :
  double* moyenne_x = x_mean(curr, dsize1, rank , size,post_comm);
  double* moyenne_y = y_mean(curr, dsize1, rank , size,post_comm);
  double global_mean =  mean(moyenne_x,dsize1[0],rank);
  write_mean(moyenne_x, moyenne_y , global_mean ,dsize1, argument, rank);
  MPI_Barrier(post_comm );



  //Allocation d'un buffer afinafin de stocker prev
  double **local_prev;
  malloc2D(&local_prev, dsize2[0], dsize2[1]);
  //Allocation d'un buffer afin de stocker prev;
  double **prev;
  malloc2D(&prev, dsize1[0],dsize1[1]);

    //copier le cur dans le local_prev
  for (size_t i = 0; i <dsize2[0]; i++) {
    for (size_t j = 0; j < dsize2[1]; j++){
      local_prev[i][j] = cur[i+1][j+1];
    }
  }

  globalptr = &(prev[0][0]);
  //Rassembler les données en utilisant gatherv
  for (size_t i = 0; i < size; i++) {
    MPI_Gatherv(&(local_prev[0][0]), dsize1[0]*dsize1[1]/(psize[0]*psize[1]),  MPI_DOUBLE,
               globalptr, sendcounts, displs, subarrtype,i, post_comm);
  }

  //Calcul derivative
  prev = Derivative(prev, curr, dsize1, rank ,size);
  hsize_t siz[2]={dsize1[0],dsize1[1]};
  write_data(siz,  prev ,argument, rank , size , post_comm);

}


//----------------------------------------------------  fonction in transit------------------------------------------------------------------
void in_transit(int dsize[2], double cur[dsize[0]][dsize[1]], double next[dsize[0]][dsize[1]], int psize[2] ,
                int rank , int size, int argument, MPI_Comm post_comm, int color, int post_moi, int post_size){

  int dsize1[2] = {(dsize[0]-2)*psize[0],(dsize[1]-2)*psize[1]};
  int dsize2[2] = {(dsize[0]-2),(dsize[1]-2)};


  //Allocation d'un buffer afin de stocker le Cur
  double **curr;
  malloc2D(&curr, dsize1[0],dsize1[1]);

  //Allocation d'un buffer afinafin de stocker local_Cur
  double **local_curr;
  malloc2D(&local_curr, dsize2[0], dsize2[1]);

  //copier le next dans le local_cur
  for (size_t i = 0; i <dsize2[0]; i++) {
    for (size_t j = 0; j < dsize2[1]; j++){
      local_curr[i][j] = next[i+1][j+1];
    }
  }

  int starts[2]   = {0,0};
  int sizes[2]    = {dsize1[0],dsize1[1]};
  int subsizes[2] = {dsize1[0]/psize[0] ,dsize1[1]/psize[1]};

  MPI_Datatype type, subarrtype;
  MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_DOUBLE, &type);
  MPI_Type_create_resized(type, 0, dsize1[1]/psize[1]*sizeof(double), &subarrtype);
  MPI_Type_commit(&subarrtype);


  double *globalptr=NULL;   globalptr = &(curr[0][0]);

  //le nombre d'element à transferer
  int displs[psize[0]*psize[1]]; 
  int sendcounts[psize[0]*psize[1]];

  for (int i=0; i<psize[0]*psize[1]; i++){
      sendcounts[i] = 1;
    }
  //Calcul le pas de stockage :
  int disp = 0;
  for (int i=0; i<psize[0]; i++) {
      for (int j=0; j<psize[1]; j++) {
          displs[i*psize[1]+j] = disp;
          disp += 1;
      }
      disp += ((dsize1[0]/psize[0])-1)*psize[1];
  }

  //Rasembler les donnees en utilisant la fonction gatherv
  for (size_t i = 0; i < size; i++) {
    MPI_Gatherv(&(local_curr[0][0]), dsize1[0]*dsize1[1]/(psize[0]*psize[1]), MPI_DOUBLE,
              globalptr, sendcounts, displs, subarrtype, i, MPI_COMM_WORLD);
  }
  //Calcul de la moyenne
  if (color == 0) {
  double* moyenne_x = x_mean(curr, dsize1, post_moi , post_size,post_comm);
  double* moyenne_y = y_mean(curr, dsize1, post_moi , post_size,post_comm);

  double global_mean =  mean(moyenne_x,dsize1[0],post_moi);

  write_mean(moyenne_x, moyenne_y , global_mean ,dsize1, argument, post_moi);
  MPI_Barrier(post_comm );
}


//Allocation d'un buffer afinafin de stocker prev
  double **prev;
  malloc2D(&prev, dsize1[0],dsize1[1]);

  //Allocation d'un buffer afinafin de stocker local prev
  double **local_prev;
  malloc2D(&local_prev, dsize2[0], dsize2[1]);

  //copier le cur dans le local_prev
  for (size_t i = 0; i <dsize2[0]; i++) {
    for (size_t j = 0; j < dsize2[1]; j++){
      local_prev[i][j] = cur[i+1][j+1];
    }
  }

  globalptr = &(prev[0][0]);

  //rassembler les donnees par gatherv
  for (size_t i = 0; i < size; i++) {
    MPI_Gatherv(&(local_prev[0][0]), dsize1[0]*dsize1[1]/(psize[0]*psize[1]), MPI_DOUBLE,
              globalptr, sendcounts, displs, subarrtype,i, MPI_COMM_WORLD);
  }
  if (color == 0) {
    //Calcul derivative
  prev = Derivative(prev, curr, dsize1, post_moi ,post_size);

  hsize_t siz[2]={dsize1[0],dsize1[1]};
  write_data(siz,  prev ,argument, post_moi , post_size , post_comm);

  }
}

/** A function to compute the temperature at t+delta_t based on the temperature at t
 * @param      	  dsize  size of the local data block (including ghost zones)
 * @param     	  pcoord position of the local data block in the array of data blocks
 * @param[in]  cur    	the current value (t) of the local data block
 * @param[out] next   the next value (t+delta_t) of the local data block
 */
//-----------------------------------------------------------------------------------------------------------------------
void iter(int dsize[2], double cur[dsize[0]][dsize[1]], double next[dsize[0]][dsize[1]],int psize[2],int pcoord[2], int ii, int* argument , 
	  int size_of_arg, int rank , int size , int type ,int color, MPI_Comm post_comm , int post_moi , int post_size){

	int xx, yy;
	// copy the boundary values at x=0 (Dirichlet boundary condition)
	for (xx=0; xx<dsize[1]; ++xx) {
		next[0][xx] = cur[0][xx];
	}
	for (yy=1; yy<dsize[0]-1; ++yy) {
		// copy the boundary values at y=0 (Dirichlet boundary condition)
		next[yy][0] = cur[yy][0];
		for (xx=1; xx<dsize[1]-1; ++xx) {
			next[yy][xx] =
			        (cur[yy][xx]   *.5)
			        + (cur[yy][xx-1] *.125)
			        + (cur[yy][xx+1] *.125)
			        + (cur[yy-1][xx] *.125)
			        + (cur[yy+1][xx] *.125);
		}
		// copy the boundary values at y=YMAX (Dirichlet boundary condition)
		next[yy][dsize[1]-1] = cur[yy][dsize[1]-1];
	}
	// copy the boundary values at x=XMAX (Dirichlet boundary condition)
	for (xx=0; xx<dsize[1]; ++xx) {
		next[dsize[0]-1][xx] = cur[dsize[0]-1][xx];
	}

  	//---------------------------------------------------------------------
	for (size_t i = 0; i < size_of_arg; i++) {
		if ( argument[i] == ii ) {
      			if (type == 0) {
          			in_sit(dsize,cur, next, psize, rank, size, argument[i], post_comm, color);
      			}
      			if (type == 1) {
         			 in_transit(dsize,cur, next, psize, rank, size, argument[i], post_comm, color, post_moi, post_size);
        		}
		break;
		}
	}
  	//-----------------------------------------------------------------------
}

/** A function to update the values of the ghost zones
 * @param      	  cart_comm a MPI Cartesian communicator including all processes arranged in grid
 * @param         dsize  size of the local data block (including ghost zones)
 * @param[out] next       the next value (t+delta_t) of the local data block
 */
void exchange(MPI_Comm cart_comm, int dsize[2], double cur[dsize[0]][dsize[1]])
{
	MPI_Status status;
	int rank_source, rank_dest;
	static MPI_Datatype column, row;
	static int initialized = 0;

    	// Build the MPI datatypes if this is the first time this function is called
	if ( !initialized ) {
        	// A vector column when exchanging width neighbours on left/right
		MPI_Type_vector(dsize[0]-2, 1, dsize[1], MPI_DOUBLE, &column);
		MPI_Type_commit(&column);
        	// A row column when exchanging width neighbours on top/down
		MPI_Type_contiguous(dsize[1]-2, MPI_DOUBLE, &row);
		MPI_Type_commit(&row);
		initialized = 1;
	}

	// send to the bottom, receive from the top
	MPI_Cart_shift(cart_comm, 0, 1, &rank_source, &rank_dest);
	MPI_Sendrecv(&cur[dsize[0]-2][1], 1, row, rank_dest,   100, /* send row before ghost */
	    	&cur[0][1], 1, row, rank_source, 100, /* receive 1st row (ghost) */
	    	cart_comm, &status);

	// send to the top, receive from the bottom
	MPI_Cart_shift(cart_comm, 0, -1, &rank_source, &rank_dest);
	MPI_Sendrecv(&cur[1][1], 1, row, rank_dest,   100, /* send column after ghost */
	    	&cur[dsize[0]-1][1], 1, row, rank_source, 100, /* receive last column (ghost) */
	    	cart_comm, &status);

	// send to the right, receive from the left
	MPI_Cart_shift(cart_comm, 1, 1, &rank_source, &rank_dest);
	MPI_Sendrecv(&cur[1][dsize[1]-2], 1, column, rank_dest,   100, /* send column before ghost */
	    	&cur[1][0], 1, column, rank_source, 100, /* receive 1st column (ghost) */
	    	cart_comm, &status);

	// send to the left, receive from the right
	MPI_Cart_shift(cart_comm, 1, -1, &rank_source, &rank_dest);
	MPI_Sendrecv(&cur[1][1], 1, column, rank_dest,   100, /* send column after ghost */
	    	&cur[1][dsize[1]-1], 1, column, rank_source, 100, /* receive last column (ghost) */
	    	cart_comm, &status);
}

/** A function to parse command line arguments
 * @param      	  argc    number of arguments received on the command line
 * @param[in]  argv       values of arguments received on the command line
 * @param[out] nb_iter   number of iterations to execute
 * @param[out] dsize     size of the local data block (including ghost zones)
 * @param[out] cart_comm a MPI Cartesian communicator including all processes arranged in grid
 */
void parse_args( int argc, char *argv[], int *nb_iter, int dsize[2], MPI_Comm *cart_comm, int psize[2])
{
	if ( argc < 4 ) {
		printf("Usage: %s <Nb_iter> <height> <width>\n", argv[0]);
		exit(1);
	}

	// total number of processes
	int comm_size; MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	//int psize[2];

	// number of processes in the x dimension
	psize[0] = sqrt(comm_size);
	if ( psize[0]<1 ) psize[0]=1;
	// number of processes in the y dimension
	psize[1] = comm_size/psize[0];
	// make sure the total number of processes is correct
	if (psize[0]*psize[1] != comm_size) {
		fprintf(stderr, "Error: invalid number of processes\n");
		abort();
    	}

	// number of iterations
	*nb_iter = atoi(argv[1]);

	// global width of the problem
	dsize[1] = atoi(argv[3]);
	if ( dsize[1]%psize[1] != 0) {
		fprintf(stderr, "Error: invalid problem width\n");
		abort();
	}
	// width of the local data block (add boundary or ghost zone: 1 point on each side)
	dsize[1]  = dsize[1]/psize[1]  + 2;

	// global height of the problem
	dsize[0] = atoi(argv[2]);
	if ( dsize[0]%psize[0] != 0) {
		fprintf(stderr, "Error: invalid problem height\n");
		abort();
	}
	// height of the local data block (add boundary or ghost zone: 1 point on each side)
	dsize[0] = dsize[0]/psize[0] + 2;

    	// creation of the communicator
	int cart_period[2] = { 0, 0 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, psize, cart_period, 1, cart_comm);
}

int main( int argc, char* argv[] )
{
	// initialize the MPI library
	MPI_Init(&argc, &argv);

	// parse the command line arguments
  int nb_iter;
	int dsize[2];
	int psize[2];
  MPI_Comm cart_comm;
	parse_args(argc, argv, &nb_iter, dsize, &cart_comm, psize);

  // find the coordinate of the local process
	int pcoord_1d; MPI_Comm_rank(MPI_COMM_WORLD, &pcoord_1d);
	int pcoord[2]; MPI_Cart_coords(cart_comm, pcoord_1d, 2, pcoord);
 	//----------------------------------------------------------------------------------------------
	H5Eset_auto(H5E_DEFAULT, NULL, NULL);
	int rank, size, color, nbr_proc, size_of_arg;
	int type = 1;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm post_comm;
	int* pas_de_temps;
	// in situ
	if (strcmp (argv[4],"-s") == 0) {
	size_of_arg = (argc - 5);
   		pas_de_temps = malloc((argc - 5) * sizeof(int));
		type = 0;
    		
		for (size_t i = 0; i < (argc - 5); i++) {
			pas_de_temps[i] = atoi(argv[i+5]);
      post_comm = MPI_COMM_WORLD;

		}
	}

  	int post_moi , post_size;
// in transit
	if (strcmp (argv[4], "-t") == 0){
	size_of_arg = (argc - 6);
	pas_de_temps = malloc((argc - 6) * sizeof(int));
	type = 0;

	for (size_t i = 0; i < (argc - 6); i++){
		pas_de_temps[i] = atoi(argv[i+6]);

    		}
    nbr_proc = atoi(argv[5]);
  	if (rank < nbr_proc)
        color = 0;
    else
  		  color = 1;

  	MPI_Comm_split(MPI_COMM_WORLD, color , pcoord_1d, &post_comm);
  	MPI_Comm_rank(post_comm, &post_moi);
  	MPI_Comm_size(post_comm, &post_size);
 }

	//-----------------------------------------------------------------------------
	// allocate data for the current iteration
	double(*cur)[dsize[1]]  = malloc(sizeof(double)*dsize[1]*dsize[0]);

	// initialize data at t=0
	init(dsize, pcoord, cur);

	// allocate data for the next iteration
	double(*next)[dsize[1]] = malloc(sizeof(double)*dsize[1]*dsize[0]);

	//-----------------------------------------------------------------------------
	char name[100]; 
	char fname[100]; 
	//sprintf(fname, "heat%dx%d.h5", pcoord[0], pcoord[1]); // recuperation du nom de chaque fichier

	//La creaction d'un fichier avec le nom du rang
	//hid_t file_id = H5Fcreate(fname , H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT); 

	//creation d'un fichier heat.h5
	hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
	H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
	hid_t file_id = H5Fcreate("heat.h5", H5F_ACC_TRUNC, H5P_DEFAULT,plist_id);
	//-----------------------------------------------------------------------------

	// the main (time) iteration

	for (int ii=0; ii<nb_iter; ++ii) {
		sprintf(name,"/step%d",ii);// recuperation du nom de chaque dataset

		//file_avec_ghost_zones(file_id, dsize,cur, next, psize, pcoord, name); /* appel fonction avec les zones fantomes*/
		//file_sans_ghost_zones(file_id, dsize,cur, next, psize, pcoord, name); /* appel fonction sans les zones fantomes*/
		write_file_hdf5(file_id, dsize,cur, psize, pcoord, name);/* appel fonction pour ecrire en parallele*/

		// compute the temperature at the next iteration
		iter(dsize, cur, next, psize, pcoord, ii , pas_de_temps,size_of_arg, rank , size , type , color , post_comm  ,post_moi , post_size);

	 	// update ghost zones
		exchange(cart_comm, dsize, next);

	 	// switch the current and next buffers
		double (*tmp)[dsize[1]] = cur; cur = next; next = tmp;
		}

	H5Fclose (file_id); /*fermer le fichier*/

	free(cur);
	free(next);

	// finalize MPI
	MPI_Finalize();

	return 0;
}
