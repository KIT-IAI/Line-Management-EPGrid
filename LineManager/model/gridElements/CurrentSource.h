#pragma once
#include "Node.h"
#include "../packets/Setpoint.h"

/** Class representing current controller of the grid. **/
class CurrentSource : public Node
{
public:	
	CurrentSource(std::string id);
	Types getType();
	std::string serialize();
	void addPowerSetpoint(double setpoint, int begin, int duration);
	void removePowerSetpoint(double setpoint, int begin, int duration);
	double getPowerSetpoint(int timeslot);
	void addCurrentSetpoint(double setpoint, int begin, int duration);
	void removeCurrentSetpoint(double setpoint, int begin, int duration);
	double getCurrentSetpoint(int timeslot);
	std::map<int, double> getPowerSetpoints();
	std::map<int, double> getCurrentSetpoints();
private:
	std::map<int,double> powerSetpoints;
	std::map<int, double> currentSetpoints;
};