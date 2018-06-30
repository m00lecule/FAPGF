#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fapgfopt.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), number_of_buttons(5),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(600,200,800,600);
    Buttons = new QPushButton*[number_of_buttons];

    for(int i=0;i<number_of_buttons;++i)
        Buttons[i]=new QPushButton(this);

        Buttons[0]->setEnabled(true);
        Buttons[1]->setEnabled(false);
        Buttons[2]->setEnabled(false);
        Buttons[3]->setEnabled(false);
        Buttons[4]->setEnabled(false);

        Buttons[0]->setText("Browse");
        Buttons[1]->setText("Save");
        Buttons[2]->setText("Disturb Picture");
        Buttons[3]->setText("Filter");
        Buttons[4]->setText("Optimize");

        font = new QFont("MS Shell Dlg2",8);

        Fresh=new QLabel(this);
        Fresh->setAlignment(Qt::AlignCenter);

        Disturbed=new QLabel(this);
        Disturbed->setAlignment(Qt::AlignCenter);

        Cleaned=new QLabel(this);
        Cleaned->setAlignment(Qt::AlignCenter);

        ComboBox= new QComboBox(this);

        ComboBox->addItem("Czybyszevs'");
        ComboBox->addItem("Euclidean");


        ComboBox->setEditable(true);
        ComboBox->lineEdit()->setReadOnly(true);
        ComboBox->lineEdit()->setAlignment(Qt::AlignCenter);
        for(int i = 0; i < ComboBox->count(); i++)
          ComboBox->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);



        SpinBox = new QSpinBox(this);
        SpinBox->setAlignment(Qt::AlignCenter);

        SpinBox->setMaximum(100);
        SpinBox->setMinimum(0);
        SpinBox->setSingleStep(2);
        SpinBox->setValue(12);

        SB_D_Parameter = new QDoubleSpinBox(this);
        SB_D_Parameter->setAlignment(Qt::AlignCenter);
        SB_D_Parameter->setMaximum(1);
        SB_D_Parameter->setMinimum(0.01);
        SB_D_Parameter->setSingleStep(0.01);
        SB_D_Parameter->setValue(0.10);

        SB_2_D_Parameter = new QDoubleSpinBox(this);
        SB_2_D_Parameter->setAlignment(Qt::AlignCenter);
        SB_2_D_Parameter->setMaximum(1);
        SB_2_D_Parameter->setMinimum(0.01);
        SB_2_D_Parameter->setSingleStep(0.01);
        SB_2_D_Parameter->setValue(0.10);

        SB_2_D_Parameter->hide();

        Search_Window_size = new QComboBox(this);

        Search_Window_size->addItem("5");
        Search_Window_size->addItem("7");
        Search_Window_size->addItem("9");
        Search_Window_size->addItem("11");

        Search_Window_size->setEditable(true);
        Search_Window_size->lineEdit()->setReadOnly(true);
        Search_Window_size->lineEdit()->setAlignment(Qt::AlignCenter);

        for(int i = 0; i < Search_Window_size->count(); i++)
          Search_Window_size->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);


        Noise=new QComboBox(this);

        Noise->addItem("CTRI");
        Noise->addItem("Gauss Noise");

        connect(this->Noise,SIGNAL(activated(int)),this,SLOT(Change_SpinBox_maximum(int)));

        Noise->setEditable(true);
        Noise->lineEdit()->setReadOnly(true);
        Noise->lineEdit()->setAlignment(Qt::AlignCenter);

        for(int i = 0; i < Noise->count(); i++)
            Noise->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);



        PSNT_text=new QLabel(this);
        MSE_text=new QLabel(this);
        Distance_type_text=new QLabel(this);
        Accuracy_text=new QLabel(this);
        Window_frame_text=new QLabel(this);
        Noise_Density=new QLabel(this);
        NCD_text=new QLabel(this);
        d_parameter=new QLabel(this);
        d2_parameter=new QLabel(this);
        Noise_text=new QLabel(this);

        PSNT_text->setText("PSNR");
        MSE_text->setText("MSE");
        Distance_type_text->setText("Distance measure type");;
        Accuracy_text->setText("Accuracy");
        Window_frame_text->setText("Filtr.Wind.Size");
        Noise_Density->setText("Density");
        NCD_text->setText("NCD");
        d_parameter->setText("d parameter");
        d2_parameter->setText("2nd d parameter");

        d2_parameter->hide();

        Window_orient=Orientation::Vertical;

        MSE=new QLCDNumber(this);
        MSE->show();

        PSNR=new QLCDNumber(this);
        PSNR->show();

        Accuracy=new QLCDNumber(this);
        Accuracy->show();

        NCD=new QLCDNumber(this);
        NCD->show();

        ctri=nullptr;


        dt=nullptr;
        cl=nullptr;
        cn=nullptr;


    connect(Buttons[0],SIGNAL(clicked()),this,SLOT(Browse()));
    connect(Buttons[1],SIGNAL(clicked()),this,SLOT(Save()));
    connect(Buttons[2],SIGNAL(clicked()),this,SLOT(Disturb()));
    connect(Buttons[3],SIGNAL(clicked()),this,SLOT(Clean()));
    connect(Buttons[4],SIGNAL(clicked()),this,SLOT(Optimize()));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if(this)
    {
        const int number_of_buttons=5;
        const int width = this->width();
        const int height = this->height();
        const int gap = 5;
        const int frame = 15;

        const int width_of_Label=((width-2*frame-2*gap)/3);
        const int height_of_Label=7*(height/10);


        const int width_of_button=((width-2*frame-(number_of_buttons-1)*gap)/number_of_buttons);
        const int height_of_button=height/10;

        const int ComboBoxs_height = height/20;

        ComboBox->setGeometry(frame,9*height_of_button-frame-gap-height_of_button/2,width_of_button*3/4,height_of_button/2);
        Distance_type_text->setGeometry(frame,9*height_of_button-frame-4*gap-height_of_button/2,width_of_button*3/4,3*gap);

        Noise->setGeometry(frame,9*height_of_button-frame-4*gap-2*height_of_button/2,width_of_button*3/4,height_of_button/2);
        Noise_text->setGeometry(frame,9*height_of_button-frame-4*gap-2*height_of_button/2,width_of_button*3/4,3*gap);

        if (width >= 1300)
        {
                font->setPixelSize(30);
        }
        else if ((width < 1300) && (width >= 875))
        {
                font->setPixelSize(23);
        }
        else if ((width < 875) && (width >= 800))
        {
                font->setPixelSize(22);
        }
        else if ((width < 800) && (width >= 725))
        {
                font->setPixelSize(21);
        }
        else if ((width < 725) && (width >= 700))
        {
                font->setPixelSize(20);
        }
        else if ((width < 700) && (width >= 625))
        {
                font->setPixelSize(11);
        }
        else if ((width < 625) && (width >= 550))
        {
                font->setPixelSize(10);
        }
        else
        {
               font->setPixelSize(8);
        };


        this->set_Font();
        if(Window_orient==Orientation::Vertical)
        {
            Fresh->setGeometry(frame,frame,width_of_Label,height_of_Label);
            Fresh->setPixmap(Fresh_Pixmap.scaled(width_of_Label,height_of_Label,Qt::KeepAspectRatio));

            Disturbed->setGeometry(frame+width_of_Label+gap,frame,width_of_Label,height_of_Label);
            Disturbed->setPixmap(Disturbed_Pixmap.scaled(width_of_Label,height_of_Label,Qt::KeepAspectRatio));

            Cleaned->setGeometry(frame+2*width_of_Label+2*gap,frame,width_of_Label,height_of_Label);
            Cleaned->setPixmap(Cleaned_Pixmap.scaled(width_of_Label,height_of_Label,Qt::KeepAspectRatio));

        }
        else
        {
            const int height_of_horizontal_label = (height_of_Label-2*gap)/3;
            const int width_of_horizontal_label = width-2*frame;

            Fresh->setGeometry(frame,frame,width_of_horizontal_label,height_of_horizontal_label);
            Fresh->setPixmap(Fresh_Pixmap.scaled(width_of_horizontal_label,height_of_horizontal_label,Qt::KeepAspectRatio));

            Disturbed->setGeometry(frame,frame+height_of_horizontal_label+gap,width_of_horizontal_label,height_of_horizontal_label);
            Disturbed->setPixmap(Disturbed_Pixmap.scaled(width_of_horizontal_label,height_of_horizontal_label,Qt::KeepAspectRatio));

            Cleaned->setGeometry(frame,frame+2*(height_of_horizontal_label+gap),width_of_horizontal_label,height_of_horizontal_label);
            Cleaned->setPixmap(Cleaned_Pixmap.scaled(width_of_horizontal_label,height_of_horizontal_label,Qt::KeepAspectRatio));
        }


        SpinBox->setGeometry(frame+width_of_button+gap,9*height_of_button-frame-gap-height_of_button/2,width_of_button/3,height_of_button/2);
        Noise_Density->setGeometry(frame+width_of_button+gap,9*height_of_button-frame-4*gap-height_of_button/2,width_of_button/3,3*gap);

        SB_D_Parameter->setGeometry(frame+width_of_button+2*gap+width_of_button/3,9*height_of_button-frame-gap-height_of_button/2,(width_of_button-2*gap)/3,height_of_button/2);
        d_parameter->setGeometry(frame+width_of_button+2*gap+(width_of_button-2*gap)/3,9*height_of_button-frame-4*gap-height_of_button/2,(width_of_button-2*gap)/3,3*gap);

        SB_2_D_Parameter->setGeometry(frame+width_of_button+3*gap+2*(width_of_button-2*gap)/3,9*height_of_button-frame-gap-height_of_button/2,(width_of_button-2*gap)/3,height_of_button/2);
        d2_parameter->setGeometry(frame+width_of_button+3*gap+2*(width_of_button-2*gap)/3,9*height_of_button-frame-4*gap-height_of_button/2,(width_of_button-2*gap)/3,3*gap);

        PSNR->setGeometry(frame+2*width_of_button+2*gap,9*height_of_button-frame-gap-height_of_button/2,(width_of_button-2*gap)/3,height_of_button/2);
        PSNT_text->setGeometry(frame+2*width_of_button+2*gap,9*height_of_button-frame-gap-height_of_button/2-3*gap,(width_of_button-2*gap)/3,3*gap);

        MSE->setGeometry(frame+2*width_of_button+3*gap+(width_of_button-2*gap)/3,9*height_of_button-frame-gap-height_of_button/2,(width_of_button-2*gap)/3,height_of_button/2);
        MSE_text->setGeometry(frame+2*width_of_button+3*gap+(width_of_button-2*gap)/3,9*height_of_button-frame-4*gap-height_of_button/2,(width_of_button-2*gap)/3,3*gap);

        NCD->setGeometry(frame+2*width_of_button+2*gap,9*height_of_button-frame-2*gap-height_of_button-3*gap,(width_of_button-2*gap)/3,height_of_button/2);
        NCD_text->setGeometry(frame+2*width_of_button+2*gap,9*height_of_button-frame-2*gap-height_of_button-6*gap,(width_of_button-2*gap)/3,3*gap);

        Accuracy->setGeometry(frame+2*width_of_button+4*gap+2*(width_of_button-2*gap)/3,9*height_of_button-frame-gap-height_of_button/2,(width_of_button-2*gap)/3,height_of_button/2);
        Accuracy_text->setGeometry(frame+2*width_of_button+4*gap+2*(width_of_button-2*gap)/3,9*height_of_button-frame-4*gap-height_of_button/2,(width_of_button-2*gap)/3,3*gap);

        Search_Window_size->setGeometry(frame+3*width_of_button+3*gap,9*height_of_button-frame-gap-height_of_button/2,width_of_button/4,height_of_button/2);
        Window_frame_text->setGeometry(frame+3*width_of_button+3*gap,9*height_of_button-frame-4*gap-height_of_button/2,width_of_button,3*gap);


        for(int i=0;i<number_of_buttons;++i)
            Buttons[i]->setGeometry(frame+(gap+width_of_button)*i,9*height_of_button-frame,width_of_button,height_of_button);

    }
}


