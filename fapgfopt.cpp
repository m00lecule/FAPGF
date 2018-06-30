#include "fapgfopt.h"
#include <averagefilter.h>
#include <random>
#include <ctime>
#include <time.h>

using namespace std;

FAPGFOpt::FAPGFOpt()
{
    mutation=0;
    number_of_children=0;
    number_of_elicit=0;
    clear=nullptr;
    disturbed=nullptr;
    fapgf=nullptr;
}


FAPGFOpt::FAPGFOpt(RGBImage *CI, RGBImage *F, int numb,int quantity, int freq_of_mut)
{
    mutation = freq_of_mut;
    clear=CI;
    disturbed=F;
    number_of_elicit=numb;
    number_of_children=quantity;
    fapgf=nullptr;
}

double FAPGFOpt::optimize()
{
    srand(time(0));

    int number6 = number_of_children/6;
    int number3 = number_of_children/3;

    double value=0.02;

    double** children = new double*[number_of_children];
    children[0]=new double[number_of_children*2];

    for(int i=1;i<number_of_children;++i)
        children[i]=&children[0][i*2];

    mt19937 mt(time(0));
    uniform_int_distribution<int> uid(1,100);

        QFile file("/home/m00lecule/Pulpit/Nowy katalog/PSNR1");
        file.open(QIODevice::ReadWrite);
        QTextStream out(&file);
    RGBImage brudny_jamnior;
    //ZAPELNAIMY
    for(int i=0;i<number_of_children;++i)
    {
        double value1 = uid(mt);
        children[i][0]=(value1/100);

        if(fapgf!=nullptr)
            delete fapgf;

        brudny_jamnior=(*(disturbed));
        fapgf=new FAPGF(brudny_jamnior,children[i][0],Distance::Czybyszewa,Noise_Type::CTRI);
        fapgf->detect();

        AverageFilter AF;

        AF.filterRGBImage(fapgf);

        clear->MSE(brudny_jamnior);
        children[i][1]=clear->PSNR();
        clear->flush();
    }
    for(int i=0;i<number6;++i)
    {
        for(int j=i+1;j<number_of_children;++j)
        {
            if(children[i][1]<children[j][1])
            {
                swap(children[i][1],children[j][1]);
                swap(children[i][0],children[j][0]);
            }
        }
    }
/*

    int mut_index = number_of_children*mutation/100;
    //MUTUJEMY
    for(int i=0;i<mut_index;++i)
    {
        uniform_int_distribution<int> uId(i+1,number_of_children-1);

        double value1=uid(mt);
        value1/=10000;
        int ind3x=uId(mt);
        children[ind3x][0]+=value1;

        RGBImage brudny_jamnior=*(disturbed);

        delete fapgf;

        fapgf=new FAPGF(brudny_jamnior,children[ind3x][0],Distance::Czybyszewa,Noise_Type::CTRI);

        fapgf->detect();

        AverageFilter AF;

        AF.filterRGBImage(fapgf);
        clear->MSE(brudny_jamnior);
        children[ind3x][1]=clear->PSNR();
        clear->flush();
    }


    //WYBIERAMY 1/6 NAJLEPSZYCH
    for(int i=0;i<number6;++i)
    {
        for(int j=i+1;j<number_of_children;++j)
        {
            if(children[i][1]<children[j][1])
            {
                swap(children[i][1],children[j][1]);
                swap(children[i][0],children[j][0]);
            }
        }
    }

    //WYWOLANIA
    for(int i=0;i<number_of_elicit-1;++i)
    {

        //MAMY JUZ 1/6 NAJLEPSZYCH

        //KOLEJNE 1/6 RANDOMOWO

        for(int i=number6;i<number3;++i)
        {
             std::uniform_int_distribution<int> zw(i,number_of_children);
             int pick = zw(mt);

             swap(children[i][0],children[pick][0]);
             swap(children[i][1],children[pick][1]);
        }

        //ROZMNAZANIE 1/3 NAJLEPIEJ OKAZAŁYCH OSOBNIKOW

        for(int i=0;i<number3;++i)
        {
            uniform_int_distribution<int> zw(0,number3);

            int parent1 = zw(mt);

            swap(children[0][1],children[parent1][1]);
            swap(children[0][0],children[parent1][0]);

            uniform_int_distribution<int> zw1(1,number3);

            int parent2 = zw1(mt);

            swap(children[0][1],children[parent1][1]);
            swap(children[0][0],children[parent1][0]);

            double value1=children[parent1][0];
            double value2=children[parent2][0];

            if(value2>value1)
            {
                std::uniform_int_distribution<int> uid1(value1*10000,value2*10000);
                children[i+number3][0]=uid1(mt)/10000;
            }
            else
            {
                uniform_int_distribution<int> uid1(value2*10000,value1*10000);
                children[i+number3][0]=uid1(mt)/10000;
            }

            RGBImage brudny_jamnior(*(disturbed));

            delete fapgf;
            fapgf=new FAPGF(brudny_jamnior,children[i+number3][0],Distance::Czybyszewa,Noise_Type::CTRI);
            fapgf->detect();

            AverageFilter AF;

            AF.filterRGBImage(fapgf);
            clear->MSE(brudny_jamnior);
            children[i+number3][1]=clear->PSNR();
            clear->flush();

        }

        //ROZMNAZANIE RANDOMOWO kolejne 1/3 dzieciątek

        for(int i=2*number3;i<number_of_children;++i)
        {
            double value = (uid(mt)/100);

            children[i][0]=value;

            RGBImage brudny_jamnior(*disturbed);

            delete fapgf;

            fapgf= new FAPGF(brudny_jamnior,value,Distance::Czybyszewa,Noise_Type::CTRI);
            fapgf->detect();

            AverageFilter AF;

            AF.filterRGBImage(fapgf);

            clear->MSE(brudny_jamnior);
            children[i][1]=clear->PSNR();
            clear->flush();
        }

        //MUTUJEMY
        for(int i=0;i<mut_index;++i)
        {
            uniform_int_distribution<int> uId(i+1,number_of_children-1);

            double value1=uid(mt);
            value1/=10000;
            int ind3x=uId(mt);
            children[ind3x][0]+=value1;

            RGBImage brudny_jamnior=*(disturbed);

            delete fapgf;

            fapgf=new FAPGF(brudny_jamnior,children[ind3x][0],Distance::Czybyszewa,Noise_Type::CTRI);

            fapgf->detect();

            AverageFilter AF;

            AF.filterRGBImage(fapgf);
            clear->MSE(brudny_jamnior);
            children[ind3x][1]=clear->PSNR();
            clear->flush();
        }


        //WYBIERAMY 1/6 NAJLEPSZYCH
        for(int i=0;i<number6;++i)
        {
            for(int j=i+1;j<number_of_children;++j)
            {
                if(children[i][1]<children[j][1])
                {
                    swap(children[i][1],children[j][1]);
                    swap(children[i][0],children[j][0]);
                }
            }
        }
    }
*/
    for(int i=0;i<number_of_children;++i)
        out<<QString::number(children[i][0],'f',5)<<"    "<<QString::number(children[i][1],'f',5)<<endl;

    value = children[0][0];

    delete[] children[0];
    delete[] children;

    return value;
}

