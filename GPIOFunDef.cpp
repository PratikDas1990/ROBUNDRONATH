#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include "GPIOClass.h"
#include "GPIOFun.h"
#include <wiringPi.h>
using namespace std;

int getinput::direction()
{
	//char dir;
	struct termios old_tio, new_tio;
    unsigned char dir;
	int led;
    //cout << "Which Direction? " <<endl;
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO,&old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio=old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &=(~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

 
    dir=getchar();
	if(dir == 'w'){
		led = 1;
	}
	else if(dir == 'a'){
		led = 2;
	}
	else if(dir == 'd'){
		led = 3;
	}
	else if(dir == 's'){
		led = 4;
	}
	else if(dir == 'q'){
		led = 10;
	}
	else if(dir == 'e'){
		led = 5;
	}
    else if(dir == 'i'){
		led = 6;
	}
    else if(dir == 'j'){
		led = 7;
	}
    else if(dir == 'k'){
		led = 8;
	}
	else if(dir == 'l'){
		led = 9;
	}
	else if(dir == 'c'){
		led = 11;
	}
	else if(dir=='m'){
		led = 12;
	}
	else {
		led = 0;
	}
    /* restore the former settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

    //return 0;
	return led;
}

int getinput::power()
{
	//char dir;
	struct termios old_tio, new_tio;
    unsigned char dir;
	int led;
	cout<<"Change speed"<<endl;
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO,&old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio=old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &=(~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

 
    dir=getchar();
	if(dir == '1'){
		led = 380;
	}
	else if(dir == '2'){
		led = 420;
	}
	else if(dir == '3'){
		led = 460;
	}
	else if(dir == '4'){
		led = 500;
	}
	else if(dir == '5'){
		led = 700;
	}
	else if(dir == '6'){
		led = 1024;
	}
	else {
		led = 0;
	}
	std::cout<<"Speed="<<led<<endl;
    /* restore the former settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

    //return 0;
	return led;
}