MainWindow::~MainWindow()
{
    for(int i=0;i<2;++i)
        delete Buttons[i];

   delete [] Buttons;

    delete Fresh;
    delete Disturbed;

    delete Cleaned;

    delete ctri;
    delete font;

    delete Cleaned;

    delete PSNT_text;
    delete MSE_text;
    delete Distance_type_text;
    delete Accuracy_text;
    delete Window_frame_text;
    delete Noise_Density;
    delete NCD_text;
    delete d_parameter;
    delete d2_parameter;
    delete Noise_text;

    delete ComboBox;
    delete Search_Window_size;

    delete SpinBox;
    delete SB_D_Parameter;
    delete SB_2_D_Parameter;

    delete MSE;
    delete PSNR;
    delete Accuracy;
    delete NCD;

    delete Noise;

    delete ui;
}

void MainWindow::Browse()
{
   QString Path=QFileDialog::getOpenFileName(this,tr("Pick image"),"C:\\Users\\michaldygaz\\Desktop",tr("Image (*.jpg *.jpeg *.png *bmp)"));

   if(Path!="")
   {
       Buttons[1]->setEnabled(true);
       Buttons[2]->setEnabled(true);
       Image.load(Path);

        if(Image.height()*2>Image.width())
            Window_orient=Orientation::Vertical;
        else
            Window_orient=Orientation::Horizontal;

       Fresh_Pixmap = QPixmap::fromImage(Image);
       Cleaned_Pixmap=Fresh_Pixmap;
       Disturbed_Pixmap=Fresh_Pixmap;

       Fresh->setPixmap(Fresh_Pixmap.scaled(Fresh->width(),Fresh->height(),Qt::KeepAspectRatio));
       Disturbed->setPixmap(Fresh_Pixmap.scaled(Fresh->width(),Fresh->height(),Qt::KeepAspectRatio));
       Cleaned->setPixmap(Fresh_Pixmap.scaled(Fresh->width(),Fresh->height(),Qt::KeepAspectRatio));
   }

}

