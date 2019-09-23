
// Test servo control

#include <iostream>
#include <Python.h>
using namespace std;

int main()
{
	double angle_x,angle_y;
	cout << "Testing servo!"<<"\n";
	cout << "Enter angle(x-axis)= ";
	cin >> angle_x;
        cout << "Enter angle(y-axis)= ";
	cin >> angle_y;
	Py_Initialize();
	PyRun_SimpleString("print('I am in python')");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")"); 
	PyObject* myModuleString = PyUnicode_FromString("servo");
	cout<<myModuleString<<"\n";
	PyObject* myModule = PyImport_Import(myModuleString);
	if(myModule)
	{
		cout<<"Module available"<<"\n";
	}
	else
	{
		cout<<"Module not loaded"<<"\n";
		return 0;
	}
	PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"moveservo");
	PyObject* args = PyTuple_Pack(2,PyFloat_FromDouble(angle_x),PyFloat_FromDouble(angle_y));
	PyObject* myResult = PyObject_CallObject(myFunction, args);
	double result = PyFloat_AsDouble(myResult);
	cout << result<<"\n";
	Py_Finalize();
	return 0;
}
