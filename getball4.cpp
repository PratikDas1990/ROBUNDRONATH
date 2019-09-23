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

//Start and check wiringPi
   	if (wiringPiSetupGpio() == -1){
		std::cout<<"trouble with wiringPi"<<endl;
		exit (1) ;
	}
	
	int debug_mode;
	
	const int yes = 1;
	const int no = 0;

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

//Start servo set-up
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
        PyObject* myModuleString = PyUnicode_FromString("servo");
        PyObject* myModule = PyImport_Import(myModuleString);
	PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"moveservo");

//Variables for controlling the neck
	const int lookforward         = 90;
	const int lookleft1           = 115;
	const int lookleft2           = 140;
	const int lookleft3           = 165;
	const int lookright1          = 60;
	const int lookright2          = 30;
	const int lookright3          = 5;
	const int lookup              = 0;
	const int lookdown            = 180;
	int	look_x,look_y;
//Locate ball
	int ball_in_left;
	int ball_in_right;
	int ball_in_double_left;
	int ball_in_double_right;
	int ball_in_triple_left;
	int ball_in_triple_right;
	
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
	double ball[3];
	double x_cord;
	double y_cord;
	double radius;
//Set environment
	double* pointer_env;
	double env[6];
	
    cout<<"Run on debug mode?(yes =1 , no = 0)"<<"\n";
	cin>>debug_mode;
	
    if(debug_mode == yes){

	    pointer_env = set_env();
		/*std::cout<<"Hue:"<<env[0]<<" - "<<env[1]<<endl;
		std::cout<<"Saturation:"<<env[2]<<" - "<<env[3]<<endl;
		std::cout<<"Value:"<<env[4]<<" - "<<env[5]<<endl;*/
	}
	else{
		
		env[0]= 50.0;
		env[1]= 100.0;
		env[2]= 125.0;
		env[3]= 255.0;
		env[4]= 33.0;
		env[5]= 163.0;
	        pointer_env = env;	
	}
	struct sigaction sig_struct;
	sig_struct.sa_handler = sig_handler;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
		cout << "Problem with sigaction" << endl;
		exit(1);
	}
	

	setup.SetGPIOout(gpio12,gpio16,gpio19,gpio26);
	//SetGPIOpwm(gpio5,gpio6);
	
	direction = 0;
	look_x   = 90;
	look_y   = 110;
	look(look_x,look_y,(PyObject*)myFunction);;

	while(1){
		//count = 0;
		ball_in_left = 0;
		ball_in_right = 0;
		ball_in_double_left = 0;
		ball_in_double_right = 0;
		ball_in_triple_left = 0;
		ball_in_triple_right = 0;

		look(lookforward,look_y,(PyObject*)myFunction);


		if(ctrl_c_pressed){
			direction = halt;
			move_bot(direction,power,gpio12,gpio16,gpio19,gpio26);
			setup.cleanGPIO(gpio12,gpio19,gpio16,gpio26);
			//cleanGPIO(gpio5,gpio6);
     			break;
        	}

	//Find ball
		//count++;
		//usleep(1.0*second);
		//cout<<" bs &x_cord = "<<&x_cord<<"\n";
		//cout<<"bs &y_cord = "<<&y_cord<<"\n";
		//cout<<"bs &radius = "<<&radius<<"\n";

		locate_ball(pointer_env,&x_cord,&y_cord,&radius,gpio12,gpio16,gpio19,gpio26);
		//x_cord = ball[0];
		//y_cord = ball[1];
		//radius = ball[2];
		//cout<<"as &x_cord = "<<&x_cord<<"\n";
		//cout<<"as &y_cord = "<<&y_cord<<"\n";
		//cout<<"as &radius = "<<&radius<<"\n";
		//cout<<"x_cord = "<<x_cord<<"\n";
		//cout<<"y_cord = "<<y_cord<<"\n";
		//cout<<"radius = "<<radius<<"\n";
		if(radius == 0){
			move_bot(halt,gpio12,gpio16,gpio19,gpio26);
			look(lookright1,look_y,(PyObject*)myFunction);
			//usleep(1.0*second);
			locate_ball(pointer_env,&x_cord,&y_cord,&radius,gpio12,gpio16,gpio19,gpio26);
			if(radius != 0){
				ball_in_right = 1;
			}	
		}
		if(radius == 0){
			move_bot(halt,gpio12,gpio16,gpio19,gpio26);
			look(lookleft1,look_y,(PyObject*)myFunction);
			//usleep(1.0*second);
			locate_ball(pointer_env,&x_cord,&y_cord,&radius,gpio12,gpio16,gpio19,gpio26);
			if(radius != 0){
				ball_in_left = 1;
			}
		}
		if(radius == 0){
			move_bot(halt,gpio12,gpio16,gpio19,gpio26);
			look(lookright2,look_y,(PyObject*)myFunction);
			//usleep(1.0*second);
			locate_ball(pointer_env,&x_cord,&y_cord,&radius,gpio12,gpio16,gpio19,gpio26);
			if(radius != 0){
				ball_in_double_right = 1;
			}	
		}
		if(radius == 0){
			move_bot(halt,gpio12,gpio16,gpio19,gpio26);
			look(lookleft2,look_y,(PyObject*)myFunction);
			//usleep(1.0*second);
			locate_ball(pointer_env,&x_cord,&y_cord,&radius,gpio12,gpio16,gpio19,gpio26);
			if(radius != 0){
				ball_in_double_left = 1;
			}
		}
		if(radius == 0){
			move_bot(halt,gpio12,gpio16,gpio19,gpio26);
			look(lookright3,look_y,(PyObject*)myFunction);
			//usleep(1.0*second);
			locate_ball(pointer_env,&x_cord,&y_cord,&radius,gpio12,gpio16,gpio19,gpio26);
			if(radius != 0){
				ball_in_triple_right = 1;
			}	
		}
		if(radius == 0){
			move_bot(halt,gpio12,gpio16,gpio19,gpio26);
			look(lookleft3,look_y,(PyObject*)myFunction);
			//usleep(1.0*second);
			locate_ball(pointer_env,&x_cord,&y_cord,&radius,gpio12,gpio16,gpio19,gpio26);
			if(radius != 0){
				ball_in_triple_left = 1;
			}
		}
		if(ball_in_left == 1){
			cout<<"turn left"<<"\n";
			move_bot(GOleft,2.5*second,440,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		else if(ball_in_right == 1){
			cout<<"turn right"<<"\n";
			move_bot(GOright,2.5*second,440,gpio12,gpio16,gpio19,gpio26);
			continue;
		}

		else if(ball_in_double_left == 1){
			cout<<"turn left"<<"\n";
			move_bot(GOleft,3.5*second,440,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		else if(ball_in_double_right == 1){
			cout<<"turn right"<<"\n";
			move_bot(GOright,3.5*second,440,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		else if(ball_in_triple_left == 1){
			cout<<"turn left"<<"\n";
			move_bot(GOleft,4.5*second,440,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		else if(ball_in_triple_right == 1){
			cout<<"turn right"<<"\n";
			move_bot(GOright,4.5*second,440,gpio12,gpio16,gpio19,gpio26);
			continue;
		}
		if(radius <= 1){
			move_bot(halt,gpio12,gpio16,gpio19,gpio26);
			continue;			
		}
		//cout<<"x_cord"<<x_cord<<"\n";
		//cout<<"y_cord"<<y_cord<<"\n";
		//cout<<"radius"<<radius<<"\n";
		
		
		
		if(y_cord <  175*2&& y_cord> 120*2 && x_cord<220*2 && x_cord>100*2){
			move_bot(halt,400,gpio12,gpio16,gpio19,gpio26);
		}
		
		if(x_cord >  220*2 ){
			cout<<"turn right 2"<<"\n";
			move_bot(GOright,1*second,440,gpio12,gpio16,gpio19,gpio26);
			continue;

		}
		else if(x_cord < 100*2 ){
			cout<<"turn left 2"<<"\n";
			move_bot(GOleft,1*second,440,gpio12,gpio16,gpio19,gpio26);
			continue;

		}
		else{
			//cout<<"keep straight"<<"\n";
			move_bot(halt,400,gpio12,gpio16,gpio19,gpio26);

		}
		
		if(y_cord >  175*2 ){
			move_bot(GObackward,400,gpio12,gpio16,gpio19,gpio26);
			//continue;
		}
		else if(y_cord < 120*2 ){
			move_bot(GOforward,400,gpio12,gpio16,gpio19,gpio26);
			//continue;
		}
		else{
			move_bot(halt,400,gpio12,gpio16,gpio19,gpio26);
			//continue;
		}		

	}
    cout << "Exiting....." << endl;
    return 0;
}

void sig_handler(int sig){
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;

 }

