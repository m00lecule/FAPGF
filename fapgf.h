#ifndef FAPGF_H
#define FAPGF_H

#include <QObject>
#include <QImage>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <ctri.h>
#include <rgbimage.h>

enum class Distance{Czybyszewa, Euklidesaowa};
enum class Noise_Type{CTRI,GaussNoise};

extern void gpu_calcul(double r_d,int delay, double* r,double* g, double* b, int w, int h, bool*& nosie_map);
//extern void set_val(double* r, int w, int h, int delay);
extern void set_zeros(float* tab,float* tab1,float* tab2, int w, int h);

class FAPGF
{
public:
    FAPGF();
    FAPGF(RGBImage&,double,Distance,Noise_Type);
    ~FAPGF();

    void set_distance_type_unit(Distance type); 
    void set_noise_type(Noise_Type nt) {Type_of_noise=nt;}
    bool set_d_parameter(float const);
    void set_RGBImage(RGBImage&);

    bool detect();

    void detect_CUDA(){
/*
         double* ptr =RGBptr->get_R();
         int width=RGBptr->get_width();
         int height=RGBptr->get_height();


         set_val(ptr,RGBptr->get_width(),RGBptr->get_height(),RGBptr->get_delay());

         QFile outputFile("/home/michald/Dokumenty/FAPGF/kolory.txt");
         outputFile.open(QIODevice::WriteOnly);


         QTextStream outStream(&outputFile);

         for(int i=0;i<width;++i){
             for(int j=0;j<height;++j)
                 outStream << QString::number(ptr[i+j*width],'f',2)<<" ";

             outStream<<endl;
         }
         outputFile.close();



         */

/*
       int width=RGBptr->get_width();
       int height=RGBptr->get_height();

       if(noise_map!=nullptr){
           delete[] noise_map;
       }

      noise_map=new bool[width*height];

      gpu_calcul(d_parameter ,RGBptr->get_delay(), RGBptr->get_R() , RGBptr->get_G() , RGBptr->get_B() , RGBptr->get_width() , RGBptr->get_height() , noise_map );
*/

        int w=5;
        int h=2;
        float* t=new float[w*h];
        float* t1=new float[w*h];
        float* t2=new float[w*h];

        for(int i=0;i<w*h;++i){
            t1[i]=10;
            t[i]=15;
            t2[i]=1;
        }
        set_zeros(t,t1,t2,w,h);

        QFile outputFile("/home/michald/Dokumenty/FAPGF/jamniory.txt");
        outputFile.open(QIODevice::WriteOnly);


        QTextStream outStream(&outputFile);

        for(int i=0;i<h;++i){
            for(int j=0;j<w;++j)
                outStream << QString::number(t2[i*w+j],'f',2)<<" ";

            outStream<<endl;
        }
        delete[] t1;
        delete[] t;
        delete[] t2;
        outputFile.close();
    }

    bool calculate_distances();
    double Accuracy(CTRI&);

    bool Clean_Aux_Table();
    bool* get_noise_map() const {return noise_map;}

    bool* get_aux_b_table() const { return aux_b_table; }

    RGBImage* get_RGB() const {return RGBptr;}


private:
    bool aux_tab_is_calculated;
    bool* noise_map;

    double* aux_table;
    bool* aux_b_table;

    RGBImage *RGBptr;
    double d_parameter;
    Distance measure_type_unit;
    Noise_Type Type_of_noise;

};

#endif // FAPGF_H
