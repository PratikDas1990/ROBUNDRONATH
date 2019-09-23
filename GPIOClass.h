#ifndef GPIO_CLASS_H
#define GPIO_CLASS_H

//#include <string>
#include <iostream>
using namespace std;
/* GPIO Class
 * Purpose: Each object instantiated from this class will control a GPIO pin
 * The GPIO pin number must be passed to the overloaded class constructor
 */
class GPIOClass
{
public:
    GPIOClass();  // create a GPIO object that controls GPIO4 (default
    GPIOClass(string x); // create a GPIO object that controls GPIOx, where x is passed to this constructor
    int export_gpio(); // exports GPIO
    int unexport_gpio(); // unexport GPIO
    int setdir_gpio(string dir); // Set GPIO Direction
    int setval_gpio(string val); // Set GPIO Value (putput pins)
    int getval_gpio(string& val); // Get GPIO Value (input/ output pins)
    string get_gpionum(); // return the GPIO number associated with the instance of an object
private:
    string gpionum; // GPIO number associated with the instance of an object
};
class GPIOSetup
{
	
public:
	void SetGPIOout(GPIOClass* gpio1);
	void SetGPIOout(GPIOClass* gpio1,GPIOClass* gpio2,GPIOClass* gpio3,GPIOClass* gpio4);
	void SetGPIOpwm(GPIOClass* gpio1,GPIOClass* gpio2);
	void cleanGPIO(GPIOClass* gpio1);
	void cleanGPIO(GPIOClass* gpio1,GPIOClass* gpio2);
	void cleanGPIO(GPIOClass* gpio1,GPIOClass* gpio2,GPIOClass* gpio3,GPIOClass* gpio4);
};

#endif
