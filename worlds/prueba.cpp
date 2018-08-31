#include <iostream>
#include <cmath>
#include <libplayerc++/playerc++.h>//cabezeras player
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

 		act.avanzar();


 		if(senso.x != 5)
 		{	
        // Funcion problema1:
 		for(i=0; i<=3; i+=1)
 		{
 			act.u = 1.4142+(2.8284*i);
 			act.w = -.19741+(.105*i);
 		}
}
 // writes the the robot the desired position and velocity

		cout.precision(4);
   		cout << fixed << " (" << senso.x << " , " << senso.y << ")\tyaw: " << sen << "\t" << i << endl;
   		//cout << fixed << "\tSonar left : " << s << "\tSonar right : " << s1 << "\tSonar Front : " << sf << endl;
	}
	return 0;
}