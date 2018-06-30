#include "cie_labimage.h"
#include <cmath>

CIE_labImage::CIE_labImage()
{
    a=nullptr;
    b=nullptr;
    l=nullptr;
    width=0;
    height=0;
    delay=0;
}

CIE_labImage::~CIE_labImage()
{
    if(a!=nullptr)
    {
        delete[] a;
        delete[] b;
        delete[] l;
    }
}


CIE_labImage::CIE_labImage(CIE_labImage &image)
{
    double* a1=image.a;

    if(a1==nullptr)
    {
        a=nullptr;
        b=nullptr;
        l=nullptr;
        height=0;
        width=0;
        delay=0;
    }
    else
    {
        width=image.width;
        height=image.height;
        delay=image.delay;

        int size=width*height;

        a=new double[width*height];
        b=new double[width*height];
        l=new double[width*height];


        double* b1=image.b;
        double* l1=image.l;

        memcpy(a,a1,size*sizeof(double));
        memcpy(b,b1,size*sizeof(double));
        memcpy(l,l1,size*sizeof(double));
    }
}

CIE_labImage& CIE_labImage::operator =(CIE_labImage& image)
{
      double* a1=image.a;

      if(a1==nullptr)
      {
        if(this->a!=nullptr)
        {

            delete[] a;
            delete[] b;
            delete[] l;

            a=nullptr;
            b=nullptr;
            l=nullptr;
        }

        width=0;
        height=0;
        delay=0;

        return(*this);
      }
      else
      {

         int width1=image.width;
         int height1=image.height;
         int delay1=image.delay;
         int size=width1*height1;

         if(width1!=width || height1!= height || delay1!=delay)
         {
             if(this->a!=nullptr)
             {
                 delete[] a;
                 delete[] b;
                 delete[] l;
             }

             width=width1;
             height=height1;
             delay=delay1;

             a=new double[width*height];
             b=new double[width*height];
             l=new double[width*height];
         }

        double* b1=image.b;
        double* l1=image.l;

        memcpy(a,a1,size*sizeof(double));
        memcpy(b,b1,size*sizeof(double));
        memcpy(l,l1,size*sizeof(double));

        return(*this);
    }
}


CIE_labImage& CIE_labImage::operator =(XYZImage & image)
{
    double* X = image.get_X();

    if(X==nullptr)
    {
        if(this->a!=nullptr)
        {
            delete[] a;
            delete[] b;
            delete[] l;

            a=nullptr;
            b=nullptr;
            l=nullptr;
        }

        width=0;
        height=0;
        delay=0;

        return(*this);
    }
    else
    {

        int height1=image.get_height();
        int width1=image.get_width();
        int delay1=image.get_delay();

        if(height1!=height || width1 != width || delay1 != delay)
        {
            if(a!=nullptr)
            {
                delete[] a;
                delete[] b;
                delete[] l;
            }

            height=height1;
            width=width1;
            delay=delay1;

            a=new double[width*height];
            b=new double[width*height];
            l=new double[width*height];
        }

        double* Y = image.get_Y();
        double* Z = image.get_Z();

        for(int i=0;i<width;++i)
        {
            for(int j=0;j<height;++j)
            {
                double temp_X=X[i+j*width]/94.81;
                double temp_Y=Y[i+j*width]/100.0;
                double temp_Z=Z[i+j*width]/107.3;

                if(temp_X>0.008856)
                    temp_X = pow(temp_X,1/3);
                else
                    temp_X = (7.787*temp_X)+(16/116);

                if(temp_Y>0.008856)
                    temp_Y = pow(temp_Y,1/3);
                else
                    temp_Y = (7.787*temp_Y)+(16/116);

                if(temp_Z > 0.008856)
                    temp_Z = pow(temp_Z,1/3);
                else
                    temp_Z = (7.787*temp_Z)+(16/116);

                  l[i+j*width] = (116*temp_Y)-16;
                  a[i+j*width] = 500*(temp_X-temp_Y);
                  b[i+j*width] = 200*(temp_Y-temp_Z);
            }
        }

       return(*this);
    }
}

