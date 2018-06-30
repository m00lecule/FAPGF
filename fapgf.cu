#include "cuda.h"
#include <stdio.h>


__global__
void calculate_distances(double r_d, int radix,bool* contaminated,double* R,double* G,double* B,int height,int width){
/*
  __shared__ double buffor_r[5][256+4];
  __shared__ double buffor_g[5][256+4];
  __shared__ double buffor_b[5][256+4];

  int len=256;

  for(int k=0 ; k<=2*radix ; ++k)
    for(int i=threadIdx.x ; i<len+2*radix ; i+=blockDim.x){
      buffor_r[k][i] = 0;
      buffor_g[k][i] = 0;
      buffor_b[k][i] = 0;
    }

  __syncthreads();

  for(int j=0 ; j<=2*radix ; ++j)
    for(int i=threadIdx.x ; i<len+2*radix ; i+=blockDim.x)
      if(i+blockDim.x*blockIdx.x<width){
        buffor_r[j][i] = R[j*width+(blockIdx.x*blockDim.x)+i];
        buffor_g[j][i] = G[j*width+(blockIdx.x*blockDim.x)+i];
        buffor_b[j][i] = B[j*width+(blockIdx.x*blockDim.x)+i];
      }

  __syncthreads();

//wysokosc zawiera ramke
for(int j=radix ; j<height-radix ; ++j){

      for(int y=2*radix ; y>=1 ; --y)
        for(int i=threadIdx.x ; i<len+2*radix ; i+=blockDim.x)
          if(i+blockDim.x*blockIdx.x<width){
            buffor_r[y][i] = buffor_r[y-1][i];
            buffor_g[y][i] = buffor_g[y-1][i];
            buffor_b[y][i] = buffor_b[y-1][i];
          }
      __syncthreads();

      for(int i=threadIdx.x ; i<len+2*radix ; i+=blockDim.x)
        if(i+blockDim.x*blockIdx.x<width){
          buffor_r[0][i] = R[(j+radix)*width+(blockIdx.x*blockDim.x)+i];
          buffor_g[0][i] = G[(j+radix)*width+(blockIdx.x*blockDim.x)+i];
          buffor_b[0][i] = B[(j+radix)*width+(blockIdx.x*blockDim.x)+i];
        }

    __syncthreads();

    if(blockDim.x*blockIdx.x+threadIdx.x+radix<width-2*radix){
      double current_distance;
      int g_n=0;

      for(int y=0 ; y<=2*2 ; y++){
        for(int x=0 ; x<=2*2 ; x++){
          current_distance=0;
          current_distance += (buffor_r[radix][threadIdx.x+radix]-buffor_r[y][threadIdx.x+x])*(buffor_r[radix][threadIdx.x+radix]-buffor_r[y][threadIdx.x+x]);
          current_distance += (buffor_g[radix][threadIdx.x+radix]-buffor_g[y][threadIdx.x+x])*(buffor_g[radix][threadIdx.x+radix]-buffor_g[y][threadIdx.x+x]);
          current_distance += (buffor_b[radix][threadIdx.x+radix]-buffor_b[y][threadIdx.x+x])*(buffor_b[radix][threadIdx.x+radix]-buffor_b[y][threadIdx.x+x]);

          current_distance=sqrt(current_distance);
          //counter nie posiada bufforu zzew
          if(current_distance<r_d)
            g_n++;
        }
      }

      if(g_n<3)
        contaminated[j*width+(blockDim.x*blockIdx.x)+threadIdx.x+radix]=true;
      else
        contaminated[j*width+(blockDim.x*blockIdx.x)+threadIdx.x+radix]=false;
    }
    __syncthreads();
  }

  */
    int counter=0;
    if(blockIdx.x*blockDim.x+threadIdx.x<width-radix&&blockIdx.x*blockDim.x+threadIdx.x>=radix&&blockIdx.y*blockDim.y+threadIdx.y>=radix&&blockIdx.y*blockDim.y+threadIdx.y<height-radix)
    {
        double dist;
        counter=0;
        for(int i=-radix;i<=radix;++i){
            for(int j=-radix;j<=radix;++j){
                dist=sqrt((R[blockIdx.x*blockDim.x+threadIdx.x+width*blockIdx.y*blockDim.y+threadIdx.y]-R[blockIdx.x*blockDim.x+threadIdx.x+i+width*(blockIdx.y*blockDim.y+threadIdx.y+j)])*(R[blockIdx.x*blockDim.x+threadIdx.x+width*blockIdx.y*blockDim.y+threadIdx.y]-R[blockIdx.x*blockDim.x+threadIdx.x+i+width*(blockIdx.y*blockDim.y+threadIdx.y+j)])+(G[blockIdx.x*blockDim.x+threadIdx.x+width*blockIdx.y*blockDim.y+threadIdx.y]-G[blockIdx.x*blockDim.x+threadIdx.x+i+width*(blockIdx.y*blockDim.y+threadIdx.y+j)])*(G[blockIdx.x*blockDim.x+threadIdx.x+width*blockIdx.y*blockDim.y+threadIdx.y]-G[blockIdx.x*blockDim.x+threadIdx.x+i+width*(blockIdx.y*blockDim.y+threadIdx.y+j)])+(B[blockIdx.x*blockDim.x+threadIdx.x+width*blockIdx.y*blockDim.y+threadIdx.y]-B[blockIdx.x*blockDim.x+threadIdx.x+i+width*(blockIdx.y*blockDim.y+threadIdx.y+j)])*(B[blockIdx.x*blockDim.x+threadIdx.x+width*blockIdx.y*blockDim.y+threadIdx.y]-B[blockIdx.x*blockDim.x+threadIdx.x+i+width*(blockIdx.y*blockDim.y+threadIdx.y+j)]));

                if(dist<r_d)
                counter++;
            }
        }

        if(counter<3)
            contaminated[blockIdx.x*blockDim.x+threadIdx.x+width*blockIdx.y*blockDim.y+threadIdx.y]=true;
         else
            contaminated[blockIdx.x*blockDim.x+threadIdx.x+width*blockIdx.y*blockDim.y+threadIdx.y]=false;

    }
}

