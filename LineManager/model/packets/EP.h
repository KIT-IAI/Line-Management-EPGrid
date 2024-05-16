#pragma once
#include "Setpoint.h"
#include <list>
#include <string>
#include "Setpoint.h"


/** Generic class for describing an energy packet (EP) **/
class EP
{
public:
	EP(std::string supplierID, std::string absorberID);
	~EP();
	std::string getSupplierID();
	std::string getAbsorberID();
	std::list<Setpoint*> getSetpoints();
	int getBegin();
	int getDuration();
	int getEnd();
	double getEnergy();

protected:
	void addPowerSetpoint(double power,  int begin, int duration, Setpoint::Types type);
	void addCurrentSetpoint(double current, int begin, int duration, Setpoint::Types type);
	void addMixedSetpoint(double supplierPower, double supplierCurrent, double absorberPower, double absorberCurrent, int begin, int duration, Setpoint::Types type);
	double toTimeslot(double time);
	double energy;

private:
	std::string supplierID;
	std::string absorberID;
	std::list<Setpoint*> setpoints;


};
