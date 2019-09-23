
// A hello world program in C++

#include <iostream>
#include <Python.h>
using namespace std;

int main()
{
	cout << "Hello World!"<<"\n";
	Py_Initialize();
	PyRun_SimpleString("print('I am in python')");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")"); 
        PyRun_SimpleString("import testpy;print(testpy.plus(1,2))");
	PyObject* myModuleString = PyUnicode_FromString("testpy");
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
	PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"plus");
	PyObject* args = PyTuple_Pack(2,PyFloat_FromDouble(2.4),PyFloat_FromDouble(1.9));
	PyObject* myResult = PyObject_CallObject(myFunction, args);
	double result = PyFloat_AsDouble(myResult);
	cout << result<<"\n";
	Py_Finalize();
	return 0;
}
