
#ifndef __MML_WRAPPER_H__
#define __MML_WRAPPER_H__

#include <Python.h>

class MMLWrapper
{
private:
	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue;

public:
	MMLWrapper(const char *argv);
	~MMLWrapper();

	double run(char* data, int size);
};

#endif