double* FAPGFOpt::optimize_2()
{

    int number6 = number_of_children/6;
    int number3 = number_of_children/3;

    double** children = new double*[number_of_children];

    for(int i=0;i<number_of_children;++i)
        children[i]=new double[3];

    std::mt19937 mt(time(0));
    std::uniform_int_distribution<int> uid(1,100);

        QFile file("/home/m00lecule/Pulpit/Nowy katalog/PSNR");
        file.open(QIODevice::ReadWrite);
        QTextStream out(&file);


    //ZAPELNAIMY
    for(int i=0;i<number_of_children;++i)
    {
        children[i][0]=uid(mt)/100;
        children[i][1]=uid(mt)/100;

        if(fapgf!=nullptr)
            delete fapgf;

        AverageFilter AF;

        RGBImage brudny_jamnior(*(disturbed));
        fapgf = new FAPGF(brudny_jamnior,children[i][0],Distance::Czybyszewa,Noise_Type::GaussNoise);
        fapgf->detect();

        AF.filterALLRGBImage(fapgf);

        fapgf->Clean_Aux_Table();
        fapgf->set_noise_type(Noise_Type::CTRI);
        fapgf->set_d_parameter(children[i][1]);
        fapgf->detect();

        AF.filterRGBImage(fapgf);


        clear->MSE(brudny_jamnior);
        children[i][2]=clear->PSNR();

        out<<QString::number(children[i][0],'f',3)<<" "<<QString::number(children[i][1],'f',3)<<": "<<QString::number(children[i][2],'f',3)<<endl;
        clear->flush();
    }


    /*
    int mut_index = number_of_children*mutation/100;
    //MUTUJEMY
    for(int i=0;i<mut_index;++i)
    {
        uniform_int_distribution<int> uId(i+1,number_of_children-1);
        uniform_int_distribution<int> uuid(0,200);

        int ind3x=uId(mt);

        children[ind3x][0]+=(uuid(mt)-100)/100;
        children[ind3x][1]+=(uuid(mt)-100)/100;

        delete fapgf;
        AverageFilter AF;

        RGBImage brudny_jamnior(*(disturbed));
        fapgf = new FAPGF(brudny_jamnior,children[ind3x][0],Distance::Czybyszewa,Noise_Type::GaussNoise);
        fapgf->detect();

        AF.filterALLRGBImage(fapgf);

        fapgf->Clean_Aux_Table();
        fapgf->set_noise_type(Noise_Type::CTRI);
        fapgf->set_d_parameter(children[ind3x][1]);
        fapgf->detect();

        AF.filterRGBImage(fapgf);

        clear->MSE(brudny_jamnior);
        children[ind3x][2]=clear->PSNR();
        clear->flush();
    }

    //WYBIERAMY 1/6 NAJLEPSZYCH
    for(int i=0;i<number6;++i)
    {
        for(int j=i+1;j<number_of_children;++j)
        {
            if(children[i][2]<children[j][2])
            {
                swap(children[i][1],children[j][1]);
                swap(children[i][0],children[j][0]);
                swap(children[i][2],children[j][2]);
            }
        }
    }

    //WYWOLANIA
    for(int index=0;index<number_of_elicit-1;++index)
    {

        //MAMY JUZ 1/6 NAJLEPSZYCH

        //KOLEJNE 1/6 RANDOMOWO

        for(int i=number6;i<number3;++i)
        {
             std::uniform_int_distribution<int> uid(i,number_of_children);
             int pick = uid(mt);

             swap(children[i][0],children[pick][0]);
             swap(children[i][1],children[pick][1]);
             swap(children[i][2],children[pick][2]);
        }

        //ROZMNAZANIE 1/3 NAJLEPIEJ OKAZAŁYCH OSOBNIKOW

        for(int i=0;i<number3;++i)
        {
            uniform_int_distribution<int> uid(0,number3);

            int parent1 = uid(mt);

            swap(children[0][1],children[parent1][1]);
            swap(children[0][0],children[parent1][0]);
            swap(children[0][2],children[parent1][2]);

            uniform_int_distribution<int> uid1(1,number3);

            int parent2 = uid1(mt);

            swap(children[0][1],children[parent1][1]);
            swap(children[0][0],children[parent1][0]);
            swap(children[0][2],children[parent1][2]);

            double value1=children[parent1][0];
            double value2=children[parent2][0];

            double value12=children[parent1][0];
            double value22=children[parent2][0];

            if(value2>value1)
            {
                uniform_int_distribution<int> uid1(value1*100,value2*100);
                children[i+number6][0]=uid1(mt)/100;
            }
            else
            {
                uniform_int_distribution<int> uid1(value2*100,value1*100);
                children[i+number6][0]=uid1(mt)/100;
            }


            if(value22>value12)
            {
                uniform_int_distribution<int> uid1(value12*100,value22*100);
                children[i+number6][1]=uid1(mt)/100;
            }
            else
            {
                uniform_int_distribution<int> uid1(value22*100,value12*100);
                children[i+number6][1]=uid1(mt)/100;
            }

            RGBImage brudny_jamnior(*(disturbed));
            AverageFilter AF;

            fapgf = new FAPGF(brudny_jamnior,children[i+number6][0],Distance::Czybyszewa,Noise_Type::GaussNoise);
            fapgf->detect();

            AF.filterALLRGBImage(fapgf);

            fapgf->Clean_Aux_Table();
            fapgf->set_noise_type(Noise_Type::CTRI);
            fapgf->set_d_parameter(children[i+number6][1]);
            fapgf->detect();

            AF.filterRGBImage(fapgf);

            clear->MSE(brudny_jamnior);
            children[i+number6][2]=clear->PSNR();
            clear->flush();

        }

        //ROZMNAZANIE RANDOMOWO kolejne 1/3 dzieciątek

        for(int i=2*number3;i<number_of_children;++i)
        {
            double value = uid(mt)/100;
            double value1=uid(mt)/100;

            children[i][0]=value;
            children[i][1]=value1;


            delete fapgf;
            AverageFilter AF;

            RGBImage brudny_jamnior(*(disturbed));

            fapgf = new FAPGF(brudny_jamnior,children[i][0],Distance::Czybyszewa,Noise_Type::GaussNoise);
            fapgf->detect();

            AF.filterALLRGBImage(fapgf);

            fapgf->Clean_Aux_Table();
            fapgf->set_noise_type(Noise_Type::CTRI);
            fapgf->set_d_parameter(children[i][1]);
            fapgf->detect();

            AF.filterRGBImage(fapgf);

            clear->MSE(brudny_jamnior);
            children[i][2]=clear->PSNR();
            clear->flush();
       }

        //WYBIERAMY 1/6 NAJLEPSZYCH
        for(int i=0;i<number6;++i)
        {
            for(int j=i+1;j<number_of_children;++j)
            {
                if(children[i][2]<children[j][2])
                {
                    swap(children[i][2],children[j][2]);
                    swap(children[i][1],children[j][1]);
                    swap(children[i][0],children[j][0]);
                }
            }
        }
    }


    */
    for(int i=0;i<number_of_children;++i)
        delete children[i];

    delete[] children;

    double*ptr = new double[2];
    ptr[0]=children[0][0];
    ptr[1]=children[0][1];

    return ptr;
}




