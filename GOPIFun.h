#ifndef GOPI_FUN_H
#define GOPI_FUN_H

#include <iostream>
#include "GPIOClass.h"
#include <Python.h>
using namespace std;
/* Bot movement functions
 */

class GOPI
{
private:
	GPIOClass* gpio12 = new GPIOClass("12");
	GPIOClass* gpio16 = new GPIOClass("16");
	GPIOClass* gpio19 = new GPIOClass("19");
	GPIOClass* gpio26 = new GPIOClass("26");
	//variables for ultrasound sensor
	int echo = 24;
	int trig = 23;
	//variables for the speed encoder
	const int lsp = 6;//lsp:left speed port 
	const int rsp = 5;//lsp:left speed port
	//variables for servo control
        PyObject* myModuleString;
        PyObject* myModule;
	PyObject* myFunction;

public:
	GOPI();
	void cleanGPIO();
	void move_bot(int direction);
	void move_bot(int direction,int power);
	void move_bot(int direction,int power,double distacne);
	void move_bot(int direction,long double halt_time,int power);
	double get_distance();
	double left_wheel_speed();
	double right_wheel_speed();
	int get_power(double distance);
	void look(int direction);
	void look(int x_direction,int y_direction);
};
 void move_bot(int direction,GPIOClass* gpio1,GPIOClass* gpio2,GPIOClass* gpio3,GPIOClass* gpio4);
 void move_bot(int direction,long double halt_time,int power,GPIOClass* gpio1,GPIOClass* gpio2,GPIOClass* gpio3,GPIOClass* gpio4);
 void move_bot(int direction,int power,GPIOClass* gpio1,GPIOClass* gpio2,GPIOClass* gpio3,GPIOClass* gpio4);
 void move_bot(int direction,int power,double distacne, GPIOClass* gpio1,GPIOClass* gpio2,GPIOClass* gpio3,GPIOClass* gpio4);
 void move_bot_to_get_ball(int direction,int power,GPIOClass* gpio1,GPIOClass* gpio2,GPIOClass* gpio3,GPIOClass* gpio4);
 void look(void);
 void look(int direction,PyObject* myFunction);
 void look(int x_direction,int y_direction,PyObject* myFunction);
 //void move_bot(int direction,long double halt_time,int power,GPIOClass* gpio12,GPIOClass* gpio16,GPIOClass* gpio19,GPIOClass* gpio26)
 double get_distance(int trig,int echo);
 int get_power(double distance);
 double get_speed(int port);
 int correct_direction(int power);
 
#endif
