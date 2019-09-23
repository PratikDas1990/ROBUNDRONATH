#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <cmath>
#include "GPIOClass.h"
#include "GPIOFun.h"
#include "GOPIFun.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <Python.h>

using namespace std;

GOPI::GOPI()
{
	cout << " GPIO pins exported" << endl;
	gpio12->export_gpio();
	gpio16->export_gpio();
	gpio19->export_gpio();
	gpio26->export_gpio();
	cout << " Set GPIO pin directions" << endl;
	gpio12->setdir_gpio("out");
	gpio16->setdir_gpio("out");
	gpio19->setdir_gpio("out");
	gpio26->setdir_gpio("out");
	cout << " Set pin mode for PWM speed control" << endl;
	pinMode(13, PWM_OUTPUT);
	pinMode(18, PWM_OUTPUT);
	cout << "Set pin mode for ultrasound sensor" << endl;
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
	cout << "Set pin mode for speed measurement" << endl;
	pinMode(lsp, INPUT);
	pinMode(rsp, INPUT);
	cout << "Set-up python env. for servo control" << endl;
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
        myModuleString = PyUnicode_FromString("servo");
        myModule = PyImport_Import(myModuleString);
	myFunction = PyObject_GetAttrString(myModule,(char*)"moveservo");

}
void GOPI::cleanGPIO()
{
	cout << "App Stopped" << endl;
	cout << "unexporting pins" << endl;
	gpio12->unexport_gpio();
	gpio16->unexport_gpio();
	gpio19->unexport_gpio();
	gpio26->unexport_gpio();
	cout << "deallocating GPIO Objects" << endl;
	delete gpio12;
	delete gpio16;
	delete gpio19;
	delete gpio26;
	gpio12 = 0;
	gpio16 = 0;
	gpio19 = 0;
	gpio26 = 0;
	
	pwmWrite(13,0);
	pwmWrite(18,0);	

	Py_Finalize();
}
void GOPI::move_bot(int direction)
{

	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt    = 10;
	
	gpio12->setval_gpio("0");
	gpio19->setval_gpio("0");
	gpio16->setval_gpio("0");
	gpio26->setval_gpio("0");

	switch(direction){
        		
		case GOforward:{	 
	                cout << "FORWARD"<<endl;
	                gpio12->setval_gpio("1");
			gpio16->setval_gpio("1");
	                break;
		}
			 
		case GOleft:{			 
			cout << "LEFT"<<endl;
			gpio12->setval_gpio("1");
			gpio19->setval_gpio("1");
			break;
		}
		case GOright:{	 
			cout << "RIGHT"<<endl;
			gpio16->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
		}
		case GObackward:{
			cout << "BACKWARD"<<endl;
			gpio19->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
		}
		case halt:{
			gpio12->setval_gpio("0");
			gpio19->setval_gpio("0");
			gpio16->setval_gpio("0");
			gpio26->setval_gpio("0");
			break;
		}
		gpio12->setval_gpio("0");
		gpio19->setval_gpio("0");
		gpio16->setval_gpio("0");
		gpio26->setval_gpio("0");
	}
	return;
}
void GOPI::move_bot(int direction,int power)
{
	int err_correct_direction;
	int powerL,powerR;
	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt    = 10;

	powerR = power;
	powerL = power;//*(400 - 20)/400;
	pwmWrite(13,powerR);
	pwmWrite(18,powerL);	

	
	gpio12->setval_gpio("0");
	gpio19->setval_gpio("0");
	gpio16->setval_gpio("0");
	gpio26->setval_gpio("0");
	switch(direction){
        		
		case GOforward:{	 
	        cout << "FORWARD"<<endl;
        	gpio12->setval_gpio("1");
			gpio16->setval_gpio("1");
			/*err_correct_direction = correct_direction(power);
			if(err_correct_direction == 1){
				cout << "err_correct_direction"<<endl;
				gpio12->setval_gpio("0");
				gpio19->setval_gpio("0");
				gpio16->setval_gpio("0");
				gpio26->setval_gpio("0");
			}*/
        	break;
			}
		case GOleft:{			 
			cout << "LEFT"<<endl;
			gpio12->setval_gpio("1");
			gpio19->setval_gpio("1");
			break;
			}
		case GOright:{	 
			cout << "RIGHT"<<endl;
			gpio16->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
			}
		case GObackward:{
			cout << "BACKWARD"<<endl;
			gpio19->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
			}
		case halt:{
			gpio12->setval_gpio("0");
			gpio19->setval_gpio("0");
			gpio16->setval_gpio("0");
			gpio26->setval_gpio("0");
			break;
			}
		gpio12->setval_gpio("0");
		gpio19->setval_gpio("0");
		gpio16->setval_gpio("0");
		gpio26->setval_gpio("0");
		}
	return;
}
void GOPI::move_bot(int direction,long double halt_time,int power)
{
	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt    = 10;
	int powerL,powerR;
	powerL = power;
	powerR = power;
	//power2 = power;//*(400-20)/400;
	pwmWrite(13,powerR);
	pwmWrite(18,powerL);	
	
	switch(direction){
        		
		case GOforward:{	 
	                cout << "FORWARD"<<endl;
	                gpio12->setval_gpio("1");
			gpio16->setval_gpio("1");
			usleep(halt_time);
			break;
		}
			 
		case GOleft:{			 
	                cout << "LEFT"<<endl;
	                gpio12->setval_gpio("1");
			gpio19->setval_gpio("1");
			usleep(halt_time);
			break;
		}
		case GOright:{	 
	                cout << "RIGHT"<<endl;
	                gpio16->setval_gpio("1");
			gpio26->setval_gpio("1");
			usleep(halt_time);
			break;
		}
		case GObackward:{
			cout << "BACKWARD"<<endl;
			gpio19->setval_gpio("1");
			gpio26->setval_gpio("1");
			usleep(halt_time);
			break;
		}
		case halt:{
			gpio12->setval_gpio("0");
			gpio19->setval_gpio("0");
			gpio16->setval_gpio("0");
			gpio26->setval_gpio("0");
			break;
		}
		gpio12->setval_gpio("0");
		gpio19->setval_gpio("0");
		gpio16->setval_gpio("0");
		gpio26->setval_gpio("0");
	}
	gpio12->setval_gpio("0");
	gpio19->setval_gpio("0");
	gpio16->setval_gpio("0");
	gpio26->setval_gpio("0");		
	return;
}
void GOPI::move_bot(int direction,int power,double distance)
{
	int err_correct_direction;
	int powerL,powerR;
	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt    = 10;
	powerR = power;
	powerL = power;//*(400 - 20)/400;
	pwmWrite(13,powerR);
	pwmWrite(18,powerL);	

	
	gpio12->setval_gpio("0");
	gpio19->setval_gpio("0");
	gpio16->setval_gpio("0");
	gpio26->setval_gpio("0");
	switch(direction){
        		
		case GOforward:{	 
			cout << "FORWARD"<<endl;
        	gpio12->setval_gpio("1");
			gpio16->setval_gpio("1");
			if(distance >= 100){
				err_correct_direction = correct_direction(power);
				if(err_correct_direction == 1){
					cout << "err_correct_direction"<<endl;
					gpio12->setval_gpio("0");
					gpio19->setval_gpio("0");
					gpio16->setval_gpio("0");
					gpio26->setval_gpio("0");
				}
			}
        	break;
		}
		case GOleft:{			 
			cout << "LEFT"<<endl;
			gpio12->setval_gpio("1");
			gpio19->setval_gpio("1");
			break;
		}
		case GOright:{	 
			cout << "RIGHT"<<endl;
			gpio16->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
		}
		case GObackward:{
			cout << "BACKWARD"<<endl;
			gpio19->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
		}
		case halt:{
			gpio12->setval_gpio("0");
			gpio19->setval_gpio("0");
			gpio16->setval_gpio("0");
			gpio26->setval_gpio("0");
			break;
		}
		gpio12->setval_gpio("0");
		gpio19->setval_gpio("0");
		gpio16->setval_gpio("0");
		gpio26->setval_gpio("0");
	}
	return;
}
double GOPI::get_distance()
{
	int pulse_start;
	int pulse_end;
	int pulse_duration;
	long double distance;
	
	pulse_start = 0;
	pulse_end =0;
	pulse_duration =0;
	distance = 0;
	//std::cout<<"Tim:"<<micros()<<endl;
	digitalWrite(trig,0);
	//delay(2000);

	digitalWrite(trig,1);
	delayMicroseconds(10);
	digitalWrite(trig,0);

	while(digitalRead(echo) == 0){
		pulse_start = micros();
	}
	//std::cout<<"Strats receiving signal"<<endl;
	while(digitalRead(echo) == 1){
		pulse_end = micros();
	}
	//std::cout<<"Stops receiving signal"<<endl;
	pulse_duration = pulse_end - pulse_start;
	
	//std::cout<<"pulse_duration:"<<pulse_duration<<endl;
	
	distance = pulse_duration*0.0165;
	
	//std::cout<<"distance = "<<distance<<"cm"<<endl;
	//std::cout<<"Tim:"<<micros()<<endl;

	return distance;
}

