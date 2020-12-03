#ifndef CIE_LABIMAGE_H
#define CIE_LABIMAGE_H
#include <xyzimage.h>

class CIE_labImage
{
public:
    CIE_labImage();
    CIE_labImage(XYZImage&);
    CIE_labImage(RGBImage&);
    CIE_labImage(CIE_labImage&);
    ~CIE_labImage();

    CIE_labImage& operator=(CIE_labImage&);
    CIE_labImage& operator=(XYZImage&);
    CIE_labImage& operator=(RGBImage&);

    double NCD(CIE_labImage&);

    double* get_a() const { return a; }
    double* get_b() const { return b; }
    double* get_l() const { return l; }

    int get_height() const {return height;}
    int get_width() const {return width;}
    int get_delay() const {return delay;}


private:
    double* a;
    double* b;
    double* l;

    int width;
    int height;
    int delay;
};

#endif // CIE_LABIMAGE_H
