#ifndef RGBIMAGE_H
#define RGBIMAGE_H
#include <QImage>
#include <stdio.h>
class RGBImage
{
public:
    RGBImage();
    RGBImage(const QImage&,int);
    RGBImage(int,int,int);

    RGBImage(RGBImage&);
    ~RGBImage();

    QImage return_Image() const;
    RGBImage operator=(RGBImage&);
    bool operator==(RGBImage&);

    double PSNR();
    double MSE(RGBImage&);

    void flush() {psnr=0;mse=0;}

    double* get_R() const { return R;}
    double* get_G() const { return G;}
    double* get_B() const { return B;}

    int get_height() const {return height;}
    int get_width() const {return width;}
    int get_delay() const {return delay;}

private:
    double *R;
    double *G;
    double *B;

    double mse;
    double psnr;


    int height;  //real height + 2*delay;
    int width;
    int delay;
};

#endif // RGBIMAGE_H


