#pragma once
#include "EP.h"

/** Class for describing an energy packet with a trapezoidal power shape. **/
class EPPowerTrapeze : public EP
{
public:
	//EPPowerTrapeze(int v1id, int v2id, double power, double rampPower, int begin, int duration);
	EPPowerTrapeze(std::string v1id, std::string v2id, double power, double rampPower, int begin, double energy);
	EPPowerTrapeze(std::string v1id, std::string v2id, double power, double rampUpPower, double rampDownPower, int begin, double energy);
	double getPossibleEnergy();
private:
	double getRampDuration(double power, double rampPower);
	double getRampEnergy(double power, double duration);
	double getSteadyDuration(double power, double energy);
	double getRampPowerPerTimeslot(double rampPower);
	void setRampUp(double power, double rampUpPower, int begin, int duration);
	void setConstant(double power, int begin, int duration);
	void setRampDown(double power, double rampDownPower, int begin, int duration);
};