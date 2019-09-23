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
#include <Python.h>
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
	const int GOforward           = 1;
	const int GOleft              = 2;
	const int GOright             = 3;
	const int GObackward          = 4;
	const int halt                = 10;
	const int continuous_control  = 1;
	const int burst_control       = 2;
	const long int second         = 100000;
	GPIOSetup setup;
	GPIOClass* gpio12 = new GPIOClass("12");
	GPIOClass* gpio16 = new GPIOClass("16");
	GPIOClass* gpio19 = new GPIOClass("19");
	GPIOClass* gpio26 = new GPIOClass("26");
	setup.SetGPIOout(gpio12,gpio16,gpio19,gpio26);

//Start servo set-up
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
        PyObject* myModuleString;
        PyObject* myModule;
	PyObject* myFunction;
        myModuleString = PyUnicode_FromString("servo");
        myModule = PyImport_Import(myModuleString);
	myFunction = PyObject_GetAttrString(myModule,(char*)"moveservo");
	
//initiatevariables and GPIO for ultrasound sensor	
	int echo = 24;
	int trig = 23;
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
//initiate variables and GPIO for PWM speed control
	int direction;
	int control_mode;
	int power;
	int power_prev;
	int random_decision;
	string inputstate;
	double distance;
	double last_distance;
	double right_distance;
	double left_distance;
	int change_in_distance;
	int change_direction;
	int last_direction;
	int count;
	int count_nomovement;
	long double halt_time,movement_time;

	pinMode(13, PWM_OUTPUT);
	pinMode(18, PWM_OUTPUT);
	power = 1024;	
	halt_time = 4.9;
	distance = 100.0;
	change_direction = 0;
	movement_time = 3.0;
   
//initiate variables and ports for speed encoder
	double speed;
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
//initiate pwm ports for controlling the neck
	const int lookforward         = 90;
	const int lookleft            = 180;
	const int lookright           = 0;
	const int lookup              = 0;
	const int lookdown            = 180;
	const int x_axis              = 21;
	const int y_axis              = 20;
	
	/*if(softPwmCreate (x_axis, lookforward, 200) == 0){
		std::cout<<"Soft pwm pin created successfully"<<endl;
	}
	if(softPwmCreate (y_axis, lookforward, 200) == 0){
		std::cout<<"Soft pwm pin created successfully"<<endl;
	}*/
        //softPwmWrite (21,15 ) ;//x//max 22,min 7, mean 15
	//softPwmWrite (20,15 ) ;//y
	
    //GPIOClass* gpio5 = new GPIOClass("5");
    //GPIOClass* gpio6 = new GPIOClass("6");


	//SetGPIOpwm(gpio5,gpio6);
	
	direction = 0;
	//std::cout<<"Enter 1 for continuous control\n"<<"Enter 2 for burst control\n"<<endl;
	//std::cin>>control_mode;
	
	//if(control_mode == continuous_control){
	//	std::cout<<"Enter speed:\n"<<endl;
	//	std::cin>>power;
	//}
	control_mode = burst_control;
