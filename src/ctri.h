#ifndef CTRI_H
#define CTRI_H

#include <QImage>
#include <qglobal.h>
#include <ctime>

class CTRI
{
public:
    CTRI();
    CTRI(double);

    bool set_density(double);
    bool* get_noise_map() const { return map_of_noise;}
    bool Disturb(QImage &);

private:
    double density;
    bool* map_of_noise;
};

#endif // CTRI_H
