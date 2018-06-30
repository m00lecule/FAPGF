#include "ctri.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <random>

CTRI::CTRI() : density(0), map_of_noise(nullptr)
{
}

CTRI::CTRI(double parameter) : map_of_noise(nullptr)
{
    if(parameter>=1&&parameter<=100)
        density=parameter/100;
    else
        density=0;
}

bool CTRI::Disturb(QImage & Image)
{
    if(map_of_noise!=nullptr)
    {
        delete [] map_of_noise;
    }

    int height=Image.height();
    int width=Image.width();

    int size=height*width;
    int how_many=round(density*size);

    map_of_noise=new bool [width*height];

    for(int i=0;i<width;++i)
        for(int j=0;j<height;++j)
            map_of_noise[i+j*width]=false;


   int*position= new int[size];

   for(int i=0;i<size;++i)
       *(position+i)=i;

    int index =0;

    std::mt19937 mt(time(0));
    std::uniform_int_distribution<int> cw(0,255);

    QColor color;

    for(int i=0;i<how_many;++i)
    {
        std::uniform_int_distribution<int> dist(0,size-i-1);

        index = dist(mt);

        int drawn= *(position+index);

        int x=drawn%width;
        int y=floor(drawn/width);

        color.setRed(cw(mt));
        color.setGreen(cw(mt));
        color.setBlue(cw(mt));

        map_of_noise[x+y*width]=true;

        Image.setPixelColor(x,y,color);
        *(position+index)=*(position+(size)-i-1);
    }

    delete [] position;

    return true;
}



