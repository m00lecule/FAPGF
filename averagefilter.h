#ifndef AVERAGEFILTER_H
#define AVERAGEFILTER_H
#include "vmf.h"
#include "fapgf.h"

using namespace std;

class AverageFilter
{
public:
    AverageFilter();

    bool filterImage(FAPGF*,QImage&);
    bool filterRGBImage(FAPGF*);
    bool filterALL(FAPGF*,QImage&);
    bool filterALLRGBImage(FAPGF*);
};

#endif // AVERAGEFILTER_H
