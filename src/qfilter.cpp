#include "qfilter.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVariant>
#include <QDebug>

QFilter::QFilter()
{
    noise_map=nullptr;
}

QFilter::QFilter(QImage & Image,unsigned short size_of_filtering_window,bool**map):noise_map(map),RGB(Image,size_of_filtering_window)
{
}

QFilter::QFilter(RGBImage& image):RGB(image),noise_map(nullptr)
{
}

QFilter::QFilter(RGBImage& image,bool **noise)
{
    RGB=image;

    int height = RGB.get_height();
    int width = RGB.get_width();

    noise_map = new bool*[width];
    noise_map[0] = new bool[width*height];

    for(int i=1;i<width;++i)
        noise_map[i]=&noise_map[0][i*height];

    for(int i=0;i<width;++i)
        for(int j=0;j<height;++j)
            noise_map[i][j]=noise[i][j];
}


QFilter::~QFilter()
{
    if(noise_map!=nullptr)
    {
            delete [] noise_map[0];
            delete [] noise_map;
    }

}

bool QFilter::set_map_shallow(bool** pointer)
{
   if(pointer!=nullptr)
   {
        if(noise_map!=nullptr)
        {
            delete[] noise_map[0];
            delete[] noise_map;
        }

        noise_map=pointer;
        return true;
   }
   else
   {}
       return false;
}



