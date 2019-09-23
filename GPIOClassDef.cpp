#include <fstream>
//#include <string>
#include <iostream>
#include <sstream>
#include "GPIOClass.h"
#include <wiringPi.h>

using namespace std;

GPIOClass::GPIOClass()
{
    this->gpionum = "4"; //GPIO4 is default
}

GPIOClass::GPIOClass(string gnum)
{
    this->gpionum = gnum;  //Instatiate GPIOClass object for GPIO pin number "gnum"
}

int GPIOClass::export_gpio()
{
    string export_str = "/sys/class/gpio/export";
    ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    /*if (exportgpio << 0){
        cout << " OPERATION FAILED: Unable to export GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
    }*/

    exportgpio << this->gpionum ; //write GPIO number to export
    exportgpio.close(); //close export file
    return 0;
}

int GPIOClass::unexport_gpio()
{
    string unexport_str = "/sys/class/gpio/unexport";
    ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
    /*if (unexportgpio << 0){
        cout << " OPERATION FAILED: Unable to unexport GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
    }*/

    unexportgpio << this->gpionum ; //write GPIO number to unexport
    unexportgpio.close(); //close unexport file
    return 0;
}

int GPIOClass::setdir_gpio(string dir)
{

    string setdir_str ="/sys/class/gpio/gpio" + this->gpionum + "/direction";
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
        /*if (setdirgpio << 0){
            cout << " OPERATION FAILED: Unable to set direction of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }*/

        setdirgpio << dir; //write direction to direction file
        setdirgpio.close(); // close direction file
        return 0;
}

int GPIOClass::setval_gpio(string val)
{

    string setval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
        /*if (setvalgpio << 0){
            cout << " OPERATION FAILED: Unable to set the value of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }*/

        setvalgpio << val ;//write value to value file
        setvalgpio.close();// close value file
        return 0;
}

int GPIOClass::getval_gpio(string& val){

    string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    ifstream getvalgpio(getval_str.c_str());// open value file for gpio
    /*if (getvalgpio < 0){
        cout << " OPERATION FAILED: Unable to get value of GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
    }*/

    getvalgpio >> val ;  //read gpio value

    if(val != "0")
        val = "1";
    else
        val = "0";

    getvalgpio.close(); //close the value file
    return 0;
}

string GPIOClass::get_gpionum(){

return this->gpionum;

}

void GPIOSetup::SetGPIOout(GPIOClass* gpio)
{
    gpio->export_gpio();
	
    cout << " GPIO pins exported" << endl;

    gpio->setdir_gpio("out");   
    cout << " Set GPIO pin directions" << endl;
}
void GPIOSetup::cleanGPIO(GPIOClass* gpio1)
{
	cout << "App Stopped" << endl;
    cout << "unexporting pins" << endl;
	gpio1->unexport_gpio();
	cout << "deallocating GPIO Objects" << endl;
    delete gpio1;
    gpio1 = 0;	
	pwmWrite(13,0);
	pwmWrite(18,0);	
}
void GPIOSetup::SetGPIOout(GPIOClass* gpio12, GPIOClass* gpio16, GPIOClass* gpio19, GPIOClass* gpio26)
{
    gpio12->export_gpio();
    gpio16->export_gpio();
    gpio19->export_gpio();
    gpio26->export_gpio();
	
    cout << " GPIO pins exported" << endl;

    gpio12->setdir_gpio("out");
	gpio16->setdir_gpio("out");
    gpio19->setdir_gpio("out");
	gpio26->setdir_gpio("out");
    
    cout << " Set GPIO pin directions" << endl;
}
void GPIOSetup::SetGPIOpwm(GPIOClass* gpio5, GPIOClass* gpio6)
{
    gpio5->export_gpio();
    gpio6->export_gpio();
	
    cout << " GPIO pins exported for pwm" << endl;

    gpio5->setdir_gpio("out");
	gpio6->setdir_gpio("out");    
    cout << " Set GPIO pin directions for pwm" << endl;
}
void GPIOSetup::cleanGPIO(GPIOClass* gpio1, GPIOClass* gpio2, GPIOClass* gpio3, GPIOClass* gpio4)
{
	cout << "App Stopped" << endl;
    cout << "unexporting pins" << endl;
	gpio1->unexport_gpio();
	gpio2->unexport_gpio();
	gpio3->unexport_gpio();
	gpio4->unexport_gpio();
	cout << "deallocating GPIO Objects" << endl;
    delete gpio1;
    delete gpio2;
	delete gpio3;
	delete gpio4;
    gpio1 = 0;
	gpio2 = 0;
	gpio3 = 0;
	gpio4 = 0;
	
	pwmWrite(13,0);
	pwmWrite(18,0);	
}

void GPIOSetup::cleanGPIO(GPIOClass* gpio1, GPIOClass* gpio2)
{
	cout << "App Stopped" << endl;
    cout << "unexporting pins" << endl;
	gpio1->unexport_gpio();
	gpio2->unexport_gpio();

	cout << "deallocating GPIO Objects" << endl;
    delete gpio1;
    delete gpio2;
    gpio1 = 0;
	gpio2 = 0;
}



