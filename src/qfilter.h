#ifndef QFILTER_H
#define QFILTER_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <rgbimage.h>
#include <xyzimage.h>
#include <cie_labimage.h>

class QFilter
{
public:
    QFilter();
    QFilter(QImage&,unsigned short,bool**);
    QFilter(RGBImage&);
    QFilter(RGBImage&,bool**);

    ~QFilter();


    bool load_the_image(QImage&);
    virtual bool filter()=0;
    virtual bool detect_and_filter()=0;
    virtual bool detect()=0;

    bool set_map_shallow(bool**);

    double** get_R() const {return RGB.get_R();}
    double** get_G() const {return RGB.get_G();}
    double** get_B() const {return RGB.get_B();}

    int get_delay() const {return RGB.get_delay();}

    bool** get_noise_map_pointer() {return noise_map;}
    bool set_filtration_window_size(const unsigned short);

    RGBImage get_RGB() {return RGB;}

    double Accuracy(bool**);

protected:
    RGBImage RGB;
    bool** noise_map;
};

#endif // QFILTER_H
