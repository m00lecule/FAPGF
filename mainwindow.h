#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <qglobal.h>
#include <fapgf.h>
#include <ctri.h>
#include "averagefilter.h"
#include <QComboBox>
#include <QPixmap>
#include <QSpinBox>
#include <QFont>
#include <QLCDNumber>
#include "gaussnoise.h"
#include <QLineEdit>
#include <rgbimage.h>

enum struct Orientation {Vertical,Horizontal};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void Change_SpinBox_maximum(int);
    void Browse();
    void Save();
    void Disturb();
    void Clean();
    void Optimize();

private:



    unsigned int SWS;

    Ui::MainWindow *ui;

    QImage Image;
    RGBImage* cl;
    RGBImage* dt;
    RGBImage* cn;

    QImage copy;
    QImage clean;

    QFont *font;


    QLabel* Fresh;

    QPixmap Fresh_Pixmap;
    QPixmap Disturbed_Pixmap;
    QPixmap Cleaned_Pixmap;

    QLabel *Disturbed;
    QLabel *Cleaned;

    QLabel *PSNT_text;
    QLabel *MSE_text;
    QLabel *Distance_type_text;
    QLabel *Accuracy_text;
    QLabel *Window_frame_text;
    QLabel *Noise_Density;
    QLabel *NCD_text;
    QLabel *d_parameter;
    QLabel *d2_parameter;
    QLabel *Noise_text;


    QPushButton **Buttons;

    CTRI *ctri;


    QComboBox * ComboBox;
    QComboBox * Search_Window_size;

    Orientation Window_orient;

    QSpinBox * SpinBox;
    QDoubleSpinBox * SB_D_Parameter;
    QDoubleSpinBox * SB_2_D_Parameter;

    QLCDNumber * MSE;
    QLCDNumber * PSNR;
    QLCDNumber * Accuracy;
    QLCDNumber * NCD;

    QComboBox *Noise;

    const unsigned int number_of_buttons;


    void set_Font();
    void resizeEvent(QResizeEvent*);
};

#endif // MAINWINDOW_H
