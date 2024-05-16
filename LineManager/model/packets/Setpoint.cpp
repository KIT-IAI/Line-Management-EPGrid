#include "Setpoint.h"
#include <iostream>

/** Constructor.
* @param setpoint the value of the setpoint.
* @param begin the begin of the setpoint.
* @param duration the duration of the setpoint.
* @param type the type of the setpoint.
**/
Setpoint::Setpoint(double supplierPower, double supplierCurrent, double absorberPower, double absorberCurrent, int begin, int duration, Types type)
{

	this->begin = begin;
	this->duration = duration;
	this->type = type;
	this->supplierPower = supplierPower;
	this->supplierCurrent = supplierCurrent;
	this->absorberPower = absorberPower;
	this->absorberCurrent = absorberCurrent;
	//std::cout << "Power: " << setpoint << " begin: " << begin << " duration: " << duration << std::endl;
}
/** Returns the power of the supplier of the setpoint.
* @return The power of the supplier.**/
double Setpoint::getSupplierPower()
{
	return supplierPower;
}
/** Returns the current of the supplier of the setpoint.
* @return The current of the supplier.**/
double Setpoint::getSupplierCurrent()
{
	return supplierCurrent;
}
/** Returns the power of the absorber of the setpoint.
* @return The power of the absorber.**/
double Setpoint::getAbsorberPower()
{
	return absorberPower;
}
/** Returns the current of the absorber of the setpoint.
* @return The current of the absorber.**/
double Setpoint::getAbsorberCurrent()
{
	return absorberCurrent;
}
/** Returns the begin of the setpoint.
* @return The begin of the setpoint.**/
int Setpoint::getBegin()
{
	return begin;
}
/** Returns the duration of the setpoint.
* @return The duration of the setpoint.**/
int Setpoint::getDuration()
{
	return duration;
}
/** Returns the type of the setpoint.
* @return The type of the setpoint.**/
Setpoint::Types Setpoint::getType()
{
	return type;
}
/** Returns the type of the setpoint as string.
* @return The type of the setpoint.**/
std::string Setpoint::getTypeString()
{
	switch (type)
	{
	case Types::CONSTANT:
		return "CONSTANT";
	case Types::RAMP:
		return "RAMP";
	}
}