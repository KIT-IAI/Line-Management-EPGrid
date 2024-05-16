#pragma once
#include "Visualizer.h"

#include <string>
#include <iostream>
#include "../model/gridElements/CurrentController.h"
#include "../Config.h"
#include <chrono>
#include <fstream>

int Visualizer::count = 0;
/** Constructor. **/
Visualizer::Visualizer()
{
	// Initializes the Python interpreter
	Py_Initialize();
	PyObject* sysPath = PySys_GetObject((char*)"path");
	PyObject* programName = PyUnicode_FromString("../LineManagerVisualization");
	PyList_Append(sysPath, programName);

	// Initialize the Python Interpreter
	//Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString("Visualization");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	PyObject* pDict = PyModule_GetDict(pModule);


	PyObject* pClass = PyDict_GetItemString(pDict, "GraphVisualizer");
	// Create an instance of the class
	pInstance = NULL;
	if (PyCallable_Check(pClass))
	{
		pInstance = PyObject_CallObject(pClass, NULL);
	}
}
/** Destructor **/
Visualizer::~Visualizer()
{
	// Clean up
	//Py_DECREF(pModule);
	//Py_DECREF(pName);

	// Finish the Python Interpreter
	//Py_Finalize();
}

/** Shows and saves a rendered plot.**/
void Visualizer::visualize()
{
	if (SAVE_PLOTS && PLOT_FOLDER != "")
	{
		//Save the plot
		//save(PLOT_FOLDER+"/"+std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))+"_"+std::to_string(count) + ".png");
		save(PLOT_FOLDER+"/eval" + std::to_string(count));

		//Save Parameters to generate plot
		std::ofstream file;
		file.open(PLOT_FOLDER + "/eval" + std::to_string(count) + ".txt");
		if (!file.is_open()) throw std::runtime_error("Could not open file.");
		if (!file.good()) throw std::runtime_error("Filestream not good.");
		file << callerString;
		file.close();

		//Increase plot count
		count++;
	}

	if (SHOW_PLOTS)
	{
		this->show();
	}
	close();
}

/** Saves a rendered plot.
* @param show path where the plot is saved.**/
void Visualizer::save(std::string path)
{
	PyObject_CallMethod(pInstance, "save", "(s)", path.c_str());
}
/** Shows a rendered plot.**/
void Visualizer::show()
{
	PyObject_CallMethodNoArgs(pInstance, PyUnicode_FromString("show"));
}

void Visualizer::close()
{
	PyObject_CallMethodNoArgs(pInstance, PyUnicode_FromString("close"));
}

