#include <iostream>
#include <string.h>
#include "FreeImage.h"

#define WIDTH 1920
#define HEIGHT 1024
#define BPP 24 // Since we're outputting three 8 bit RGB values

#define BLOCK_WIDTH 32

using namespace std;

__global__ void img_copy(unsigned int* src, unsigned int* dest, unsigned width, unsigned height)
{
  int col   = threadIdx.x + blockDim.x * blockIdx.x;
  int line  = threadIdx.y + blockDim.y * blockIdx.y;

  if((col < width) && (line < height))
  {
    dest[((line * width + col) * 3) + 0] = src[((line * width + col) * 3 )+ 0];
    dest[((line * width + col) * 3) + 1] = src[((line * width + col) * 3 )+ 1];
    dest[((line * width + col) * 3) + 2] = src[((line * width + col) * 3 )+ 2];
  }
}

int main (int argc , char** argv)
{
  FreeImage_Initialise();
  const char *PathName = "lena.jpg";
  const char *PathDest = "new_lena.png";
  // load and decode a regular file
  FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(PathName);

  FIBITMAP* bitmap = FreeImage_Load(FIF_JPEG, PathName, 0);

  if(! bitmap )
    exit( 1 ); //WTF?! We can't even allocate images ? Die !

  unsigned width  = FreeImage_GetWidth(bitmap);
  unsigned height = FreeImage_GetHeight(bitmap);
  unsigned pitch  = FreeImage_GetPitch(bitmap);

  fprintf(stderr, "Processing Image of size %d x %d\n", width, height);

  unsigned int *img = (unsigned int*) malloc(sizeof(unsigned int) * 3 * width * height);

  unsigned int* cuda_src;
  cudaMalloc(&cuda_src, sizeof(unsigned int) * 3 * width * height); 
  unsigned int* cuda_dest;
  cudaMalloc(&cuda_dest, sizeof(unsigned int) * 3 * width * height); 

  BYTE *bits = (BYTE*)FreeImage_GetBits(bitmap);
  for ( int y =0; y<height; y++)
  {
    BYTE *pixel = (BYTE*)bits;
    for ( int x =0; x<width; x++)
    {
      int idx = ((y * width) + x) * 3;
      img[idx + 0] = pixel[FI_RGBA_RED];
      img[idx + 1] = pixel[FI_RGBA_GREEN];
      img[idx + 2] = pixel[FI_RGBA_BLUE];
      pixel += 3;
    }
    // next line
    bits += pitch;
  }

  cudaMemcpy(cuda_src, img, 3 * width * height * sizeof(unsigned int), cudaMemcpyHostToDevice);

  int nbBlocksx = width / BLOCK_WIDTH;
  if(width % BLOCK_WIDTH) nbBlocksx++;

  int nbBlocksy = height / BLOCK_WIDTH;
  if(height % BLOCK_WIDTH) nbBlocksy++;

  fprintf(stderr, "(%d, %d) blocks of size (%d, %d)\n", nbBlocksx, nbBlocksy, BLOCK_WIDTH, BLOCK_WIDTH);

  dim3 gridSize(nbBlocksx, nbBlocksy);
  dim3 blockSize(BLOCK_WIDTH, BLOCK_WIDTH);

  img_copy<<<gridSize, blockSize>>>(cuda_src, cuda_dest, width, height);

  cudaMemcpy(img, cuda_dest, 3 * width * height * sizeof(unsigned int), cudaMemcpyDeviceToHost);

  bits = (BYTE*)FreeImage_GetBits(bitmap);
  for ( int y =0; y<height; y++)
  {
    BYTE *pixel = (BYTE*)bits;
    for ( int x =0; x<width; x++)
    {
      RGBQUAD newcolor;

      int idx = ((y * width) + x) * 3;
      newcolor.rgbRed = img[idx + 0];
      newcolor.rgbGreen = img[idx + 1];
      newcolor.rgbBlue = img[idx + 2];

      if(!FreeImage_SetPixelColor(bitmap, x, y, &newcolor))
      { fprintf(stderr, "(%d, %d) Fail...\n", x, y); }

      pixel+=3;
    }
    // next line
    bits += pitch;
  }

  if( FreeImage_Save (FIF_PNG, bitmap , PathDest , 0 ))
    cout << "Image successfully saved ! " << endl ;
  FreeImage_DeInitialise(); //Cleanup !

  free(img);
  cudaFree(cuda_dest);
  cudaFree(cuda_src);
}
