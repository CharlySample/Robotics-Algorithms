#include <iostream>
#include <libplayerc++/playerc++.h>//cabezeras player
#include <cmath>
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING // Stop warning "Boost.Signals"

using namespace std;
using namespace PlayerCc; //LIbreria para player

PlayerClient reybot("localhost"); // forma de declarar conexion al bot y su name.
RangerProxy rp(&reybot,0); //usar ranger
Position2dProxy pp(&reybot,0); //usar position2d

class sensado
{
public:

	double x = pp.GetXPos();
	double y = pp.GetYPos();
	double a = pp.GetYaw();
};

class actuado
{
public:

	double u,w;
	 
	 void avanzar()
	 {

	 	pp.SetSpeed(u,w);
	 }
	
};

int main()
{
	float i = 0.0;
	while(1)
	{
		reybot.Read();
		sensado senso;
		actuado act;
		double sen = senso.a * (180/3.141615);

 		//act.avanzar();

        // Funcion problema8:
 		for(i=0; i<=1; i+=0.1)
 		{
 			act.u = (1/4) + (1/2)*i - (pow(i,2)/-8);
 			act.w = -(pow(i,2)/8) + (pow(i,3)/16);
 			act.avanzar();

 		}

 // writes the the robot the desired position and velocity

		cout.precision(4);
   		cout << fixed << " (" << senso.x << " , " << senso.y << ")\tyaw: " << sen << "\t" << i << endl;
   		//cout << fixed << "\tSonar left : " << s << "\tSonar right : " << s1 << "\tSonar Front : " << sf << endl;
	}
	return 0;
}