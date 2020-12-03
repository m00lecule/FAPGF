#include "gaussnoise.h"

GaussNoise::GaussNoise()
{
    range=0;
}

GaussNoise::GaussNoise(double Range)
{
    range=Range/3.0;
}

bool GaussNoise::Disturb(QImage & Image)
{
    int width=Image.width();
    int height=Image.height();

    std::mt19937 mt(time(0));
    std::uniform_real_distribution<double> urd(0,1);

    double deviation_r;
    double deviation_g;
    double deviation_b;

    QColor color;

    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            deviation_r=0;
            deviation_g=0;
            deviation_b=0;

            for(int z=0;z<12;++z)
            {
                deviation_r+=urd(mt);
                deviation_g+=urd(mt);
                deviation_b+=urd(mt);
            }

            deviation_r-=6;
            deviation_g-=6;
            deviation_b-=6;

            deviation_r*=range;
            deviation_g*=range;
            deviation_b*=range;

            color=Image.pixelColor(i,j);

            double red =color.red()+deviation_r;
            double green =color.green()+deviation_g;
            double blue=color.blue()+deviation_b;

            if(red<0||red>255)
            {
                if(red>255)
                    red=255;
                else
                    red=0;
            }

            if(green<0||green>255)
            {
                if(green>255)
                    green=255;
                else
                    green=0;
            }

            if(blue<0||blue>255)
            {
                if(blue>255)
                    blue=255;
                else
                    blue=0;
            }


            color.setRed(red);
            color.setGreen(green);
            color.setBlue(blue);

            Image.setPixelColor(i,j,color);
        }
    }
    return true;
}