void MainWindow::Save()
{
   QString Path= QFileDialog::getSaveFileName(this,tr("Choose location:"),"C:\\Users\\michaldygaz\\Desktop",tr("Image (*.jpg *.jpeg *.png *bmp)"));

   QFile file(Path);
   file.open(QIODevice::WriteOnly);

   Cleaned_Pixmap.save(&file,"PNG");
}
void MainWindow::Disturb()
{
    SWS=2*(Search_Window_size->currentIndex())+5;

    Buttons[3]->setEnabled(true);
    Buttons[4]->setEnabled(true);

    PSNR->display(0.00);
    MSE->display(0.00);
    Accuracy->display(0.00);
    NCD->display(0.00);

    if(cl!=nullptr)
        delete cl;

    cl=new RGBImage(Image,SWS);
    copy=Image;

    switch(static_cast<Noise_Type>(Noise->currentIndex()))
    {
        case(Noise_Type::CTRI):
        {
            ctri=new CTRI(SpinBox->value());
            ctri->Disturb(copy);

        }        break;

        case(Noise_Type::GaussNoise):
        {
            GaussNoise GN(SpinBox->value());
            GN.Disturb(copy);
        } break;

    }

    Disturbed_Pixmap=QPixmap::fromImage(copy);
    Cleaned_Pixmap=Disturbed_Pixmap;

    Disturbed->setPixmap(Disturbed_Pixmap.scaled(Disturbed->width(),Disturbed->height(),Qt::KeepAspectRatio));
    Cleaned->setPixmap(Disturbed_Pixmap.scaled(Disturbed->width(),Disturbed->height(),Qt::KeepAspectRatio));
}

