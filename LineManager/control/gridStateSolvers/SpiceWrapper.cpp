#include "SpiceWrapper.h"
#include "../../Config.h"
#include <iostream>
#include <filesystem>

int SpiceWrapper::count = 0;
/** Constructor. **/
SpiceWrapper::SpiceWrapper()
{
	std::string libPath = "SpiceLib/ngspice" + std::to_string(count) + ".dll";
	count++;

	try {
		std::filesystem::copy_file("SpiceLib/ngspice.dll", libPath);
	}
	catch (std::filesystem::filesystem_error& e) {
		if (e.code().value() != 80)//if error anything else than file already exists
		{
			std::cout << e.what() << std::endl;
			exit(1);
		}
	}

	std::wstring temp = std::wstring(libPath.begin(), libPath.end());
	hinstLib = LoadLibrary(temp.c_str());// Get a handle to the DLL module
	if (hinstLib == NULL)//check if lib handle is valid
	{
		std::cerr << "Failed loading lib" << std::endl;
		exit(1);
	}

	SPICEINIT spiceInitFunc = (SPICEINIT)GetProcAddress(hinstLib, "ngSpice_Init");
	if (NULL == spiceInitFunc)// If the function address is invalid
	{
		std::cerr << "Failed loading lib" << std::endl;
		exit(1);
	}
	(spiceInitFunc)(SpiceWrapper::sendChar, SpiceWrapper::sendStat, SpiceWrapper::controlledExit, SpiceWrapper::sendData, SpiceWrapper::sendInitData, NULL, this);

	spiceCommandFunc = (SPICECOMMAND)GetProcAddress(hinstLib, "ngSpice_Command");
	if (NULL == spiceCommandFunc)
	{
		std::cerr << "Failed loading lib" << std::endl;
		exit(1);
	}
	//fFreeResult = FreeLibrary(hinstLib);
}
/** Destructor. **/
SpiceWrapper::~SpiceWrapper()
{
	bool fFreeResult = FreeLibrary(hinstLib);
}