void gpu_calcul(double r_d,int delay, double* r,double* g, double* b, int w, int h, bool*& noise_map){

  if(r!=nullptr){
    double *p_r, *p_g,*p_b;
    bool* noise_map_cuda;

    //tylko do których zapisujemy osobno 16 streamów moze byc
    //cuda callback
  //  cuda_stream_t stream;

    //cudaStreamCreate(&stream);
    cudaMalloc((void**)&p_r,sizeof(double)*w*h);
    cudaMalloc((void**)&p_g,sizeof(double)*w*h);
    cudaMalloc((void**)&p_b,sizeof(double)*w*h);
    cudaMalloc((void**)&noise_map_cuda,sizeof(bool)*w*h);

    //asynchronicznie
    cudaMemcpy(p_r,r,sizeof(double)*w*h,cudaMemcpyHostToDevice);
    cudaMemcpy(p_g,g,sizeof(double)*w*h,cudaMemcpyHostToDevice);
    cudaMemcpy(p_b,b,sizeof(double)*w*h,cudaMemcpyHostToDevice);
    //asynchroicznie

    //calculate_distances<<<((w-2*delay+255)/256),256>>>( r_d , 2 , noise_map_cuda , p_r , p_g, p_b , h , w );

    dim3 Grid((w+7)/8,(h+7)/8);
    dim3 Block(8,8);
    calculate_distances<<<Grid,Block>>>( r_d , 2 , noise_map_cuda , p_r , p_g, p_b , h , w );

    cudaDeviceSynchronize();

    cudaMemcpy(noise_map,noise_map_cuda,sizeof(bool)*w*h,cudaMemcpyDeviceToHost);

    cudaFree(noise_map_cuda);
    cudaFree(p_r);
    cudaFree(p_g);
    cudaFree(p_b);

  }
}

__global__ void set(float* tab,float* tab2,float* tab3, int w,int h){
    if(threadIdx.x<w*h)
        tab3[threadIdx.x]=tab2[threadIdx.x]+tab[threadIdx.x];
}

void set_zeros(float* tab,float* tab2, float* tab3, int w, int h)
{
    float* ptr1, *ptr2, *ptr3;
    int dev;
    cudaGetDeviceCount(&dev);
    tab3[6]=dev;
    cudaMalloc((void**)&ptr1,sizeof(float)*w*h);
    tab3[0]=(int)cudaGetLastError();
    cudaMalloc((void**)&ptr2,sizeof(float)*w*h);
    tab3[1]=(int)cudaGetLastError();
    cudaMalloc((void**)&ptr3,sizeof(float)*w*h);
    tab3[2]=(int)cudaGetLastError();
    cudaMemcpy((void*)ptr1,(void*)tab,sizeof(float)*w*h,cudaMemcpyHostToDevice);
    tab3[3]=(int)cudaGetLastError();
    cudaMemcpy((void*)ptr2,(void*)tab2,sizeof(float)*w*h,cudaMemcpyHostToDevice);
    tab3[4]=(int)cudaGetLastError();
    cudaMemset((void*)ptr3, 0, sizeof(float)*w*h);
    tab3[5]=(int)cudaGetLastError();
    set<<<w*h,w*h>>>(ptr1,ptr2,ptr3,w,h);

    cudaDeviceSynchronize();
    cudaMemcpy((void*)tab3 , (void*)ptr3 , w*h*sizeof(float) , cudaMemcpyDeviceToHost);

    //for(int i=0;i<w*h;++i)
    //    tab3[i]=ptr3[i];

    cudaFree(ptr1);
    cudaFree(ptr2);
    cudaFree(ptr3);
}


