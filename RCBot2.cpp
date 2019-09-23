#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "GPIOClass.h"
#include "GPIOFun.h"
#include "GOPIFun.h"
using namespace std;

void sig_handler(int sig);
bool ctrl_c_pressed = false;

int main (void)
{
//Start and check wiringPi
   	if (wiringPiSetupGpio() == -1){
		std::cout<<"trouble with wiringPi"<<endl;
		exit (1) ;
	}

//initiate viriables for direction control	
	const int continuous_control  = 1;
	const int burst_control       = 2;
	const long int second         = 100000;
	getinput input;//gets direction and power
	GOPI pi;	
//initiate variables and GPIO for PWM speed control
	int direction;
	int control_mode;
	int power;
	string inputstate;
	double distance;
	long double halt_time;

	power = 1024;	
	halt_time = 0.3;
	
	struct sigaction sig_struct;
	sig_struct.sa_handler = sig_handler;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
		cout << "Problem with sigaction" << endl;
		exit(1);
	}

	//SetGPIOpwm(gpio5,gpio6);
	
	direction = 0;
	std::cout<<"Enter 1 for continuous control\n"<<"Enter 2 for burst control\n"<<endl;
	std::cin>>control_mode;
	
	if(control_mode == continuous_control){
		std::cout<<"Enter speed:\n"<<endl;
		std::cin>>power;
	}
	if(control_mode == burst_control){
		//std::cout<<"Enter halt time in seconds:\n"<<endl;
		//std::cin>>halt_time;
		halt_time = 0.38;
		//std::cout<<"Enter speed:\n"<<endl;
		//std::cin>>power;
		power = 460;
	}
    while(1)
    {
		
		direction=input.direction();
		
		if(direction == 5 ){
			power = input.power();
			direction=input.direction();
		}
		
        if(control_mode == burst_control){
			pi.move_bot(direction,halt_time*second,power);
		}
		if(control_mode == continuous_control){
			distance =pi.get_distance();	
			std::cout<<"obstacle"<<distance<<"cm away!!"<<endl;
			pi.move_bot(direction,power);
		}
		
		if(direction == 11 ){
			look();
		}
				
		
		
		if(direction !=10){
			continue;
		}
        

         if(ctrl_c_pressed || direction == 10){
			pi.cleanGPIO();
     		break;
        }

    }
    cout << "Exiting....." << endl;
    return 0;
}

void sig_handler(int sig){
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
 }

