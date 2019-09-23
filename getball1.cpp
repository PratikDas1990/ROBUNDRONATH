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

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "vision.h"
using namespace std;

void sig_handler(int sig);
bool ctrl_c_pressed = false;

int main (void)
{
//	locate_ball();
//Hi

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
	int power_prev;
	int random_decision;
	string inputstate;
	double distance;
	double last_distance;
	int change_in_distance;
	int change_direction;
	int last_direction;
	int count = 0;
	int count_nomovement;
	long double halt_time,movement_time;

	pinMode(13, PWM_OUTPUT);
	pinMode(18, PWM_OUTPUT);
	power = 1024;	
	halt_time = 4.9;
	distance = 100.0;
	change_direction = 0;
	movement_time = 1.2;
   
//initiate variables and ports for speed encoder
	double speed;
	const int lsp = 6;//lsp:left speed port 
	const int rsp = 5;//lsp:left speed port
	pinMode(lsp, INPUT);
	pinMode(rsp, INPUT);

//initiate variables for vision
	double* ball;
	double x_cord;
	double y_cord;
	double radius;
//Set environment

	double* pointer_env = set_env();
	
	std::cout<<"Hue:"<<pointer_env[0]<<" - "<<pointer_env[1]<<endl;
	std::cout<<"Saturation:"<<pointer_env[2]<<" - "<<pointer_env[3]<<endl;
	std::cout<<"Value:"<<pointer_env[4]<<" - "<<pointer_env[5]<<endl;

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

	SetGPIOout(gpio12,gpio16,gpio19,gpio26);
	//SetGPIOpwm(gpio5,gpio6);
	
	direction = 0;
	//std::cout<<"Enter 1 for continuous control\n"<<"Enter 2 for burst control\n"<<endl;
	//std::cin>>control_mode;
	
	//if(control_mode == continuous_control){
	//	std::cout<<"Enter speed:\n"<<endl;
	//	std::cin>>power;
	//}
	//control_mode = burst_control;
 	//if(control_mode == burst_control){
	//	std::cout<<"Enter halt time in seconds:\n"<<endl;
	//	std::cin>>halt_time;
	//	std::cout<<"Enter speed:\n"<<endl;
	//	std::cin>>power;
	//} 
	while(1){
		count = 0;

		if(ctrl_c_pressed){
			direction = halt;
			move_bot(direction,power,gpio12,gpio16,gpio19,gpio26);
			cleanGPIO(gpio12,gpio19,gpio16,gpio26);
			//cleanGPIO(gpio5,gpio6);
     			break;
        	}

	//Find ball
		count++;
		ball = locate_ball(pointer_env);
		x_cord = ball[0];
		y_cord = ball[1];
		radius = ball[2];
		std::cout << "Ball position X = "<< ball[0]	// x position of center point of circle
				<<",\tY = "<< ball[1]			// y position of center point of circle
				<<",\tRadius = "<< ball[2]<< "\n";	// radius of circle
		while( radius == 0 && count<= 5){
			count++;
			ball = locate_ball(pointer_env);
			x_cord = ball[0];
			y_cord = ball[1];
			radius = ball[2];
			std::cout << "Ball position X = "<< ball[0]	// x position of center point of circle
				<<",\tY = "<< ball[1]			// y position of center point of circle
				<<",\tRadius = "<< ball[2]<< "\n";	// radius of circle
			//continue;
		}
		if( radius == 0 && count> 5){
			count++;
			move_bot(GObackward,4*second,400,gpio12,gpio16,gpio19,gpio26);
			//continue;
		}

		ball = locate_ball(pointer_env);
		x_cord = ball[0];
		y_cord = ball[1];
		radius = ball[2];
		while(radius == 0){
			count = 0;
			move_bot(GOright,3*second,400,gpio12,gpio16,gpio19,gpio26);
			//move_bot(GObackward,8*second,400,gpio12,gpio16,gpio19,gpio26);
			//move_bot(GOleft,4*second,400,gpio12,gpio16,gpio19,gpio26);	
			while (radius == 0 && count <=2){	
				ball = locate_ball(pointer_env);
				x_cord = ball[0];
				y_cord = ball[1];
				radius = ball[2];
				count++;
			}
		}
		//while((x_cord > 700 || x_cord < 400)&& radius != 0){
			if(radius < 50){
				if(x_cord <  200){
					move_bot(GOleft,1.5*second,400,gpio12,gpio16,gpio19,gpio26);
					continue;
				}
				else if(x_cord > 1000){
					move_bot(GOright,1.5*second,400,gpio12,gpio16,gpio19,gpio26);
					continue;
				}
				if(x_cord <  300){
					move_bot(GOleft,1*second,400,gpio12,gpio16,gpio19,gpio26);
					continue;
				}
				else if(x_cord > 800){
					move_bot(GOright,1*second,400,gpio12,gpio16,gpio19,gpio26);
					continue;

				}
			}
			if(radius >= 50){
				if(x_cord <  200){
					move_bot(GOleft,1*second,400,gpio12,gpio16,gpio19,gpio26);
					continue;
				}
				else if(x_cord > 1000){
					move_bot(GOright,1*second,400,gpio12,gpio16,gpio19,gpio26);
					continue;
				}
				if(x_cord <  400){
					move_bot(GOleft,0.6*second,400,gpio12,gpio16,gpio19,gpio26);
					continue;
				}
				else if(x_cord > 700){
					move_bot(GOright,0.6*second,400,gpio12,gpio16,gpio19,gpio26);
					continue;

				}
			}
		//	else{
		//		move_bot(GOright,0.6*second,400,gpio12,gpio16,gpio19,gpio26);
		//	}
		//	ball = locate_ball(pointer_env);	
		//	x_cord = ball[0];
		//	y_cord = ball[1];
		//	radius = ball[2];
		//}
		if(radius >150){
			move_bot(GOforward,4*movement_time*second,380,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		else if(radius >= 100 && radius <=150){
			move_bot(GOforward,4*movement_time*second,380,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		else if(radius <= 50){
			move_bot(GOforward,8*movement_time*second,450,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		else if(radius < 100 && radius >50){
			move_bot(GOforward,6*movement_time*second,380,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		//else{
		//	break;
		//}
		/*count = 0;
		//count_nomovement =0;
		
		
		distance =get_distance(trig,echo);
		//change_in_distance = abs(last_distance - distance);
		std::cout<<"moved "<<change_in_distance<<"cm"<<endl;
		std::cout<<"obstacle"<<distance<<"cm away!!"<<endl;

		power = get_power(distance);
		//if(distance > 1300 ){
		//	return 1;
		//}
		if(distance > 20 ){
			direction = GOforward;//getinput();
		}
		else{
			move_bot(halt,power,gpio12,gpio16,gpio19,gpio26);
		}
		move_bot(direction,power,distance,gpio12,gpio16,gpio19,gpio26);
		do{
			change_direction =0;
			delay(100);
			
			last_distance = distance;
			last_direction = direction;
			power_prev = power;
			
			distance =get_distance(trig,echo);

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
					change_direction = 1;
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
					random_decision = rand()%2;
					direction = GOright*random_decision + GOleft*(1 - random_decision);
					do{
						last_distance = distance;
						move_bot(direction,movement_time*second,400,gpio12,gpio16,gpio19,gpio26);
						move_bot(halt,gpio12,gpio16,gpio19,gpio26);
						delay(1000);
						distance =get_distance(trig,echo);
						change_in_distance = abs(last_distance - distance);
						if(change_in_distance ==0 && 0.5*(get_speed(lsp)+get_speed(rsp)) < 20.0){
							move_bot(GObackward,halt_time*second,400,gpio12,gpio16,gpio19,gpio26);
						}
					}while(distance < 20 || distance > 1300);
				}
				else{
					random_decision = rand()%2;
					direction = GOright*random_decision + GOleft*(1 - random_decision);
					move_bot(halt,gpio12,gpio16,gpio19,gpio26);
					power = 380;
					if(change_in_distance == 0){
						move_bot(GObackward,halt_time*second,power,gpio12,gpio16,gpio19,gpio26);
					}
					delay(1000);
					move_bot(direction,halt_time*second,power,gpio12,gpio16,gpio19,gpio26);
					delay(1000);
		
				}
			}	

			//count++;
			//if(change_in_distance == 0 && speed < 5.0){
			//	count_nomovement++;
			//}
		}while(change_direction == 0);
		*/
		

	}
    cout << "Exiting....." << endl;
    return 0;
}

void sig_handler(int sig){
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;

 }

