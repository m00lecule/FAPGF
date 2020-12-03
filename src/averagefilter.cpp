#include "averagefilter.h"
#include <cmath>

AverageFilter::AverageFilter()
{
}

bool AverageFilter::filterImage(FAPGF* fapgf,QImage & Image)
{

    RGBImage* RGB = fapgf->get_RGB();

    bool * noise_map = fapgf->get_noise_map();

    int height = RGB->get_height();
    int width=RGB->get_width();
    int delay=RGB->get_delay();

    double*R = RGB->get_R();
    double*G = RGB->get_G();
    double*B = RGB->get_B();

    if(R!=nullptr&&noise_map!=nullptr)
    {
        double average_r;
        double average_g;
        double average_b;
        int index;

        for(int i=delay;i<width-delay;++i)
        {
            for(int j=delay;j<height-delay;++j)
            {
                 average_b=0;
                 average_g=0;
                 average_r=0;
                 index=0;

                 QColor color;
                 if(noise_map[i+j*width])
                 {
                    for(int x=i-delay;x<i+delay+1;++x)
                        for(int y=j-delay;y<j+delay+1;++y)
                            {
                                if(!noise_map[x+y*width])
                                {
                                    average_b+=B[x+y*width];
                                    average_r+=R[x+y*width];
                                    average_g+=G[x+y*width];
                                    ++index;

                                }

                            }

                    if(index!=0)
                    {
                        R[i+j*width]=round(average_r/index);
                        G[i+j*width]=round(average_g/index);
                        B[i+j*width]=round(average_b/index);

                        color.setRed(round(average_r/index));
                        color.setGreen(round(average_g/index));
                        color.setBlue(round(average_b/index));

                    }
                    else
                    {
                        VMF vmf(i,j,*(RGB),2*delay+1);
                        vmf.filter();

                        R[i+j*width]=vmf.get_red();
                        G[i+j*width]=vmf.get_green();
                        B[i+j*width]=vmf.get_blue();


                        color.setRed(R[i+j*width]);
                        color.setGreen(G[i+j*width]);
                        color.setBlue(B[i+j*width]);
                    }
                    Image.setPixelColor(i-delay,j-delay,color);
                }
            }
        }

    }
   return true;
}

bool AverageFilter::filterALL(FAPGF *fapgf,QImage & Image)
{

    RGBImage* RGB=fapgf->get_RGB();

    double* R = RGB->get_R();
    double* G = RGB->get_G();
    double* B = RGB->get_B();

    int height = RGB->get_height();
    int width = RGB->get_width();
    int delay = RGB->get_delay();
    int filtration_window_size=2*delay+1;

    if(fapgf!=nullptr)
    {

                double average_r;
                double average_g;
                double average_b;
                int index;

               bool* temporary_pointer=fapgf->get_aux_b_table(); //bez ramki odczytu

                for(int i=delay;i<width-delay;++i)
                {
                    for(int j=delay;j<height-delay;++j)
                    {
                         average_b=0;
                         average_g=0;
                         average_r=0;
                         index=0;

                         QColor color;

                         for(int x=0;x<filtration_window_size;++x)
                             for(int y=0;y<filtration_window_size;++y)
                                    {
                                        if(temporary_pointer[(i-delay)*(height-2*delay)*filtration_window_size*filtration_window_size+(j-delay)*filtration_window_size*filtration_window_size+x*filtration_window_size+y]==true)
                                        {
                                            average_b+=B[i-delay+x+(j-delay+y)*width];
                                            average_r+=R[i-delay+x+(j-delay+y)*width];
                                            average_g+=G[i-delay+x+(j-delay+y)*width];
                                            ++index;
                                        }

                                    }

                            if(index!=0)
                            {
                                R[i+j*width]=round(average_r/index);
                                G[i+j*width]=round(average_g/index);
                                B[i+j*width]=round(average_b/index);

                                color.setRed(round(average_r/index));
                                color.setGreen(round(average_g/index));
                                color.setBlue(round(average_b/index));

                            }
                            else
                            {
                                VMF vmf(i,j,*(RGB),filtration_window_size);
                                vmf.filter();

                                R[i+j*width]=vmf.get_red();
                                G[i+j*width]=vmf.get_green();
                                B[i+j*width]=vmf.get_blue();


                                color.setRed(R[i+j*width]);
                                color.setGreen(G[i+j*width]);
                                color.setBlue(B[i+j*width]);
                            }

                            Image.setPixelColor(i-delay,j-delay,color);
                     }
                }
                return true;

    }
    return false;
}


