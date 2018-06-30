#include "clearimage.h"
#include <cmath>

ClearImage::ClearImage() :QFilter()
{
}

ClearImage::ClearImage(QImage & image,unsigned short frame) :QFilter(image,frame,nullptr)
{
}


double ClearImage::calculate_MSE(RGBImage image)
{
    double** r = image.get_R();
    double** g = image.get_G();
    double** b = image.get_B();
    int height = image.get_height();
    int width = image.get_width();
    int delay = image.get_delay();

    double** R = RGB.get_R();
    double** G = RGB.get_G();
    double** B = RGB.get_B();

    if(r==nullptr)
    {
        return 0.0;
    }
    else
    {
        double mse=0.0;

        for(int i=delay;i<width-delay;++i)
        {
            for(int j=delay;j<height-delay;++j)
            {
                //dodajemy po każdym kanale, tablica w ClearImage nie ma ramki, wiec w tej z AverageFilter trzeba uwzględnić przesunięcie

                mse+=(R[i][j]-r[i][j])*(R[i][j]-r[i][j]);

                mse+=(B[i][j]-b[i][j])*(B[i][j]-b[i][j]);

                mse+=(G[i][j]-g[i][j])*(G[i][j]-g[i][j]);
            }
        }

        mse*=(1/(3.0*((width-2*delay)*(height-2*delay))));

         return mse;
    }
}


double ClearImage::calculate_PSNR(RGBImage image)
{
    double** r = image.get_R();

    if(r==nullptr)
    {
        return 0.0;
    }
    else
    {
        double mse=calculate_MSE(image);

        return 10.0*log10((255*255/mse));
    }
}