/* 	if(control_mode == burst_control){
		std::cout<<"Enter halt time in seconds:\n"<<endl;
		std::cin>>halt_time;
		std::cout<<"Enter speed:\n"<<endl;
		std::cin>>power;
	} */
	while(1){
		count = 0;
		//count_nomovement =0;
		
		
		distance =get_distance(trig,echo);
		//change_in_distance = abs(last_distance - distance);
		//std::cout<<"moved "<<change_in_distance<<"cm"<<endl;
		std::cout<<"obstacle"<<distance<<"cm away!!"<<endl;

		power = get_power(distance);
		/*if(distance > 1300 ){
			return 1;
		}*/
		if(distance > 20 ){
			direction = GOforward;//getinput();
		}
		else{
			direction = halt;//move_bot(halt,power,gpio12,gpio16,gpio19,gpio26);
		}
		move_bot(direction,power,distance,gpio12,gpio16,gpio19,gpio26);
		do{
			change_direction =0;
			delay(100);
			
			last_distance = distance;
			last_direction = direction;
			power_prev = power;
			
			distance = get_distance(trig,echo);

			change_in_distance = abs(last_distance - distance);
			
			std::cout<<"moved "<<change_in_distance<<"cm"<<endl;
			std::cout<<"obstacle "<<distance<<"cm away!!"<<endl;
			
			
			if(distance < 20.0){
				move_bot(halt,gpio12,gpio16,gpio19,gpio26);
				change_direction =1;
				delay(1000);
			}
			if(change_in_distance == 0){
			    speed = 0.5*(get_speed(lsp)+get_speed(rsp));
				if(speed < 20.0){
					move_bot(halt,gpio12,gpio16,gpio19,gpio26);
					change_direction = 1;
					cout<<"I'm stuck :("<<endl;
				}	
			}
			
			//change power bsed on proximity while moving
			if( change_direction == 0){
				power = get_power(distance);
				direction = GOforward;
				move_bot(direction,power,distance,gpio12,gpio16,gpio19,gpio26);
			}
				
			if(change_direction == 1){
				if(last_distance < 20 ){
					delay(1000);
					look(lookright,(PyObject*)myFunction);
					delay(1000);
					right_distance = get_distance(trig,echo);
					delay(1000);
					look(lookleft,(PyObject*)myFunction);
					delay(1000);
					left_distance = get_distance(trig,echo);
					delay(1000);
					look(lookforward,(PyObject*)myFunction);
					if(left_distance >= right_distance){
						direction = GOleft;
					}
					else{
						direction = GOright;
					}
//					random_decision = rand()%2;
//					direction = GOright*random_decision + GOleft*(1 - random_decision);

					do{
						last_distance = distance;
						move_bot(direction,movement_time*second,400,gpio12,gpio16,gpio19,gpio26);
						move_bot(halt,gpio12,gpio16,gpio19,gpio26);
						delay(1000);
						distance =get_distance(trig,echo);
						change_in_distance = abs(last_distance - distance);
						if(change_in_distance ==0 && 0.5*(get_speed(lsp)+get_speed(rsp)) < 20.0){
							move_bot(GObackward,halt_time*second,450,gpio12,gpio16,gpio19,gpio26);
						}
					}while(distance < 20 || distance > 1300);
				}
				else{
					delay(1000);
					look(lookright,(PyObject*)myFunction);
					delay(1000);
					right_distance = get_distance(trig,echo);
					delay(1000);
					look(lookleft,(PyObject*)myFunction);
					delay(1000);
					left_distance = get_distance(trig,echo);
					delay(1000);
					look(lookforward,(PyObject*)myFunction);
					if(left_distance >= right_distance){
						direction = GOleft;
					}
					else{
						direction = GOright;
					}
//					random_decision = rand()%2;
//					direction = GOright*random_decision + GOleft*(1 - random_decision);
					move_bot(halt,gpio12,gpio16,gpio19,gpio26);
					power = 380;
					if(change_in_distance == 0){
						move_bot(GObackward,halt_time*second,power,gpio12,gpio16,gpio19,gpio26);
					}
					delay(1000);
					move_bot(direction,halt_time*second,400,gpio12,gpio16,gpio19,gpio26);
					delay(1000);
		
				}
			}	

			//count++;
			//if(change_in_distance == 0 && speed < 5.0){
			//	count_nomovement++;
			//}
		}while(change_direction == 0);
		
		
		if(ctrl_c_pressed){
			direction = halt;
			move_bot(direction,power,gpio12,gpio16,gpio19,gpio26);
			setup.cleanGPIO(gpio12,gpio19,gpio16,gpio26);
			//cleanGPIO(gpio5,gpio6);
     		break;
        }
	}
    cout << "Exiting....." << endl;
    Py_Finalize();
    return 0;
}

void sig_handler(int sig){
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
 }

