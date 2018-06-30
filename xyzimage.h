#ifndef XYZIMAGE_H
#define XYZIMAGE_H
#include <rgbimage.h>

class XYZImage
{
public:
    XYZImage();
    XYZImage(RGBImage&);
    XYZImage(XYZImage&);
    ~XYZImage();

    XYZImage& operator=(RGBImage&);
    XYZImage& operator=(XYZImage&);

    double* get_X() const { return X;}
    double* get_Y() const { return Y;}
    double* get_Z() const { return Z;}

    int get_height() const {return height;}
    int get_width() const {return width;}
    int get_delay() const {return delay;}


private:
    double* X;
    double* Y;
    double* Z;
    int height;  //real height + 2*delay;
    int width;
    int delay;

};

#endif // XYZIMAGE_H