void MainWindow::Clean()
{
    Noise_Type temporary=static_cast<Noise_Type>(Noise->currentIndex());

    if(dt!=nullptr)
        delete dt;

    AverageFilter AF;

     dt= new RGBImage(copy,SWS);

     QImage distrb = copy;

    switch(temporary)
    {
        case(Noise_Type::CTRI):
        {
            FAPGF fapgf(*(dt),SB_D_Parameter->value(),static_cast<Distance>(ComboBox->currentIndex()),temporary);
            fapgf.detect();
            //fapgf.detect_CUDA();
            AF.filterImage(&fapgf,distrb);
            Accuracy->display(fapgf.Accuracy(*ctri)*100);

        }break;

        case(Noise_Type::GaussNoise):
        {
            FAPGF fapgf(*(dt),SB_D_Parameter->value(),static_cast<Distance>(ComboBox->currentIndex()),Noise_Type::CTRI);
            fapgf.detect();

            fapgf.Clean_Aux_Table();
            fapgf.set_noise_type(Noise_Type::GaussNoise);
            fapgf.set_d_parameter(SB_2_D_Parameter->value());
            fapgf.detect();

            AF.filterALL(&fapgf,distrb);

            AF.filterImage(&fapgf,distrb);
        } break;
    }

    Cleaned_Pixmap=QPixmap::fromImage(distrb);
    Cleaned->setPixmap(Cleaned_Pixmap.scaled(Cleaned->width(),Cleaned->height(),Qt::KeepAspectRatio));


    MSE->display(cl->MSE(*(dt)));
    PSNR->display(cl->PSNR());
    cl->flush();
}


