#ifndef FAPGFOPT_H
#define FAPGFOPT_H
#include <fapgf.h>
#include <averagefilter.h>
#include <QImage>
#include <QTextStream>
#include <QFile>
#include <random>
#include <ctime>
#include <qglobal.h>

class FAPGFOpt
{
public:
    FAPGFOpt();
    FAPGFOpt(RGBImage*,RGBImage*,int,int,int);
    double optimize();
    double* optimize_2();

private:
    int number_of_elicit;
    int number_of_children;
    int mutation;
    FAPGF* fapgf;
    RGBImage* clear;
    RGBImage* disturbed;

};

#endif // FAPGFOPT_H