double GOPI::left_wheel_speed()
{
	int port = lsp;
	double speed;
	int stat_curr,stat_old;
	int time,time_start,time_end,time_inter;
	int count;

	count = 0;
	time = 0;
	time_start = 0;
	time_end = 0;
	speed = 1;

	//std::cout<<"port ::"<<port<<endl;
	//std::cout<<"count ::"<<count<<endl;
	stat_old = digitalRead(port);
	time_start = micros();
	while(count <= 10){ 
		time_inter = micros();
		stat_curr = digitalRead(port);
		if(stat_old != stat_curr){
			count++;
			//std::cout<<"count ::"<<count<<endl;
			stat_old = digitalRead(port);
		}
		else if((time_inter - time_start)>1000000){
			speed = 0;
			break;
		}
        }
        time_end = micros();
	time = time_end - time_start;

        speed = speed*0.25*19949113.35/time;
        //std::cout<<"time ::"<<time<<endl;
	return speed;
}
double GOPI::right_wheel_speed()
{
	int port = rsp;
	double speed;
	int stat_curr,stat_old;
	int time,time_start,time_end,time_inter;
	int count;

	count = 0;
	time = 0;
	time_start = 0;
	time_end = 0;
	speed = 1;

	//std::cout<<"port ::"<<port<<endl;
	//std::cout<<"count ::"<<count<<endl;
	stat_old = digitalRead(port);
	time_start = micros();
	while(count <= 10){ 
		time_inter = micros();
		stat_curr = digitalRead(port);
		if(stat_old != stat_curr){
			count++;
			//std::cout<<"count ::"<<count<<endl;
			stat_old = digitalRead(port);
		}
		else if((time_inter - time_start)>1000000){
			speed = 0;
			break;
		}
        }
        time_end = micros();
	time = time_end - time_start;

        speed = speed*0.25*19949113.35/time;
        //std::cout<<"time ::"<<time<<endl;
	return speed;
}

