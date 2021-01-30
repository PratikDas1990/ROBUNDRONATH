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

	
	const int yes = 1;
	const int no = 0;

//initiate viriables for direction control
	int direction;	
	const int GOforward           = 1;
	const int GOleft              = 2;
	const int GOright             = 3;
	const int GObackward          = 4;
	const int halt                = 10;
	const int continuous_control  = 1;
	const int burst_control       = 2;
	const long int second         = 100000;
	GOPI pi;


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
	
	int count = 0;
	int count_nomovement;
	long double halt_time,movement_time;

	halt_time = 4.9;
	movement_time = 1.2;

        direction = 0;
        look_x   = 90;
        look_y   = 110;
        pi.look(look_x,look_y);;
  
	vision ball;
	
	struct sigaction sig_struct;
	sig_struct.sa_handler = sig_handler;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
		cout << "Problem with sigaction" << endl;
		exit(1);
	}
	
	//SetGPIOpwm(gpio5,gpio6);
	

	while(1){
		//count = 0;
		ball_in_left = 0;
		ball_in_right = 0;
		ball_in_double_left = 0;
		ball_in_double_right = 0;
		ball_in_triple_left = 0;
		ball_in_triple_right = 0;

		pi.look(lookforward,look_y);

		if(ctrl_c_pressed){
			pi.move_bot(halt);
			pi.cleanGPIO();
			ball.clean();
     			break;
        	}

		ball.locate_ball(pi);
		//cout<<"as &x_cord = "<<&x_cord<<"\n";
		//cout<<"as &y_cord = "<<&y_cord<<"\n";
		//cout<<"as &radius = "<<&radius<<"\n";

		if( ball.radius == 0){
			pi.move_bot(halt);
			pi.look(lookright1,look_y);
			//usleep(1.0*second);
			 ball.locate_ball(pi);
			if( ball.radius != 0){
				ball_in_right = 1;
			}	
		}
		if( ball.radius == 0){
			pi.move_bot(halt);
			pi.look(lookleft1,look_y);
			//usleep(1.0*second);
			ball.locate_ball(pi);
			if( ball.radius != 0){
				ball_in_left = 1;
			}
		}
		if( ball.radius == 0){
			pi.move_bot(halt);
			pi.look(lookright2,look_y);
			//usleep(1.0*second);
			ball.locate_ball(pi);
			if( ball.radius != 0){
				ball_in_double_right = 1;
			}	
		}
		if( ball.radius == 0){
			pi.move_bot(halt);
			pi.look(lookleft2,look_y);
			//usleep(1.0*second);
			ball.locate_ball(pi);
			if( ball.radius != 0){
				ball_in_double_left = 1;
			}
		}
		if( ball.radius == 0){
			pi.move_bot(halt);
			pi.look(lookright3,look_y);
			//usleep(1.0*second);
			ball.locate_ball(pi);
			if( ball.radius != 0){
				ball_in_triple_right = 1;
			}	
		}
		if( ball.radius == 0){
			pi.move_bot(halt);
			pi.look(lookleft3,look_y);
			//usleep(1.0*second);
			ball.locate_ball(pi);
			if( ball.radius != 0){
				ball_in_triple_left = 1;
			}
		}

		cout<<"x_cord = "<< ball.x_cord<<"  ";
		cout<<"y_cord = "<< ball.y_cord<< "  ";
		cout<<"radius = "<< ball.radius<<"\n";

		if(ball_in_left == 1){
			cout<<"turn left"<<"\n";
			pi.move_bot(GOleft,2.0*second,460);
			//usleep(1.0*second);
			continue;
		}
		else if(ball_in_right == 1){
			cout<<"turn right"<<"\n";
			pi.move_bot(GOright,2.0*second,460);
			//usleep(1.0*second);
			continue;
		}

		else if(ball_in_double_left == 1){
			cout<<"turn double left"<<"\n";
			pi.move_bot(GOleft,2.5*second,460);
			//usleep(1.0*second);
			continue;
		}
		else if(ball_in_double_right == 1){
			cout<<"turn double right"<<"\n";
			pi.move_bot(GOright,2.5*second,460);
			usleep(1.0*second);
			continue;
		}
		else if(ball_in_triple_left == 1){
			cout<<"turn triple left"<<"\n";
			pi.move_bot(GOleft,3.2*second,460);
			//usleep(1.0*second);
			continue;
		}
		else if(ball_in_triple_right == 1){
			cout<<"turn triple right"<<"\n";
			pi.move_bot(GOright,3.2*second,460);
			//usleep(1.0*second);
			continue;
		}
		if( ball.radius <= 1){
			pi.move_bot(halt);
			continue;			
		}
		
		
		
		if( ball.y_cord <  175*2&&  ball.y_cord> 120*2 &&  ball.x_cord<220*2 &&  ball.x_cord>100*2){
			pi.move_bot(halt);
		}
		
		if( ball.x_cord >  220*2 ){
			cout<<"turn right 2"<<"\n";
			pi.move_bot(GOright,1*second,460);
			//usleep(1.0*second);
			continue;

		}
		else if( ball.x_cord < 100*2 ){
			cout<<"turn left 2"<<"\n";
			pi.move_bot(GOleft,1*second,460);
			//usleep(1.0*second);
			continue;

		}
		else{
			//cout<<"keep straight"<<"\n";
			pi.move_bot(halt,400);

		}
		
		if( ball.y_cord >  175*2 ){
			pi.move_bot(GObackward,2*second,460);
		}
		else if( ball.y_cord < 150*2 ){
			pi.move_bot(GOforward,420);
		}
		else{
			pi.move_bot(halt,400);
		}		

	}
    cout << "Exiting....." << endl;
    return 0;
}

void sig_handler(int sig){
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;

 }

