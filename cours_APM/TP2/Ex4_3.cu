#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda.h>
#include <sys/time.h>

#define SIZE 102400
#define MOD 102399
#define STEP 1

/* ARRAY A INITIALIZER */
void init_a(int * a)
{
    int i;
    for(i=0; i<SIZE; i++)
    {
        a[i] = 1;
    }
}

/* ARRAY B INITIALIZER */
void init_b(int * b)
{
	int i, j;

	j=0;

	for(i=0; i<SIZE-1; i++)
	{
		b[j] = i;
		j = (j+STEP)%MOD;
	}	

    b[SIZE-1] = SIZE-1;
}

/* CHECKING A VALUES */
int check_a(int * a)
{
    int i;
    int correct = 1;
	for(i=0; i<SIZE; i++)
	{
		if(a[i] != (i+1)) 
		{
         
			correct = 0;
		} 
	}	

    return correct;
}


/* CUDA FUNCTION */
__global__ void mykernel(int * a, int * b, int N)
{
    int index =  threadIdx.x;
    int i;
    for(i=index; i<N; i+=blockDim.x)
    {
        a[b[i]] = a[b[i]] + b[i];
   } 
}


int main(int argc, char * argv[])
{

	int * a = (int *)malloc(sizeof(int)*SIZE);
	int * b = (int *)malloc(sizeof(int)*SIZE);

    init_a(a);
	init_b(b);


/*  INSERT CUDA ALLOCATION AND COPY HERE */
    int * d_a, * d_b;
    cudaMalloc(&d_a, sizeof(int)*SIZE);
    cudaMalloc(&d_b, sizeof(int)*SIZE);	

    cudaMemcpy(d_a, a, sizeof(int)*SIZE, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof(int)*SIZE, cudaMemcpyHostToDevice);

	dim3 nBlocks;
	dim3 nThperBlock;

	nBlocks.x = 1;
	
	nThperBlock.x = 1024;

    struct timeval tv_start, tv_stop;

    gettimeofday(&tv_start, NULL);

	mykernel<<< nBlocks , nThperBlock >>>(d_a, d_b, SIZE);
    cudaDeviceSynchronize();
	
    gettimeofday(&tv_stop, NULL);
    
/* INSERT CUDA COPY HERE */
    cudaMemcpy(a, d_a, sizeof(int)*SIZE, cudaMemcpyDeviceToHost);


	int correct = check_a(a);;
	
	if(0 == correct)
	{
		printf("\n\n ******************** \n ***/!\\ ERROR /!\\ *** \n ******************** \n\n");
	}
	else
	{
		printf("\n\n ******************** \n ***** SUCCESS! ***** \n ******************** \n\n");
	}

    
    int nsec  = tv_stop.tv_sec - tv_start.tv_sec;
    int nusec = tv_stop.tv_usec - tv_start.tv_usec;

    if(nusec < 0)
    {
        nusec = nusec + 1000000;
        nsec = nsec - 1;
    }    

    printf("time = %d s %d us\n", nsec,  nusec);


	return 1;
}