CIE_labImage::CIE_labImage(XYZImage& image)
{
    double* X = image.get_X();

    if(X==nullptr)
    {
        a=nullptr;
        b=nullptr;
        l=nullptr;

        height=0;
        width=0;
        delay=0;
    }
    else
    {

        width=image.get_width();
        height=image.get_height();
        delay=image.get_delay();

        a=new double[width*height];
        b=new double[width*height];
        l=new double[width*height];


        double* Y = image.get_Y();
        double* Z = image.get_Z();

        for(int i=0;i<width;++i)
        {
            for(int j=0;j<height;++j)
            {
                double temp_X=X[i+j*width]/94.81;
                double temp_Y=Y[i+j*width]/100.0;
                double temp_Z=Z[i+j*width]/107.3;

                if(temp_X>0.008856)
                    temp_X = pow(temp_X,1/3);
                else
                    temp_X = (7.787*temp_X)+(16/116);

                if(temp_Y>0.008856)
                    temp_Y = pow(temp_Y,1/3);
                else
                    temp_Y = (7.787*temp_Y)+(16/116);

                if(temp_Z > 0.008856)
                    temp_Z = pow(temp_Z,1/3);
                else
                    temp_Z = (7.787*temp_Z)+(16/116);

                  l[i+j*width] = (116*temp_Y)-16;
                  a[i+j*width] = 500*(temp_X-temp_Y);
                  b[i+j*width] = 200*(temp_Y-temp_Z);
            }
        }
    }
}

CIE_labImage::CIE_labImage(RGBImage & image)
{
    //XYZImage img1=image;

    XYZImage img1;
    double* X = img1.get_X();

    if(X==nullptr)
    {
        a=nullptr;
        b=nullptr;
        l=nullptr;

        height=0;
        delay=0;
        width=0;
    }
    else
    {
        this->height=image.get_height();
        this->width=image.get_width();
        this->delay=image.get_delay();

        a=new double[width*height];
        b=new double[width*height];
        l=new double[width*height];

        double* Y = img1.get_Y();
        double* Z = img1.get_Z();

        for(int i=0;i<width;++i)
        {
            for(int j=0;j<height;++j)
            {
                double temp_X=X[i+j*width]/94.81;
                double temp_Y=Y[i+j*width]/100.0;
                double temp_Z=Z[i+j*width]/107.3;

                if(temp_X>0.008856)
                    temp_X = pow(temp_X,1/3);
                else
                    temp_X = (7.787*temp_X)+(16/116);

                if(temp_Y>0.008856)
                    temp_Y = pow(temp_Y,1/3);
                else
                    temp_Y = (7.787*temp_Y)+(16/116);

                if(temp_Z > 0.008856)
                    temp_Z = pow(temp_Z,1/3);
                else
                    temp_Z = (7.787*temp_Z)+(16/116);

                  l[i+j*width] = (116*temp_Y)-16;
                  a[i+j*width] = 500*(temp_X-temp_Y);
                  b[i+j*width] = 200*(temp_Y-temp_Z);
            }
        }
    }
}

double CIE_labImage::NCD(CIE_labImage & image)
{
    int width=this->get_width();
    int height=this->get_height();
    int delay=this->get_delay();

    double* CIE_a=image.get_a();
    double* CIE_b=image.get_b();
    double* CIE_l=image.get_l();

    double sum_of_difference=0;
    double sum=0;

    for(int i=delay;i<width-delay;++i)
    {
        for(int j=delay;j<height-delay;++j)
        {
            sum_of_difference+=sqrt(pow(l[i+j*width]-CIE_l[i+j*width],2)+pow(a[i+j*width]-CIE_a[i+j*width],2)+pow(b[i+j*width]-CIE_b[i+j*width],2));

            sum+=CIE_l[i+j*width]+CIE_a[i+j*width]+CIE_b[i+j*width];
        }

    }
    return (sum_of_difference*1.0/sum);
}
