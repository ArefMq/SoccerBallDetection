
#include "mmlwrapper.h"

MMLWrapper::MMLWrapper(const char *argv)
{
	Py_SetProgramName((char*)argv);
	Py_Initialize();
	PySys_SetArgv(1, (char**)&argv);

	pName = PyString_FromString("mmlcore");

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule == NULL)
		throw "can not find module...";

	pFunc = PyObject_GetAttrString(pModule, "run");
	if (!pFunc || !PyCallable_Check(pFunc))
		throw "can not find a callable function...";

	PyObject* pInitFunc = PyObject_GetAttrString(pModule, "init");
	if (!pInitFunc || !PyCallable_Check(pInitFunc))
		throw "can not find a callable init function...";

	PyObject_CallObject(pInitFunc, NULL);
	Py_XDECREF(pInitFunc);
}

MMLWrapper::~MMLWrapper()
{
	Py_XDECREF(pFunc);
	Py_DECREF(pModule);
	Py_Finalize();
}

double MMLWrapper::run(char* data, int size)
{
	pArgs = PyTuple_New(1);
	pValue = PyString_FromStringAndSize(data, size);
	PyTuple_SetItem(pArgs, 0, pValue);

	pValue = PyObject_CallObject(pFunc, pArgs);
	Py_DECREF(pArgs);

	if (pValue == NULL)
		throw "no value retured...";

	double result = PyFloat_AsDouble(pValue);
	Py_DECREF(pValue);
	return result;
}