int GOPI::get_power(double distance){
	int power;
	if(distance >= 700){
		power = 380;
	}	
	if(distance >= 100.0 && distance<700){
		power = 450;
	}
	else if(distance >= 70.0 && distance < 100.0){
		power = 420;
	}
	else if(distance >= 50.0 && distance < 70.0){
		power = 400;
	}
	else if(distance >= 30 && distance < 50.0){
		power = 380;
	}
	else if(distance >= 10 && distance < 30.0){
		power = 370;
	}
	else if(distance  < 10.0){
		power = 370;
	}
	std::cout<<"Power ::"<<power<<endl;
	return power;
}
void GOPI::look(int direction)
{
	double angle_x;
        angle_x = (double)direction;
        PyObject* args = PyTuple_Pack(2,PyFloat_FromDouble(angle_x),PyFloat_FromDouble(90.0));
        PyObject* myResult = PyObject_CallObject(myFunction, args);
	//softPwmWrite (x_axis,direction ) ;//x//max 22,min 7, mean 15
	//softPwmWrite (20,15 ) 
	return;
}
void GOPI::look(int x_direction,int y_direction)
{

	if(x_direction <= 0) x_direction = 0;
	if(x_direction >= 180) x_direction = 180;
	if(y_direction <=0) y_direction = 0;
	if(y_direction >= 180) y_direction = 180;

        double angle_x,angle_y;
        angle_x = (double)x_direction;
	angle_y = (double)y_direction;

        PyObject* args = PyTuple_Pack(2,PyFloat_FromDouble(angle_x),PyFloat_FromDouble(angle_y));
        PyObject* myResult = PyObject_CallObject(myFunction, args);
        //Py_Finalize();
        return;
}









