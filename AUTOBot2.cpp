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
	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt      = 10;
	const long int second         = 100000;
	getinput input;//gets direction and power
	cout<<"Initialize Pi"<<"\n";
	GOPI pi;
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

	power = 1024;	
	halt_time = 4.9;
	distance = 100.0;
	change_direction = 0;
	movement_time = 4.0;
   
//initiate variables and ports for speed encoder
	double speed;

	
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
		
	direction = 0;
	pi.look(lookforward);
	while(1){
		count = 0;
		//count_nomovement =0;
		
		
		distance =pi.get_distance();
		std::cout<<"obstacle"<<distance<<"cm away!!"<<endl;

		power = pi.get_power(distance);

		if(distance > 20 ){
			direction = GOforward;//getinput();
		}
		else{
			direction = halt;//move_bot(halt,power,gpio12,gpio16,gpio19,gpio26);
		}
		pi.move_bot(direction,power,distance);
		do{
			change_direction =0;
			delay(100);
			
			last_distance = distance;
			last_direction = direction;
			power_prev = power;
			
			distance =pi.get_distance();

			change_in_distance = abs(last_distance - distance);
			
			std::cout<<"moved "<<change_in_distance<<"cm"<<endl;
			std::cout<<"obstacle "<<distance<<"cm away!!"<<endl;
			
			
			if(distance < 20.0){
				pi.move_bot(halt);
				change_direction =1;
				delay(1000);
			}
			if(change_in_distance == 0){
			    speed = 0.5*(pi.left_wheel_speed()+pi.right_wheel_speed());
				if(speed < 20.0){
					pi.move_bot(halt);
					change_direction = 1;
					cout<<"I'm stuck :("<<endl;
				}	
			}
			
			//change power bsed on proximity while moving
			if( change_direction == 0){
				power = pi.get_power(distance);
				direction = GOforward;
				pi.move_bot(direction,power,distance);
			}
				
			if(change_direction == 1){
				if(last_distance < 20 ){
					delay(1000);
					pi.look(lookright);
					delay(1000);
					right_distance = pi.get_distance();
					delay(1000);
					pi.look(lookleft);
					delay(1000);
					left_distance = pi.get_distance();
					delay(1000);
					pi.look(lookforward);
					if(left_distance >= right_distance){
						direction = GOleft;
					}
					else{
						direction = GOright;
					}
//					random_decision = rand()%2;
//					direction = GOright*random_decision + GOleft*(1 - random_decision);

					do{
						power = 450;
						last_distance = distance;
						pi.move_bot(direction,movement_time*second,power);
						pi.move_bot(halt);
						delay(1000);
						distance =pi.get_distance();
						change_in_distance = abs(last_distance - distance);
						if(change_in_distance ==0 && 0.5*(pi.left_wheel_speed()+pi.right_wheel_speed()) < 20.0){
							pi.move_bot(GObackward,movement_time*second,500);
						}
					}while(distance < 20 || distance > 1300);
				}
				else{
					delay(1000);
					pi.look(lookright);
					delay(1000);
					right_distance = pi.get_distance();
					delay(1000);
					pi.look(lookleft);
					delay(1000);
					left_distance = pi.get_distance();
					delay(1000);
					pi.look(lookforward);
					if(left_distance >= right_distance){
						direction = GOleft;
					}
					else{
						direction = GOright;
					}
//					random_decision = rand()%2;
//					direction = GOright*random_decision + GOleft*(1 - random_decision);
					pi.move_bot(halt);
					power = 450;
					if(change_in_distance == 0){
						pi.move_bot(GObackward,halt_time*second,power);
					}
					delay(1000);
					pi.move_bot(direction,movement_time*second,power);
					delay(1000);
		
				}
			}	

			//count++;
			//if(change_in_distance == 0 && speed < 5.0){
			//	count_nomovement++;
			//}
		}while(change_direction == 0);
		
		
		if(ctrl_c_pressed){
			pi.move_bot(halt);
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

