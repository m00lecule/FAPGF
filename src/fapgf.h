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