void move_bot(int direction,GPIOClass* gpio12,GPIOClass* gpio16,GPIOClass* gpio19,GPIOClass* gpio26)
{
	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt    = 10;
	
	gpio12->setval_gpio("0");
	gpio19->setval_gpio("0");
	gpio16->setval_gpio("0");
	gpio26->setval_gpio("0");

	switch(direction){
        		
		case GOforward:{	 
	                cout << "FORWARD"<<endl;
	                gpio12->setval_gpio("1");
			gpio16->setval_gpio("1");
	                break;
		}
			 
		case GOleft:{			 
			cout << "LEFT"<<endl;
			gpio12->setval_gpio("1");
			gpio19->setval_gpio("1");
			break;
		}
		case GOright:{	 
			cout << "RIGHT"<<endl;
			gpio16->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
		}
		case GObackward:{
			cout << "BACKWARD"<<endl;
			gpio19->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
		}
		case halt:{
			gpio12->setval_gpio("0");
			gpio19->setval_gpio("0");
			gpio16->setval_gpio("0");
			gpio26->setval_gpio("0");
			break;
		}
		gpio12->setval_gpio("0");
		gpio19->setval_gpio("0");
		gpio16->setval_gpio("0");
		gpio26->setval_gpio("0");
	}
	return;
}
void move_bot(int direction,long double halt_time,int power,GPIOClass* gpio12,GPIOClass* gpio16,GPIOClass* gpio19,GPIOClass* gpio26)
{
	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt    = 10;
	int power2;
	power2 = power*(400-20)/400;
	pwmWrite(13,power);
	pwmWrite(18,power2);	
	
	switch(direction){
        		
		case GOforward:{	 
	                cout << "FORWARD"<<endl;
	                gpio12->setval_gpio("1");
			gpio16->setval_gpio("1");
			usleep(halt_time);
			break;
		}
			 
		case GOleft:{			 
	                cout << "LEFT"<<endl;
	                gpio12->setval_gpio("1");
			gpio19->setval_gpio("1");
			usleep(halt_time);
			break;
		}
		case GOright:{	 
	                cout << "RIGHT"<<endl;
	                gpio16->setval_gpio("1");
			gpio26->setval_gpio("1");
			usleep(halt_time);
			break;
		}
		case GObackward:{
			cout << "BACKWARD"<<endl;
			gpio19->setval_gpio("1");
			gpio26->setval_gpio("1");
			usleep(halt_time);
			break;
		}
		case halt:{
			gpio12->setval_gpio("0");
			gpio19->setval_gpio("0");
			gpio16->setval_gpio("0");
			gpio26->setval_gpio("0");
			break;
		}
		gpio12->setval_gpio("0");
		gpio19->setval_gpio("0");
		gpio16->setval_gpio("0");
		gpio26->setval_gpio("0");
	}
	gpio12->setval_gpio("0");
	gpio19->setval_gpio("0");
	gpio16->setval_gpio("0");
	gpio26->setval_gpio("0");		
	return;
}
void move_bot(int direction,int power,GPIOClass* gpio12,GPIOClass* gpio16,GPIOClass* gpio19,GPIOClass* gpio26)
{
	int err_correct_direction;
	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt      = 10;
	int powerL,powerR;

	powerR = power;
	powerL = power;//*(400 - 20)/400;
	pwmWrite(13,powerR);
	pwmWrite(18,powerL);	

	
	gpio12->setval_gpio("0");
	gpio19->setval_gpio("0");
	gpio16->setval_gpio("0");
	gpio26->setval_gpio("0");
	switch(direction){
        		
		case GOforward:{	 
	        cout << "FORWARD"<<endl;
        	gpio12->setval_gpio("1");
			gpio16->setval_gpio("1");
			/*err_correct_direction = correct_direction(power);
			if(err_correct_direction == 1){
				cout << "err_correct_direction"<<endl;
				gpio12->setval_gpio("0");
				gpio19->setval_gpio("0");
				gpio16->setval_gpio("0");
				gpio26->setval_gpio("0");
			}*/
        	break;
			}
		case GOleft:{			 
			cout << "LEFT"<<endl;
			gpio12->setval_gpio("1");
			gpio19->setval_gpio("1");
			break;
			}
		case GOright:{	 
			cout << "RIGHT"<<endl;
			gpio16->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
			}
		case GObackward:{
			cout << "BACKWARD"<<endl;
			gpio19->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
			}
		case halt:{
			gpio12->setval_gpio("0");
			gpio19->setval_gpio("0");
			gpio16->setval_gpio("0");
			gpio26->setval_gpio("0");
			break;
			}
		gpio12->setval_gpio("0");
		gpio19->setval_gpio("0");
		gpio16->setval_gpio("0");
		gpio26->setval_gpio("0");
		}
	return;
}


