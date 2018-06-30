#include "vmf.h"
#include <cmath>

VMF::VMF()
{
    R=nullptr;
    G=nullptr;
    B=nullptr;
    width=0;
    delay=0;
    filtration_window_size=0;
}

VMF::VMF(int x, int y, RGBImage& image, int frame)
{
    R=image.get_R();
    B=image.get_B();
    G=image.get_G();

    filtration_window_size=frame;
    delay=floor(filtration_window_size/2);
    width=image.get_width();
    coordinates[0]=x;
    coordinates[1]=y;
}

bool VMF::filter()
{
    if(R==nullptr)
    {
        return false;
    }
    else
    {

        double minimum =0;
        double temp=0;

        int first_x=coordinates[0]-delay;
        int first_y=coordinates[1]-delay;

        colors[0]=R[first_x+(first_y)*width];
        colors[1]=G[first_x+(first_y)*width];
        colors[2]=B[first_x+(first_y)*width];

        for(int i1= first_x;i1<first_x+filtration_window_size;++i1)
        {
            for(int j1=first_y;j1<first_y+filtration_window_size;++j1)
            {
                temp=0;

                for(int i2= first_x;i2<first_x+filtration_window_size;++i2)
                {
                    for(int j2=first_y;j2<first_y+filtration_window_size;++j2)
                    {

                        if(i1==coordinates[0]-delay&&j1==coordinates[1]-delay)
                            minimum+=(abs(R[i1+j1*width]-R[i2+j2*width])+abs(B[i1+j1*width]-B[i2+j2*width])+abs(G[i1+j1*width]-G[i2+j2*width]));
                        else
                            temp+=abs(R[i1+j1*width]-R[i2+j2*width])+abs(B[i1+j1*width]-B[i2+j2*width])+abs(G[i1+j1*width]-G[i2+j2*width]);
                    }
                }

                if((i1!=coordinates[0]-delay&&j1!=coordinates[1]-delay)&&minimum>temp)
                {
                    minimum=temp;
                    colors[0]=R[i1+j1*width];
                    colors[1]=G[i1+j1*width];
                    colors[2]=B[i1+j1*width];
                }

            }
        }
        return true;
    }
}