void MainWindow::set_Font()
{
    for(int i=0;i<number_of_buttons;++i)
      Buttons[i]->setFont(*(font));

    ComboBox->setFont(*font);
    Noise->setFont(*font);
    Search_Window_size->setFont(*font);

    SpinBox->setFont(*font);
    SB_D_Parameter->setFont(*font);
    SB_2_D_Parameter->setFont(*font);
}

void MainWindow::Change_SpinBox_maximum(int index)
{
    switch(static_cast<Noise_Type>(index))
    {
        case(Noise_Type::CTRI):
        {
            SpinBox->setMaximum(100);
            SB_2_D_Parameter->hide();
            d2_parameter->hide();
            Noise_Density->setText("Density");

        }break;

        case(Noise_Type::GaussNoise):
        {
            SpinBox->setMaximum(255);
            Noise_Density->setText("Range");
            SB_2_D_Parameter->show();
            d2_parameter->show();

        }break;

    }
}

void MainWindow::Optimize()
{

    if(dt!=nullptr)
        delete dt;

   dt= new RGBImage(copy,SWS);
   Noise_Type temporary=static_cast<Noise_Type>(Noise->currentIndex());

   RGBImage zdjecie(clean,SWS);
   zdjecie=(*dt);

   if(zdjecie==(*dt))
       PSNR->display(666);
   else
       PSNR->display(111);

   /*
   if(temporary==Noise_Type::CTRI)
   {
        FAPGFOpt fapgfopt(cl,dt,2,30,30);
        SB_D_Parameter->setValue(fapgfopt.optimize());
   }
   else
   {
       FAPGFOpt fapgfopt(cl,dt,1,10,30);
       double* tmp = fapgfopt.optimize_2();
       SB_D_Parameter->setValue(tmp[0]);
       SB_2_D_Parameter->setValue(tmp[1]);
   }
   */
}
