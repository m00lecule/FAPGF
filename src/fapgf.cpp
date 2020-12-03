#include "fapgf.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <cmath>

FAPGF::FAPGF()
{
    Type_of_noise = Noise_Type::CTRI;
    d_parameter=0;
    aux_tab_is_calculated=false;
    noise_map=nullptr;
    aux_b_table=nullptr;
    aux_table=nullptr;

    measure_type_unit=Distance::Euklidesaowa;
}

FAPGF::FAPGF(RGBImage & image,double par,Distance dist, Noise_Type nt)
{
    aux_tab_is_calculated=false;
    noise_map=nullptr;
    aux_b_table=nullptr;
    aux_table=nullptr;


    Type_of_noise=nt;
    measure_type_unit=dist;
    RGBptr=&image;


    if(measure_type_unit==Distance::Euklidesaowa)
    {
        d_parameter=par*255*1.73;
    }
    else
    {
        d_parameter=par*255;
    }
}


FAPGF::~FAPGF()
{
    if(aux_b_table!=nullptr)
        delete[] aux_b_table;
    if(aux_table!=nullptr)
        delete[] aux_table;
}


bool FAPGF::set_d_parameter(float parameter)
{
    if(noise_map!=nullptr)
    {
            delete [] noise_map;
            noise_map=nullptr;
    }

    if(measure_type_unit==Distance::Euklidesaowa)
    {
        d_parameter=parameter*255*1.73;
    }
    else
    {
        d_parameter=parameter*255;
    }
        return true;

}

bool FAPGF::detect()
{
    double*R = RGBptr->get_R();

    int delay = RGBptr->get_delay();
    int width = RGBptr->get_width();
    int height = RGBptr->get_height();

    int filtration_window_size = 2*delay+1;

    if(R!=nullptr)
      {
          if(Type_of_noise==Noise_Type::CTRI)
          {
              if(!aux_tab_is_calculated)
                  calculate_distances();

              int dobry_sasiad=0;

              if(noise_map!=nullptr)
              {
                      delete [] noise_map;
              }

              noise_map=new bool[width*height];


                    for(int i=delay;i<width-delay;++i)
                    {
                        for(int j=delay;j<height-delay;++j)
                        {
                            for(int z=0;z<filtration_window_size;++z)
                            {
                                for(int y=0;y<filtration_window_size;++y)
                                {
                                    if(aux_table[(i-delay)*(height-2*delay)*filtration_window_size*filtration_window_size+(j-delay)*filtration_window_size*filtration_window_size+z*filtration_window_size+y]<d_parameter)
                                        ++dobry_sasiad;
                                }
                            }

                            if(dobry_sasiad<3)
                                noise_map[i+j*width]=true;
                            else
                               noise_map[i+j*width]=false;

                             dobry_sasiad=0;
                         }
                    }


                for (int i = 0; i<delay; ++i)
                    for (int j = delay; j<height - delay; ++j)
                        noise_map[i+j*width] = noise_map[2 * delay - i + j*width];

                for (int i =0; i<delay; ++i)
                    for (int j = delay; j<height - delay; ++j)
                        noise_map[width-delay+i+j*width] = noise_map[width -delay-2-i+j*width];

                for (int i = 0; i<width; ++i)
                    for (int j = 0; j<delay; ++j)
                        noise_map[i+j*width] = noise_map[i+(2 * delay - j)*width];

                for (int i = 0; i<width; ++i)
                    for (int j = 0; j<delay; ++j)
                        noise_map[i+(height-delay+j)*width] = noise_map[i+(height - delay - j - 2)*width];
                return true;
        }
        else
        {

              if(!aux_tab_is_calculated)
                            calculate_distances();

                        int dobry_sasiad=0;

                                aux_b_table= new bool[(width-2*delay)*(height-2*delay)*filtration_window_size*filtration_window_size];

                                for(int i=0;i<width-2*delay;++i)
                                {
                                    for(int j=0;j<height-2*delay;++j)
                                    {
                                      for(int z=0;z<filtration_window_size;++z)
                                      {
                                        for(int y=0;y<filtration_window_size;++y)
                                        {
                                            if(y==delay&&z==delay)
                                            {
                                                aux_b_table[i*(height-2*delay)*filtration_window_size*filtration_window_size+j*filtration_window_size*filtration_window_size+z*filtration_window_size+y]=false;
                                                continue;
                                            }

                                             if(aux_table[i*(height-2*delay)*filtration_window_size*filtration_window_size+j*filtration_window_size*filtration_window_size+z*filtration_window_size+y]<d_parameter)
                                             {
                                                ++dobry_sasiad;
                                                aux_b_table[i*(height-2*delay)*filtration_window_size*filtration_window_size+j*filtration_window_size*filtration_window_size+z*filtration_window_size+y]=true;
                                             }
                                             else
                                                aux_b_table[i*(height-2*delay)*filtration_window_size*filtration_window_size+j*filtration_window_size*filtration_window_size+z*filtration_window_size+y]=false;

                                        }
                                      }

                                      /*
                                      if(dobry_sasiad<3)
                                        noise_map[i+delay][j+delay]=true;
                                      else
                                        noise_map[i+delay][j+delay]=false;
                                        */
                                      dobry_sasiad=0;
                                    }
                                 }

                           return true;
                       }
                   }
                return false;
            }


