#pragma once
#include "../../SpiceLib/sharedspice.h"
#include <string>
#include <windows.h>

class SpiceWrapper
{
	typedef int(__cdecl* SPICEINIT)(SendChar* printfcn, SendStat* statfcn, ControlledExit* ngexit, SendData* sdata, SendInitData* sinitdata, BGThreadRunning* bgtrun, void* userData);
	typedef int(__cdecl* SPICECOMMAND)(char* command);
	
public:
	SpiceWrapper();
	~SpiceWrapper();
	void newCircuit(std::string name);
	void addLine(std::string v1id, std::string v2id, double resistance);
	void addVC(std::string id, std::string v1id, std::string v2id, double voltage);
	void addCC(std::string id, std::string v1id, std::string v2id, double current, double power);
	void calculate();
	static int sendChar(char* chars, int id, void* caller);
	static int sendStat(char* chars, int id, void* caller);
	static int sendInitData(pvecinfoall p, int id, void* caller);
	static int sendData(pvecvaluesall p, int number, int id, void* caller);
	static int controlledExit(int status, NG_BOOL a, NG_BOOL b, int id, void* caller);
	pvecvaluesall getSolution();

protected:

private:	
	static int count;
	HINSTANCE hinstLib;
	SPICECOMMAND spiceCommandFunc;
	pvecvaluesall solution;
	void spiceCommand(std::string command);
	int sendChar(char* chars, int id);
	int sendStat(char* chars, int id);
	int sendInitData(pvecinfoall p, int id);
	int sendData(pvecvaluesall p, int number, int id);
	int controlledExit(int status, NG_BOOL a, NG_BOOL b, int id);
	void decodeVecinfo(pvecinfo pinfo);
	void decodeVecinfoAll(pvecinfoall p);
	void decodeVector(pvecvalues p);
};
