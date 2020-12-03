#ifndef GAUSSNOISE_H
#define GAUSSNOISE_H

#include <QImage>
#include <random>
#include <QObject>
#include <ctime>
#include <rgbimage.h>

class GaussNoise
{    
public:
    GaussNoise();
    GaussNoise(double);
    bool Disturb(QImage &);

private:
    double range;
};

#endif // GAUSSNOISE_H
