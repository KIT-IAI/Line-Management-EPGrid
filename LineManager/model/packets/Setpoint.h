#pragma once
#include <string> 
/** Class for describing a setpoint of a EP transfer. **/
class Setpoint
{
public:
	enum class Types { RAMP, CONSTANT};
	Setpoint(double supplierPower, double supplierCurrent, double absorberPower, double absorberCurrent,int begin, int duration,Types type);
	double getSupplierPower();
	double getSupplierCurrent();
	double getAbsorberPower();
	double getAbsorberCurrent();
	int getBegin();
	int getDuration();
	Setpoint::Types getType();
	std::string getTypeString();
private:
	double supplierPower;
	double supplierCurrent;
	double absorberPower;
	double absorberCurrent;
	int begin;
	int duration;
	Types type;
};