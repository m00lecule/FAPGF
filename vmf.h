#ifndef VMF_H
#define VMF_H

#include<rgbimage.h>

class VMF
{
public:
    VMF();
    VMF(int,int,RGBImage&,int);
    bool filter();
    unsigned get_red() const { return colors[0];}
    unsigned get_green() const { return colors[1];}
    unsigned get_blue() const { return colors[2];}

private:
    double* R;
    double* G;
    double* B;
    int delay;
    int width;
    int filtration_window_size;
    int coordinates[2];
    unsigned colors[3];
};

#endif // VMF_H