/** Changes the spice context to a new circuit model.
* @param name the name of the circuit.
**/
void SpiceWrapper::newCircuit(std::string name)
{
	spiceCommand("circbyline " + name);
}
/** Adds a line to the spice circuit.
* @param v1id the id of the start node of the line.
* @param v2id the id of the end node of the line.
* @param resistance the resistance.**/
void SpiceWrapper::addLine(std::string v1id, std::string v2id, double resistance)
{
	std::string v1idString = "n" + v1id;
	std::string v2idString = "n" + v2id;
	std::cout.precision(20);
	std::string resistanceString = std::to_string(resistance);
	spiceCommand("circbyline R" + v1idString + ":" + v2idString + " " + v1idString + " " + v2idString + " " + resistanceString);
}
/** Adds a voltage controller to the spice circuit.
* @param id the id of the voltage controller.
* @param v1id the id of the node at the plus connection of the controller.
* @param v2id the id of the node at the minus connection of the controller
* @param voltage the voltage of the controller.**/
void SpiceWrapper::addVC(std::string id, std::string v1id, std::string v2id, double voltage)
{
	std::string idString = "n" + id;
	std::string v1idString = "n" + v1id;
	std::string v2idString;
	if (v2id == "" || v2id == "GND")
	{
		v2idString = "0";
	}
	else {
		v2idString = "n" + v2id;
	}
	std::cout.precision(20);
	std::string voltageString = std::to_string(voltage);
	spiceCommand("circbyline V" + idString + " " + v1idString + " " + v2idString + " " + voltageString + "V");
}
/** Adds a current controller to the spice circuit.
* @param id the id of the current controller.
* @param v1id the id of the node at the plus connection of the controller.
* @param v2id the id of the node at the minus connection of the controller
* @param current the current of the controller.
* @param power the power of the controller.**/
void SpiceWrapper::addCC(std::string id, std::string v1id, std::string v2id, double current, double power)
{
	std::string idString = "n" + id;
	std::string v1idString = "n" + v1id;
	std::string v2idString;
	if (v2id == "" || v2id == "GND")
	{
		v2idString = "0";
	}
	else {
		v2idString = "n" + v2id;
	}
	power = -power;
	current = -current;
	std::cout.precision(20);
	std::string powerString = std::to_string(power);
	std::string currentString = std::to_string(current);
	std::string setpointString;

	//TODO may solve this as formula in spice (dependent on actual voltage and setpoints)
	double estimatedCurrent = power / VOLTAGE + current;
	if (estimatedCurrent == 0)
	{
		setpointString = "0";
	}
	else if (estimatedCurrent > 0)
	{
		setpointString = "min(" + SPICE_RAMP_FACTOR + "*time," + powerString + "/v(" + v1idString + ")+" + currentString + ")";
	}
	else
	{
		setpointString = "max(-" + SPICE_RAMP_FACTOR + "*time," + powerString + "/v(" + v1idString + ")+" + currentString + ")";
	}
	spiceCommand("circbyline B" + idString + " " + v1idString + " " + v2idString + " i=" + setpointString);
}
/** Starts the calculation of the previously specified circuit. **/
void SpiceWrapper::calculate()
{
	spiceCommand("circbyline .end ");
	//spiceCommand("option param=x");
	spiceCommand("tran " + SPICE_STEP + "ms " + SPICE_STOP + "ms " + SPICE_START + "ms");
}
/** Callback method used by spice for sendChar.**/
int SpiceWrapper::sendChar(char* chars, int id, void* caller)
{
	return ((SpiceWrapper*)caller)->sendChar(chars, id);
}
/** Callback method used by spice for sendStat.**/
int SpiceWrapper::sendStat(char* chars, int id, void* caller)
{
	return ((SpiceWrapper*)caller)->sendStat(chars, id);
}
/** Callback method used by spice for sendInitData.**/
int SpiceWrapper::sendInitData(pvecinfoall p, int id, void* caller)
{
	return ((SpiceWrapper*)caller)->sendInitData(p, id);
}
/** Callback method used by spice for sendData.**/
int SpiceWrapper::sendData(pvecvaluesall p, int number, int id, void* caller)
{
	return ((SpiceWrapper*)caller)->sendData(p, number, id);
}
/** Callback method used by spice for controlledExit.**/
int SpiceWrapper::controlledExit(int status, NG_BOOL a, NG_BOOL b, int id, void* caller)
{
	return ((SpiceWrapper*)caller)->controlledExit(status, a, b, id);
}
/** Returns the caculated results of the circuit.
* @return The calculated result as pvecvaluesall (from sharedspice.h).**/
pvecvaluesall SpiceWrapper::getSolution()
{
	return solution;
}
/** Wrapper function passing commands to spice runtime.**/
void SpiceWrapper::spiceCommand(std::string command) {

	//std::cout << "command: " << command << std::endl;
	(spiceCommandFunc)(_strdup(command.c_str()));
}
/** Callback function for reading printf, fprintf, fputs (NULL allowed).**/
int SpiceWrapper::sendChar(char* chars, int id)
{
	return 0;
	std::string s = chars;
	if (s.find("stdout") != std::string::npos)
	{
		s = s.substr(7);
		if (s.find("v(n") != std::string::npos)
		{
			s = s.substr(3);
			int first = s.find_first_of(")");
			std::string node = s.substr(0, first);
			s = s.substr(first + 4);
			double v = std::stod(s);
			std::cout.precision(12);
			std::cout << std::fixed;
			std::cout << "voltage of " << node << " is " << v << " V" << std::endl;
		}
		else if (s.find("i(n") != std::string::npos)
		{
			s = s.substr(3);
			int first = s.find_first_of(")");
			std::string node = s.substr(0, first);
			s = s.substr(first + 4);
			double v = std::stod(s);
			std::cout << "Current of " << node << " is " << v << " A" << std::endl;
		}
		else
		{
			std::cout << s << std::endl;
		}
	}
	else if (s.find("stderr") != std::string::npos)
	{
		s = s.substr(7);
		std::cerr << s << std::endl;
	}

	else
	{
		std::cerr << s << std::endl;
	}
	return 0;
}
/** Callback function for reading status string and percent value (NULL allowed).**/
int SpiceWrapper::sendStat(char* chars, int id)
{
	return 0;
	std::cout << "sendstat: " << chars << std::endl;
	return 0;
}
/** Vallback function for sending an array of structs containing info on all vectors in the current plot (immediately before simulation starts) (NULL allowed)**/
int SpiceWrapper::sendInitData(pvecinfoall p, int id)
{
	return 0;
	std::cout << "--------------------InitData--------------------" << std::endl;
	decodeVecinfoAll(p);
	std::cout << "--------------------InitEnd--------------------" << std::endl;
	return 0;
}
/** Callback function for sending an array of structs containing data values of all vectors in the current plot (simulation output) (NULL allowed).**/
int SpiceWrapper::sendData(pvecvaluesall p, int number, int id)
{
	solution = p;
	return 0;

	std::cout << "--------------------SendData--------------------" << std::endl;
	std::cout << "Index: " << p->vecindex << ", ID: " << id << std::endl;
	for (int i = 0; i < p->veccount; i++)
	{
		decodeVector(p->vecsa[i]);
	}
	std::cout << "--------------------SendEnd--------------------" << std::endl;
	return 0;
}
/** Callback function for transferring a flag to caller, generated by ngspice upon a call to function controlled_exit. May be used by caller to detach ngspice.dll, if dynamically loaded or to try any other recovery method, or to exit. (required)**/
int SpiceWrapper::controlledExit(int status, NG_BOOL a, NG_BOOL b, int id)
{
	return 0;
	std::cout << "controlledExit" << std::endl;
	std::cout << "status: " << status << std::endl;
	return 0;
}
/** Decodes a pvecinfo info object (vector) of the spice library. **/
void SpiceWrapper::decodeVecinfo(pvecinfo pinfo)
{
	std::cout << "Vector: " << pinfo->number << ", name: " << pinfo->vecname << ", isReal: " << pinfo->is_real << std::endl;
}
void SpiceWrapper::decodeVecinfoAll(pvecinfoall p)
{
	std::cout << "Name: " << p->name << ", title: " << p->title << ", date: " << p->date << ", type: " << p->type << std::endl;
	for (int i = 0; i < p->veccount; i++)
	{
		decodeVecinfo(p->vecs[i]);
	}
}
/** Decodes a pvecvalues object (element of a vector) of the spice library. **/
void SpiceWrapper::decodeVector(pvecvalues p)
{
	std::cout << p->name << ": " << p->creal << std::endl;
}
