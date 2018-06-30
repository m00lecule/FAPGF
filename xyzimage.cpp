#include "xyzimage.h"
#include <cmath>

XYZImage::XYZImage()
{
    X=nullptr;
    Y=nullptr;
    Z=nullptr;
    delay=0;
    height=0;
    width=0;
}

XYZImage::~XYZImage()
{
    if(X!=nullptr)
    {
        delete[] X;
        delete[] Y;
        delete[] Z;
    }
}

XYZImage::XYZImage(RGBImage & image)
{
    double* R=image.get_R();

    if(R==nullptr)
    {
       width=0;
       height=0;
       delay=0;

       X=nullptr;
       Y=nullptr;
       Z=nullptr;
    }
    else
    {
        this->delay=image.get_delay();
        this->width=image.get_width();
        this->height=image.get_height();

        this->X=new double[width*height];
        this->Y=new double[width*height];
        this->Z=new double[width*height];



        double* G=image.get_G();
        double* B=image.get_B();

        for(int i=0;i<width;++i)
            for(int j=0;j<height;++j)
            {
                double temp_R;
                double temp_G;
                double temp_B;

               temp_R=R[i+j*width]/255;
               temp_G=G[i+j*width]/255;
               temp_B=B[i+j*width]/255;

               if ( temp_R > 0.04045 )
                   temp_R = pow(( ( temp_R + 0.055 ) / 1.055 ), 2.4);
               else
                   temp_R = temp_R / 12.92;


               if ( temp_G > 0.04045 )
                   temp_G = pow(( ( temp_G + 0.055 ) / 1.055 ),2.4);
               else
                   temp_G = temp_G / 12.92;

               if ( temp_B > 0.04045 )
                   temp_B = pow(( ( temp_B + 0.055 ) / 1.055 ),2.4);
               else
                   temp_B = temp_B / 12.92;

                temp_R*=100;
                temp_G*=100;
                temp_B*=100;

                X[i+j*width] = temp_R * 0.4124 + temp_G * 0.3576 + temp_B * 0.1805;
                Y[i+j*width] = temp_R * 0.2126 + temp_G * 0.7152 + temp_B * 0.0722;
                Z[i+j*width] = temp_R * 0.0193 + temp_G * 0.1192 + temp_B * 0.9505;
            }
    }
}


XYZImage::XYZImage(XYZImage &image)
{
    double* X1=image.X;

    if(X1==nullptr)
    {
        X=nullptr;
        Y=nullptr;
        Z=nullptr;

        width=0;
        height=0;
        delay=0;
    }
    else
    {
        this->delay=image.delay;
        this->height=image.height;
        this->width=image.width;

        this->X=new double[width*height];
        this->Y=new double[width*height];
        this->Z=new double[width*height];

        int size=width*height;

        double* Y1=image.Y;
        double* Z1=image.Z;

        memcpy(X,X1,size*sizeof(double));
        memcpy(Y,Y1,size*sizeof(double));
        memcpy(Z,Z1,size*sizeof(double));
    }
}

XYZImage& XYZImage::operator=(XYZImage &image)
{
     double* X1=image.X;

     if(X1==nullptr)
     {
        if(X!=nullptr)
        {

            delete[] X;
            delete[] Y;
            delete[] Z;

            X=nullptr;
            Y=nullptr;
            Z=nullptr;
        }

        width=0;
        height=0;
        delay=0;

         return (*this);
     }
     else
     {
        int delay1=image.delay;
        int height1=image.height;
        int width1=image.width;

        int size=height1*width1;

        if(delay1!=delay || height1!=height || width1!=width)
        {
            if(X!=nullptr)
            {

                delete[] this->X;
                delete[] this->Y;
                delete[] this->Z;
            }

            this->delay=delay1;
            this->height=height1;
            this->width=width1;

            this->X=new double[width*height];
            this->Y=new double[width*height];
            this->Z=new double[width*height];


        }

        double* Y1=image.Y;
        double* Z1=image.Z;

        memcpy(X,X1,size*sizeof(double));
        memcpy(Y,Y1,size*sizeof(double));
        memcpy(Z,Z1,size*sizeof(double));


        return (*this);
     }
}


XYZImage& XYZImage::operator=(RGBImage &image)
{
    double* R = image.get_R();

    if(R==nullptr)
    {
        if(X!=nullptr)
        {
            delete[] X;
            delete[] Y;
            delete[] Z;

            X=nullptr;
            Y=nullptr;
            Z=nullptr;
        }

        width=0;
        height=0;
        delay=0;

         return (*this);
    }
    else
    {
        int delay1=image.get_delay();
        int height1=image.get_height();
        int width1=image.get_width();

        int size=height1*width1;


        if(delay1!=delay || height1!=height || width1!=width)
        {
             if(X!=nullptr)
             {

                delete[] this->X;
                delete[] this->Y;
                delete[] this->Z;
            }

            this->delay=delay1;
            this->height=height1;
            this->width=width1;

            this->X=new double[width*height];
            this->Y=new double[width*height];
            this->Z=new double[width*height];

        }

        double* G=image.get_G();
        double* B=image.get_B();

        for(int i=0;i<width;++i)
            for(int j=0;j<height;++j)
            {
                double temp_R;
                double temp_G;
                double temp_B;

                   temp_R=R[i+j*width]/255;
                   temp_G=G[i+j*width]/255;
                   temp_B=B[i+j*width]/255;


                   if ( temp_R > 0.04045 )
                       temp_R = pow(( ( temp_R + 0.055 ) / 1.055 ), 2.4);
                   else
                       temp_R = temp_R / 12.92;


                   if ( temp_G > 0.04045 )
                       temp_G = pow(( ( temp_G + 0.055 ) / 1.055 ),2.4);
                   else
                       temp_G = temp_G / 12.92;

                   if ( temp_B > 0.04045 )
                       temp_B = pow(( ( temp_B + 0.055 ) / 1.055 ),2.4);
                   else
                       temp_B = temp_B / 12.92;

                    temp_R*=100;
                    temp_G*=100;
                    temp_B*=100;

                    X[i+j*width] = temp_R * 0.4124 + temp_G * 0.3576 + temp_B * 0.1805;
                    Y[i+j*width] = temp_R * 0.2126 + temp_G * 0.7152 + temp_B * 0.0722;
                    Z[i+j*width] = temp_R * 0.0193 + temp_G * 0.1192 + temp_B * 0.9505;

            }

        return (*this);
    }
}
