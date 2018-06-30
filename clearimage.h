#ifndef CLEARIMAGE_H
#define CLEARIMAGE_H
#include "averagefilter.h"
#include <qimage.h>

class ClearImage : public QFilter
{
public:
    ClearImage();
    ClearImage(QImage&,unsigned short);

    double calculate_MSE(RGBImage);
    double calculate_PSNR(RGBImage);
   // double calculate_NCD(RGBImage&);

    bool detect() {return false;}
    bool filter() {return false;}
    bool detect_and_filter() {return false;}


};

#endif // CLEARIMAGE_H