bool FAPGF::calculate_distances()
{
    double*R = RGBptr->get_R();
    double*G = RGBptr->get_G();
    double*B=RGBptr->get_B();
    int delay=RGBptr->get_delay();
    int height=RGBptr->get_height();
    int width=RGBptr->get_width();
    int filtration_window_size=2*delay+1;


    if(R!=nullptr&&measure_type_unit==Distance::Euklidesaowa)
    {

        const int horizontal =width-2*delay;
        const int vertical= height-2*delay;

        if(!aux_tab_is_calculated)
        {        

                  aux_table=new double[horizontal*vertical*filtration_window_size*filtration_window_size];

                    for(int i=delay;i<width-delay;++i)
                    {
                        for(int j=delay;j<height-delay;++j)
                        {
                            for(int z=i-delay;z<i+delay+1;++z)
                            {
                                for(int y=j-delay;y<j+delay+1;++y)
                                {
                                       aux_table[(i-delay)*filtration_window_size*filtration_window_size*vertical+(j-delay)*filtration_window_size*filtration_window_size+(z-i+delay)*filtration_window_size+y-j+delay] = sqrt((R[z+y*width]- R[i+j*width])*(R[z+y*width]- R[i+j*width])+(G[z+y*width]- G[i+j*width])*(G[z+y*width]- G[i+j*width])+(B[z+y*width]- B[i+j*width])*(B[z+y*width]- B[i+j*width]));
                                }
                            }
                        }
                    }

                    aux_tab_is_calculated=true;  
    }
    }
    else if(R!=nullptr&&measure_type_unit==Distance::Czybyszewa)
    {
        const int horizontal =width-2*delay;
        const int vertical= height-2*delay;


        if(!aux_tab_is_calculated)
        {

                    aux_table=new double[horizontal*vertical*filtration_window_size*filtration_window_size];

                    for(int i=delay;i<width-delay;++i)
                    {
                        for(int j=delay;j<height-delay;++j)
                        {
                            for(int z=i-delay;z<i+delay+1;++z)
                            {
                                for(int y=j-delay;y<j+delay+1;++y)
                                {
                                       aux_table[(i-delay)*vertical*filtration_window_size*filtration_window_size+(j-delay)*filtration_window_size*filtration_window_size+(z-i+delay)*filtration_window_size+(y-j+delay)] = std::max(std::max(abs(R[z+y*width]- R[i+j*width]),abs(G[z+y*width]- G[i+j*width])),abs(B[z+y*width]- B[i+j*width]));
                                }
                            }
                        }
                    }

                    aux_tab_is_calculated=true;

        }

        return true;
    }
     else
        return false;
}


void FAPGF::set_distance_type_unit(Distance type)
{
    measure_type_unit=type;
}

bool FAPGF::Clean_Aux_Table()
{
    int width = RGBptr->get_width();
    int delay = RGBptr->get_delay();
    int filtration_window_size=2*delay+1;


    if(aux_table==nullptr)
        delete[] aux_table;

     aux_table=nullptr;

   aux_tab_is_calculated=false;
 return true;
}


double FAPGF::Accuracy(CTRI& ctri)
{

    //ctri nie ma delay

    bool* wskaznik=ctri.get_noise_map();

    int TP=0;
    int TN=0;
    int FP=0;
    int FN=0;

    int delay = RGBptr->get_delay();
    int height=RGBptr->get_height() -2*delay;
    int width=RGBptr->get_width() -2*delay;


   for(int i=0;i<width;++i)
    {

        for(int j=0;j<height;++j)
        {
           if(!wskaznik[i+j*width]&&!noise_map[i+delay+(j+delay)*(width+2*delay)])
               ++TN;
           else
           {
               if(wskaznik[i+j*width]&&!noise_map[i+delay+(j+delay)*(width+2*delay)])
                   ++FN;
               else
               {
                   if(!wskaznik[i+j*width]&&noise_map[i+delay+(j+delay)*(width+2*delay)])
                       ++FP;
                   else
                       ++TP;

               }
           }
        }
    }


    return (TP+TN)*1.0/(TP+TN+FP+FN);
}

void FAPGF::set_RGBImage(RGBImage & img)
{
   if(aux_tab_is_calculated)
   {
       Clean_Aux_Table();
   }
       RGBptr=&img;

   if(noise_map!=nullptr)
   {
           delete [] noise_map;
           noise_map=nullptr;
   }
}

