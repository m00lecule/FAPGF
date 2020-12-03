#include "rgbimage.h"
#include <cmath>

RGBImage::RGBImage()
{
    R=nullptr;
    G=nullptr;
    B=nullptr;
    width=0;
    height=0;
    delay=0;
}

RGBImage::RGBImage(int h, int w, int d)
{
    width=w;
    height=h;
    delay=d;

    R=new double[width*height];
    G=new double[width*height];
    B=new double[width*height];


}

RGBImage::RGBImage(const QImage& image,int frame)
{
    delay=floor(frame/2);
    width=image.width()+2*delay;
    height=image.height()+2*delay;

    R=new double[width*height];
    G=new double[width*height];
    B=new double[width*height];


    QColor color;

    for(int i=delay;i<width-delay;++i)
        for(int j=delay;j<height-delay;++j)
        {
            color=image.pixelColor(i-delay,j-delay);

            R[i+j*width]=color.red();
            G[i+j*width]=color.green();
            B[i+j*width]=color.blue();
        }



    for (int i = 0; i<delay; ++i)
        for (int j = delay; j<height - delay; ++j)
        {
            R[i+j*width] = R[2 * delay - i + j*width];
            G[i+j*width] = G[2 * delay - i + j*width];
            B[i+j*width] = B[2 * delay - i + j*width];
        }


    for (int i =0; i<delay; ++i)
        for (int j = delay; j<height - delay; ++j)
        {
            R[width-delay+i+j*width] = R[width-delay-2-i+j*width];
            G[width-delay+i+j*width] = G[width-delay-2-i+j*width];
            B[width-delay+i+j*width] = B[width-delay-2-i+j*width];
        }


    for (int i = 0; i<width; ++i)
        for (int j = 0; j<delay; ++j)
        {
            R[i+j*width] = R[i+(2 * delay - j)*width];
            G[i+j*width] = G[i+(2 * delay - j)*width];
            B[i+j*width] = B[i+(2 * delay - j)*width];
         }

    for (int i = 0; i<width; ++i)
        for (int j = 0; j<delay; ++j)
        {
            R[i+(height-delay+j)*width] = R[i+(height - delay - j - 2)*width];
            G[i+(height-delay+j)*width] = G[i+(height - delay - j - 2)*width];
            B[i+(height-delay+j)*width] = B[i+(height - delay - j - 2)*width];
        }

    mse=0;
    psnr=0;
}

RGBImage RGBImage::operator=(RGBImage& image)
{
    double* pomocnicze_R=image.R;

    if(pomocnicze_R==nullptr)
    {
        if(R!=nullptr)
        {
            delete[] R;
            delete[] G;
            delete[] B;

            R=nullptr;
            G=nullptr;
            B=nullptr;
         }

        height=0;
        width=0;
        delay=0;

        return(*this);
    }
    else
    {
        int delay1=image.delay;
        int width1=image.width;
        int height1=image.height;
        int size=width1*height1;

            if(this->R!=nullptr)
            {
                delete[] this->R;
                delete[] this->G;
                delete[] this->B;
             }

            height=height1;
            width1=width1;
            delay=delay1;

            R=new double[width*height];
            G=new double[width*height];
            B=new double[width*height];

        double* pomocnicze_G=image.G;
        double* pomocnicze_B=image.B;

        memcpy(this->R,pomocnicze_R,size*sizeof(double));
        memcpy(this->G,pomocnicze_G,size*sizeof(double));
        memcpy(this->B,pomocnicze_B,size*sizeof(double));
    }
    return(*this);
}

RGBImage::RGBImage(RGBImage& image)
{
    double* R1=image.get_R();

    if(R1==nullptr)
    {
        R=nullptr;
        G=nullptr;
        B=nullptr;

        width=0;
        height=0;
        delay=0;

    }
    else
    {
        width=image.width;
        height=image.height;
        delay=image.delay;

        int size=width*height;

        double*G1=image.get_G();
        double*B1=image.get_B();

        R=new double[width*height];
        G=new double[width*height];
        B=new double[width*height];

        memcpy(R,R1,size*sizeof(double));
        memcpy(G,G1,size*sizeof(double));
        memcpy(B,B1,size*sizeof(double));
    }
}

bool RGBImage::operator ==(RGBImage &image)
{
   if(this->height!=image.height||this->width!=image.width||this->delay!=image.delay)
       return false;

   double* Rh=image.get_R();
   double* Gh=image.get_G();
   double* Bh=image.get_B();

   for(int i=0;i<this->width;++i)
   {
       for(int j=0;j<this->width;++j)
       {
           if(R[i+j*width]!=Rh[i+j*width])
               return false;
           if(G[i+j*width]!=Gh[i+j*width])
               return false;
           if(B[i+j*width]!=Bh[i+j*width])
               return false;
       }
   }
    return true;
}

RGBImage::~RGBImage()
{

    delete[] R;
    delete[] G;
    delete[] B;

}

double RGBImage::MSE(RGBImage & image)
{
   double* r = image.get_R();
   double* g = image.get_G();
   double* b = image.get_B();

   if(r==nullptr||R==nullptr)
   {
       return -1;
   }
   else
   {
       mse=0.0;

       for(int i=delay;i<width-delay;++i)
       {
           for(int j=delay;j<height-delay;++j)
           {
               //dodajemy po każdym kanale, tablica w ClearImage nie ma ramki, wiec w tej z AverageFilter trzeba uwzględnić przesunięcie

               mse+=(R[i+j*width]-r[i+j*width])*(R[i+j*width]-r[i+j*width]);

               mse+=(B[i+j*width]-b[i+j*width])*(B[i+j*width]-b[i+j*width]);

               mse+=(G[i+j*width]-g[i+j*width])*(G[i+j*width]-g[i+j*width]);
           }
       }

       mse*=(1/(3.0*((width-2*delay)*(height-2*delay))));
        return mse;
   }
}

double RGBImage::PSNR()
{
    psnr=0;

    if(mse!=0)
        psnr = 10.0*log10((255*255/mse));

     return psnr;
}