void move_bot(int direction,int power,double distance, GPIOClass* gpio12,GPIOClass* gpio16,GPIOClass* gpio19,GPIOClass* gpio26)
{
	int err_correct_direction;
	const int GOforward = 1;
	const int GOleft    = 2;
	const int GOright   = 3;
	const int GObackward= 4;
	const int halt      = 10;
	int powerL,powerR;

	powerR = power;
	powerL = power;//*(400 - 20)/400;
	pwmWrite(13,powerR);
	pwmWrite(18,powerL);	

	
	gpio12->setval_gpio("0");
	gpio19->setval_gpio("0");
	gpio16->setval_gpio("0");
	gpio26->setval_gpio("0");
	switch(direction){
        		
		case GOforward:{	 
			cout << "FORWARD"<<endl;
        	gpio12->setval_gpio("1");
			gpio16->setval_gpio("1");
			if(distance >= 100){
				err_correct_direction = correct_direction(power);
				if(err_correct_direction == 1){
					cout << "err_correct_direction"<<endl;
					gpio12->setval_gpio("0");
					gpio19->setval_gpio("0");
					gpio16->setval_gpio("0");
					gpio26->setval_gpio("0");
				}
			}
        	break;
		}
		case GOleft:{			 
			cout << "LEFT"<<endl;
			gpio12->setval_gpio("1");
			gpio19->setval_gpio("1");
			break;
		}
		case GOright:{	 
			cout << "RIGHT"<<endl;
			gpio16->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
		}
		case GObackward:{
			cout << "BACKWARD"<<endl;
			gpio19->setval_gpio("1");
			gpio26->setval_gpio("1");
			break;
		}
		case halt:{
			gpio12->setval_gpio("0");
			gpio19->setval_gpio("0");
			gpio16->setval_gpio("0");
			gpio26->setval_gpio("0");
			break;
		}
		gpio12->setval_gpio("0");
		gpio19->setval_gpio("0");
		gpio16->setval_gpio("0");
		gpio26->setval_gpio("0");
	}
	return;
}

double get_distance(int trig, int echo)
{
	int pulse_start;
	int pulse_end;
	int pulse_duration;
	long double distance;
	
	pulse_start = 0;
	pulse_end =0;
	pulse_duration =0;
	distance = 0;
	//std::cout<<"Tim:"<<micros()<<endl;
	digitalWrite(trig,0);
	//delay(2000);

	digitalWrite(trig,1);
	delayMicroseconds(10);
	digitalWrite(trig,0);

	while(digitalRead(echo) == 0){
		pulse_start = micros();
	}
	//std::cout<<"Strats receiving signal"<<endl;
	while(digitalRead(echo) == 1){
		pulse_end = micros();
	}
	//std::cout<<"Stops receiving signal"<<endl;
	pulse_duration = pulse_end - pulse_start;
	
	//std::cout<<"pulse_duration:"<<pulse_duration<<endl;
	
	distance = pulse_duration*0.0165;
	
	//std::cout<<"distance = "<<distance<<"cm"<<endl;
	//std::cout<<"Tim:"<<micros()<<endl;

	return distance;
}

int get_power(double distance){
	int power;
	if(distance >= 700){
		power = 380;
	}	
	if(distance >= 100.0 && distance<700){
		power = 450;
	}
	else if(distance >= 70.0 && distance < 100.0){
		power = 420;
	}
	else if(distance >= 50.0 && distance < 70.0){
		power = 400;
	}
	else if(distance >= 30 && distance < 50.0){
		power = 380;
	}
	else if(distance >= 10 && distance < 30.0){
		power = 370;
	}
	else if(distance  < 10.0){
		power = 370;
	}
	std::cout<<"Power ::"<<power<<endl;
	return power;
}

double get_speed(int port)
{
	double speed;
	int stat_curr,stat_old;
	int time,time_start,time_end,time_inter;
	int count;

	count = 0;
	time = 0;
	time_start = 0;
	time_end = 0;
	speed = 1;

	//std::cout<<"port ::"<<port<<endl;
	//std::cout<<"count ::"<<count<<endl;
	stat_old = digitalRead(port);
	time_start = micros();
	while(count <= 10){ 
		time_inter = micros();
		stat_curr = digitalRead(port);
		if(stat_old != stat_curr){
			count++;
			//std::cout<<"count ::"<<count<<endl;
			stat_old = digitalRead(port);
		}
		else if((time_inter - time_start)>1000000){
			speed = 0;
			break;
		}
        }
        time_end = micros();
	time = time_end - time_start;

        speed = speed*0.25*19949113.35/time;
        //std::cout<<"time ::"<<time<<endl;
	return speed;
}