bool AverageFilter::filterRGBImage(FAPGF *fapgf)
{
    RGBImage* RGB = fapgf->get_RGB();

    bool* noise_map = fapgf->get_noise_map();

    int height = RGB->get_height();
    int width=RGB->get_width();
    int delay=RGB->get_delay();

    double* R = RGB->get_R();
    double* G = RGB->get_G();
    double* B = RGB->get_B();

    if(R!=nullptr&&noise_map!=nullptr)
    {
        double average_r;
        double average_g;
        double average_b;
        int index;

        for(int i=delay;i<width-delay;++i)
        {
            for(int j=delay;j<height-delay;++j)
            {
                 average_b=0;
                 average_g=0;
                 average_r=0;
                 index=0;

                 if(noise_map[i+j*width])
                 {
                    for(int x=i-delay;x<i+delay+1;++x)
                        for(int y=j-delay;y<j+delay+1;++y)
                            {
                                if(!noise_map[x+y*width])
                                {
                                    average_b+=B[x+y*width];
                                    average_r+=R[x+y*width];
                                    average_g+=G[x+y*width];
                                    ++index;

                                }

                            }

                    if(index!=0)
                    {
                        R[i+j*width]=round(average_r/index);
                        G[i+j*width]=round(average_g/index);
                        B[i+j*width]=round(average_b/index);

                    }
                    else
                    {
                        VMF vmf(i,j,*(RGB),2*delay+1);
                        vmf.filter();

                        R[i+j*width]=vmf.get_red();
                        G[i+j*width]=vmf.get_green();
                        B[i+j*width]=vmf.get_blue();

                    }
                }
            }
        }

    }
   return true;
}


bool AverageFilter::filterALLRGBImage(FAPGF*fapgf)
{
    RGBImage* RGB=fapgf->get_RGB();

    double* R = RGB->get_R();
    double* G = RGB->get_G();
    double* B = RGB->get_B();

    int height = RGB->get_height();
    int width = RGB->get_width();
    int delay = RGB->get_delay();
    int filtration_window_size=2*delay+1;

    if(fapgf!=nullptr)
    {

                double average_r;
                double average_g;
                double average_b;
                int index;

                bool* temporary_pointer=fapgf->get_aux_b_table(); //bez ramki odczytu

                for(int i=delay;i<width-delay;++i)
                {
                    for(int j=delay;j<height-delay;++j)
                    {
                         average_b=0;
                         average_g=0;
                         average_r=0;
                         index=0;

                         for(int x=0;x<filtration_window_size;++x)
                             for(int y=0;y<filtration_window_size;++y)
                                    {
                                        if(temporary_pointer[(i-delay)*(height-2*delay)*filtration_window_size*filtration_window_size+(j-delay)*filtration_window_size*filtration_window_size+x*filtration_window_size+y]==true)
                                        {
                                            average_b+=B[i-delay+x+(j-delay+y)*width];
                                            average_r+=R[i-delay+x+(j-delay+y)*width];
                                            average_g+=G[i-delay+x+(j-delay+y)*width];
                                            ++index;
                                        }

                                    }

                            if(index!=0)
                            {
                                R[i+j*width]=round(average_r/index);
                                G[i+j*width]=round(average_g/index);
                                B[i+j*width]=round(average_b/index);
                            }
                            else
                            {
                                VMF vmf(i,j,*(RGB),filtration_window_size);
                                vmf.filter();

                                R[i+j*width]=vmf.get_red();
                                G[i+j*width]=vmf.get_green();
                                B[i+j*width]=vmf.get_blue();

                            }
                     }
                }
                return true;


}        return false;


    return true;
}




