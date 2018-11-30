#include <iostream>
#include <armadillo>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING // Stop warning "Boost.Signals"
#include <libplayerc++/playerc++.h>//cabezeras player

using namespace std;
using namespace arma;
using namespace PlayerCc; //LIbreria para player
#define pi 3.14159

////////////////////////////////////////////////////////Robot 1 (CONFIGURACION) /////////////////////////////////////////////////////////////////
PlayerClient reybot1("localhost",6665); // forma de declarar conexion al bot y su name.
Position2dProxy pp1(&reybot1,0); //usar position2d

class actuador
{
private:
mat xi;
int R=2,Rm=30;
public:
static mat ob1,ob2,x;
double veltt=0.0,velt2=0.0,velt=0.0,angt2=0.0,angt=0.0,norm1=0.0,gan2=0.0,gan = 0.0,vely =0.1,velx =0.1,deltax=0.0,deltay=0.0,norm2=0.0;
double velt3=0.0,angt3=0.0,norm3=0.0,gan3=0.0;
void avanzar()
{
        reybot1.Read();
        deltax=x(0,0);
        deltay=x(1,0);
        //cout << deltax << endl << deltay << endl;

        if(deltax < 0 and deltay > 0)
                gan3 = 3;
        else if(deltax < 0 and deltay < 0  )
                gan3 = -3;
        else
                gan3 = 0.0;

        norm1 = sqrt(pow(ob1(0,0),2) + pow(ob1(1,0),2));
        norm2 = sqrt(pow(ob2(0,0),2) + pow(ob2(1,0),2));
        norm3 = sqrt(pow(deltax,2)+ pow(deltay,2));

        ////////////////////////////////////////////////////////// Robot 1 (Actuacion) ////////////////////////////////////////////////////////////////////
        mat vel(2,1);
        mat vel2(2,1);
        mat vel3(2,1);

          vel3= ( (-3*exp((norm3)/(Rm)))/(Rm*norm3) ) * x;

          velt3 = sqrt(pow(vel3(0,0),2)+pow(vel3(1,0),2));
         angt3 =  (atan((vel3(1,0))/(vel3(0,0)))+gan3)- pp1.GetYaw();




        if(ob1(0,0) != 0 and ob1(1,0) != 0)
        {
                vel= ( (0.01*exp(-(norm1)/(R)))/(R* -norm1) ) * ob1;

              //  reybot1.Read();

                velt = sqrt(pow(vel(0,0),2)+pow(vel(1,0),2));

                if(ob1(0,0) < 0 and ob1(1,0) > 0  )
                        gan = -3;
                else
                        gan = 0.0;

                angt =  (atan((vel(1,0))/(vel(0,0)))+gan) - pp1.GetYaw();
        }

        if(ob2(0,0) != 0 and ob2(1,0) != 0)
        {
                vel2= ( (0.01*exp(-(norm2)/(R)))/(R* -norm2) ) * ob2;

              //  reybot1.Read();

                velt2 = sqrt(pow(vel2(0,0),2)+pow(vel2(1,0),2));

                if(ob2(0,0) < 0 and ob2(1,0) > 0  )
                        gan2 = -3;
                else
                        gan2 = 0.0;

                angt =  (atan((vel2(1,0))/(vel2(0,0)))+gan2) - pp1.GetYaw();
        }

        if((ob2(0,0) == 0 and ob2(1,0) == 0) and (ob1(0,0) == 0 and ob1(1,0) == 0) )
        {
            //    usleep(150);
          //      pp1.SetSpeed(0,0);
                velt=0.0,velt2=0.0;
                angt=0.0,angt2=0.0;
        }

      //  else
      //  {
                usleep(150);
                pp1.SetSpeed(velt+velt2+velt3,angt+angt2+angt3);
                cout << "Velocidad tangencial: " << velt+velt2+velt3 << " Velocidad angular: " << angt+angt2+angt3 <<  endl;
  //      }
}
};

mat actuador::ob1,actuador::ob2,actuador::x;

int main()
{
        actuador act;
        double ww,ww1,ww2,ww3,ww4,ww5;

        // ftok to generate unique key
        key_t key = ftok("shmfile",65);

        // shmget returns an identifier in shmid
        int shmid = shmget(key,1024,0666|IPC_CREAT);

        while(1)
        {
                double *str = (double*) shmat(shmid,(void*)0,0);

                ww = str[0];
                ww1 = str[1];
                ww2 = str[2];
                ww3=str[3];
                ww4 = str[4];
                ww5 = str[6];

                actuador::ob1 << ww << endr << ww1 << endr;
                actuador::ob2 << ww2 << endr << ww3 << endr;
                actuador::x << ww4 << endr << ww5 << endr;

                act.avanzar();
                shmdt(str);

        }
        return 0;
}