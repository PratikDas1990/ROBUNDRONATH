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
	const int GOforward           = 4;
	const int GOleft              = 3;
	const int GOright             = 2;
	const int GObackward          = 1;
	const int halt                = 10;
	const int continuous_control  = 1;
	const int burst_control       = 2;
	const long int second         = 100000;
	getinput input;//gets direction and power
	GPIOSetup setup;
	GPIOClass* gpio12 = new GPIOClass("12");
	GPIOClass* gpio16 = new GPIOClass("16");
	GPIOClass* gpio19 = new GPIOClass("19");
	GPIOClass* gpio26 = new GPIOClass("26");
	
//initiatevariables and GPIO for ultrasound sensor	
	int echo = 24;
	int trig = 23;
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
//initiate variables and GPIO for PWM speed control
	int direction;
	int control_mode;
	int power;
	string inputstate;
	double distance;
	long double halt_time;

	pinMode(13, PWM_OUTPUT);
	pinMode(18, PWM_OUTPUT);
	power = 1024;	
	halt_time = 0.3;
   

//initiate variables and ports for speed encoder
	const int lsp = 6;//lsp:left speed port 
	const int rsp = 5;//lsp:left speed port
	pinMode(lsp, INPUT);
	pinMode(rsp, INPUT);

	
	struct sigaction sig_struct;
	sig_struct.sa_handler = sig_handler;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
		cout << "Problem with sigaction" << endl;
		exit(1);
	}



	
    //GPIOClass* gpio5 = new GPIOClass("5");
    //GPIOClass* gpio6 = new GPIOClass("6");

	setup.SetGPIOout(gpio12,gpio16,gpio19,gpio26);
	//SetGPIOpwm(gpio5,gpio6);
	
	direction = 0;
	//std::cout<<"Enter 1 for continuous control\n"<<"Enter 2 for burst control\n"<<endl;
	//std::cin>>control_mode;
	control_mode = 2
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
			move_bot(direction,halt_time*second,power,gpio12,gpio16,gpio19,gpio26);
		}
		if(control_mode == continuous_control){
			distance =get_distance(trig,echo);	
			std::cout<<"obstacle"<<distance<<"cm away!!"<<endl;
			//move_bot(direction,power,gpio12,gpio16,gpio19,gpio26);
			move_bot(direction,power,distance,gpio12,gpio16,gpio19,gpio26);
		}
		
		if(direction == 11 ){
			look();
		}
				
		
		
		if(direction !=10){
			continue;
		}
        

         if(ctrl_c_pressed || direction == 10){
			setup.cleanGPIO(gpio12,gpio19,gpio16,gpio26);
			//cleanGPIO(gpio5,gpio6);
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

