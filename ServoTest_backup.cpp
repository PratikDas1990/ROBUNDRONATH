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

//Start servo set-up
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
        PyObject* myModuleString = PyUnicode_FromString("servo");
        PyObject* myModule = PyImport_Import(myModuleString);
	PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"moveservo");

//initiate viriables for direction control	
	const int lookup              = 6;
	const int lookleft            = 7;
	const int lookright           = 9;
	const int lookdown            = 8;
	const int center              = 0;
	const int continuous_control  = 1;
	const int burst_control       = 2;
	const long int second         = 100000;
	int direction;
	int count_x;
	int count_y;

	struct sigaction sig_struct;
	sig_struct.sa_handler = sig_handler;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
		cout << "Problem with sigaction" << endl;
		exit(1);
	}
	count_x = 90;
	count_y = 90;
    	while(1)
    	{
		
		direction = getinput();
		if(count_x <= 0) count_x = 0;
		if(count_x >= 180) count_x = 180;
		if(count_y <= 0 )count_y = 0;
		if(count_y >= 180) count_y = 180;
		
        	switch(direction){
			case lookup:{
				cout<<"look up"<<endl;
				look(count_x,count_y=count_y-10,(PyObject*)myFunction);
				break;
			}
			case lookdown:{
				cout<<"look down"<<endl;
				look(count_x,count_y=count_y+10,(PyObject*)myFunction);
				break;
			}			
			case lookleft:{
				cout<<"look left"<<endl;
				look(count_x=count_x+10,count_y,(PyObject*)myFunction);
				break;
			}			
			case lookright:{
				cout<<"look right"<<endl;
				look(count_x=count_x-10,count_y,(PyObject*)myFunction);
				break;
			}
			case center:{
				cout<<"center"<<endl;
				look(90,90,(PyObject*)myFunction);
				count_x = 90;
				count_y = 90;
				break;
			}
			
		}
		
		if(direction !=10){
			continue;
		}
        

         if(ctrl_c_pressed || direction == 10){
			//cleanGPIO(gpio12,gpio19,gpio16,gpio26);
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

