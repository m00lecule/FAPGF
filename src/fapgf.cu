#include "cuda.h"
#include <stdio.h>

__global__
void calculate_distances(double r_d, int radix, bool* contaminated, double* R, double* G, double* B, int height, int width)
{
  extern __shared__ double s[];
  double *buffor_r = s;                       
  double *buffor_g = &buffor_r[(2*radix+1)*(blockDim.x + 2*radix)];
  double *buffor_b = &buffor_g[(2*radix+1)*(blockDim.x + 2*radix)];

//load data  
for(int i = threadIdx.x ; i < blockDim.x + 2 * radix && (i + blockDim.x * blockIdx.x < width); i += blockDim.x )
  for(int j = 0 ; j < 2*radix ; ++j)
  {
    buffor_r[(j + 1)*(2*radix + 1) + i] = R[(i + blockIdx.x*blockDim.x)*height + j];
    buffor_g[(j + 1)*(2*radix + 1) + i] = G[(i + blockIdx.x*blockDim.x)*height + j];
    buffor_b[(j + 1)*(2*radix + 1) + i] = B[(i + blockIdx.x*blockDim.x)*height + j];
  }

  for(int h = radix ; h < height - radix ; ++h)
  {
    //roll rows by one
    for(int i = threadIdx.x ; i < blockDim.x + 2 * radix && (i + blockDim.x * blockIdx.x < width); i += blockDim.x )
      for(int j = 0 ; j < 2*radix ; ++j)
      {
        buffor_r[j*(2*radix + 1) + i] = buffor_r[(j + 1)*(2*radix + 1) + i];
        buffor_g[j*(2*radix + 1) + i] = buffor_g[(j + 1)*(2*radix + 1) + i];
        buffor_b[j*(2*radix + 1) + i] = buffor_b[(j + 1)*(2*radix + 1) + i];
      } 

    //read next row  
    for(int i = threadIdx.x ; i < blockDim.x + 2 * radix && (i + blockDim.x * blockIdx.x < width); i += blockDim.x )
    {
        buffor_r[(2*radix)*(2*radix + 1) + i] = R[(i + blockIdx.x*blockDim.x)*height + (h + radix)];
        buffor_g[(2*radix)*(2*radix + 1) + i] = G[(i + blockIdx.x*blockDim.x)*height + (h + radix)];
        buffor_b[(2*radix)*(2*radix + 1) + i] = B[(i + blockIdx.x*blockDim.x)*height + (h + radix)];
    }
    __syncthreads();

    if( blockDim.x*blockIdx.x + threadIdx.x + radix < width - 2*radix )
    {
      double current_distance;
      int g_n = 0;

      for(int y = 0 ; y <= 2*radix ; ++y)
      {
        for(int x = 0 ; x <= 2*radix; ++x)
        {
          current_distance=0;
          current_distance += pow((buffor_r[radix*(2*radix + 1) + threadIdx.x + radix] - buffor_r[y*(2*radix + 1) + threadIdx.x + x]), 2);
          current_distance += pow((buffor_g[radix*(2*radix + 1) + threadIdx.x + radix] - buffor_g[y*(2*radix + 1) + threadIdx.x + x]), 2);
          current_distance += pow((buffor_b[radix*(2*radix + 1) + threadIdx.x + radix] - buffor_b[y*(2*radix + 1) + threadIdx.x + x]), 2);

          if(current_distance < r_d)
            g_n++;
        }
      }
      if(g_n < 3)
        contaminated[(radix + threadIdx.x + blockIdx.x*blockDim.x)*height + h] = true;
      else
        contaminated[(radix + threadIdx.x + blockIdx.x*blockDim.x)*height + h] = false;
    }
    __syncthreads();
  }
}

void gpu_calcul(double r_d, int radix, double* r, double* g, double* b, int w, int h, bool*& noise_map){

  if(r!=nullptr)
  {
    double *d_r, *d_g, *d_b;
    bool* d_map;
    size_t img_size = w*h * sizeof(double);
    size_t noise_map_size = w*h * sizeof(bool);

    const int threadsPerBlock = 256;
    int blocksPerGrid = (w - 2*radix + threadsPerBlock - 1) / threadsPerBlock; 
    int sharedMemPerBlock = (2*radix + 1) * 3 * (threadsPerBlock + 2*radix) * sizeof(double);
    cudaError_t err;

    cudaMalloc((void**)&d_r, img_size);

    err = cudaGetLastError();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector d_r (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    cudaMalloc((void**)&d_g, img_size);

    err = cudaGetLastError();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector d_g (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    cudaMalloc((void**)&d_b, img_size);

    err = cudaGetLastError();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector d_b (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    cudaMalloc((void**)&d_map, noise_map_size);

    err = cudaGetLastError();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector d_map (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    cudaMemcpy(d_r, r, img_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_g, g, img_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, img_size, cudaMemcpyHostToDevice);

    calculate_distances<<<(blocksPerGrid, threadsPerBlock, sharedMemPerBlock >>>( r_d * r_d, radix, d_map, d_r, d_g, d_b, h, w);

    cudaDeviceSynchronize();

    err = cudaGetLastError();

    if ( err != cudaSuccess )
    {
      fprintf(stderr, "Error during kernel call (error code %s)!\n", cudaGetErrorString(err));
      exit(EXIT_FAILURE);
    }

    cudaMemcpy(noise_map, d_map, noise_map_size, cudaMemcpyDeviceToHost);

    cudaFree(d_map);
    cudaFree(d_r);
    cudaFree(d_g);
    cudaFree(d_b);
  }
}