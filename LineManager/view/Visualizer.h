#pragma once
#include <iostream>
#ifdef _DEBUG
//If debug mode enabled linker wants to use python dev libs which are not present on windows. Therefore disable debug flag when including python dependencies.
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

/** Abstract Class for visualization. This class mainly deals with the API connection to use python matplotlib for visualization. **/
class Visualizer
{
public:
	Visualizer();
	~Visualizer();
protected:
	void visualize();
	PyObject* pInstance;
	std::string callerString;

private:
	static int count;
	PyObject* pName;
	PyObject* pModule;
	void save(std::string path);
	void show();
	void close();
};