int correct_direction(int power)
{
	int count;
	const int lsp = 6;//lsp:left speed port 
	const int rsp = 5;//rsp:right speed port
	double right_speed,left_speed,diff_speed,diff_speed_initial;	
	int powerL,powerR;

	count = 0;
        powerR = power;
	powerL = power;//*(400 - 20)/400;
	right_speed = get_speed(rsp);
	left_speed  = get_speed(lsp);
	diff_speed_initial = 2*(right_speed - left_speed)/(right_speed + left_speed);
	diff_speed = diff_speed_initial;
        cout << "right_speed="<<right_speed<<",left_speed="<<left_speed<<endl;
        while(std::abs(2*diff_speed/(right_speed + left_speed)) > 0.025 ){
		right_speed = get_speed(rsp);
		left_speed  = get_speed(lsp);
                cout << "right_speed="<<right_speed<<",left_speed="<<left_speed<<endl;
		diff_speed = (right_speed - left_speed);//2*(right_speed - left_speed)/(right_speed + left_speed);
		if(diff_speed < 0){
			powerR = powerR - diff_speed;
			//powerR = powerR + 5;
		}
		else if(diff_speed > 0){
			powerL = powerL - diff_speed;
			//powerL = powerL + 5;
		}
		else if(std::abs(diff_speed) > std::abs(diff_speed_initial) ){
			return 1;
		}	
		if(right_speed == 0 || left_speed ==0){
			return 1;
		}	
		pwmWrite(13,powerR);
		pwmWrite(18,powerL);
                count++;
		cout<<"count:"<<count<<endl;
		if(count >7){
			return 1;
		}
		delay(500);
	}
	return 0;
}

void look(int direction,PyObject* myFunction)
{
	double angle_x;
        angle_x = (double)direction;
        PyObject* args = PyTuple_Pack(2,PyFloat_FromDouble(angle_x),PyFloat_FromDouble(90.0));
        PyObject* myResult = PyObject_CallObject(myFunction, args);
	//softPwmWrite (x_axis,direction ) ;//x//max 22,min 7, mean 15
	//softPwmWrite (20,15 ) 
	return;
}
void look(int x_direction,int y_direction,PyObject* myFunction)
{

	if(x_direction <= 0) x_direction = 0;
	if(x_direction >= 180) x_direction = 180;
	if(y_direction <=0) y_direction = 0;
	if(y_direction >= 180) y_direction = 180;

        double angle_x,angle_y;
        angle_x = (double)x_direction;
	angle_y = (double)y_direction;

        PyObject* args = PyTuple_Pack(2,PyFloat_FromDouble(angle_x),PyFloat_FromDouble(angle_y));
        PyObject* myResult = PyObject_CallObject(myFunction, args);
        //Py_Finalize();
        return;
}

void look (void)
{
	void sig_handler(int sig);
	bool ctrl_c_pressed = false;

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
	const int center              = 12;
	const int continuous_control  = 1;
	const int burst_control       = 2;
	const long int second         = 100000;
	getinput input;
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
    	while(direction = input.direction())
    	{
		
		//direction = getinput();
		if(count_x <= 0) count_x = 0;
		if(count_x >= 180) count_x = 180;
		if(count_y <= 0 )count_y = 0;
		if(count_y >= 180) count_y = 180;
		//cout<<direction<<"/n";
        	switch(direction){
			case lookup:{
				//cout<<"look up"<<endl;
				count_y=count_y-10;
				break;
			}
			case lookdown:{
				//cout<<"look down"<<endl;
				count_y=count_y+10;
				break;
			}			
			case lookleft:{
				//cout<<"look left"<<endl;
				count_x=count_x+10;
				break;
			}			
			case lookright:{
				//cout<<"look right"<<endl;
				count_x=count_x-10;
				break;
			}
			case center:{
				cout<<"center"<<endl;
				count_x = 90;
				count_y = 90;
				break;
			}
			
		}
		look(count_x,count_y,(PyObject*)myFunction);
		if(direction !=10){
			continue;
		}
        

         	if(ctrl_c_pressed || direction == 10){
     			break;
        	}

	}
	cout << "Exiting....." << endl;
	Py_Finalize();
	return;	
}
