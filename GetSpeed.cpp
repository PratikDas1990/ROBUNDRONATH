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
    double speed;
    int port ;

   	if (wiringPiSetupGpio() == -1){
		std::cout<<"trouble with wiringPi"<<endl;
		exit (1) ;
	}
    port = 6;
    pinMode(port, INPUT);
    speed = get_speed(port);
    cout << "speed:Left="<< speed << endl;
    port = 5;
    pinMode(port, INPUT);
    speed = get_speed(port);
    cout << "speed:Right="<< speed << endl;

    return 0;
}

void sig_handler(int sig){
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
 }

