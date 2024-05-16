#include "EPPowerTrapeze.h"
#include "../../Config.h"
#include <iostream>

/** Constructor.
* @param v1id the id of the supplier.
* @param v2id the id of the absorber.
* @param power the power of the transfer.
* @param rampPower the power increase/decrease during a second while ramping.
* @param begin the beginning of the transfer.
* @param energy the energy transferred with this EP in Wh.**/
EPPowerTrapeze::EPPowerTrapeze(std::string v1id, std::string v2id, double power, double rampPower, int begin, double energy) :EPPowerTrapeze(v1id, v2id, power, rampPower, rampPower, begin, energy) {}

EPPowerTrapeze::EPPowerTrapeze(std::string v1id, std::string v2id, double power, double rampUpPower, double rampDownPower, int begin, double energy) : EP(v1id, v2id)
{
	//std::cout << "power: " << power << " rampUpPower: " << rampUpPower << " rampDownPower: " << rampDownPower << " begin: " << begin << " energy: " << energy <<std::endl;
	this->energy = energy;
	//Calculate ramp up parameters
	double rampUpDuration = getRampDuration(power, rampUpPower);
	double rampUpEnergy = getRampEnergy(power, rampUpDuration);
	double rampUpTimeslotsDouble = toTimeslot(rampUpDuration);
	int rampUpTimeslots = ceil(rampUpTimeslotsDouble);

	//Calculate ramp down parameters
	double rampDownDuration = getRampDuration(power, rampDownPower);
	double rampDownEnergy = getRampEnergy(power, rampDownDuration);


	//Calculate constant power parameters
	double constantEnergy = energy - rampUpEnergy - rampDownEnergy;
	double constantDuration = getSteadyDuration(power, constantEnergy);
	int timeslotBegin = toTimeslot(begin);
	double constantTimeslotsDouble = toTimeslot(constantDuration) - (rampUpTimeslots - rampUpTimeslotsDouble);
	int constantTimeslots = ceil(constantTimeslotsDouble);
	double rampDownTimeslotsDouble = toTimeslot(rampDownDuration) - (constantTimeslots - constantTimeslotsDouble);
	int rampDownTimeslots = ceil(rampDownTimeslotsDouble);
	double remainingPower = power - (constantTimeslots - constantTimeslotsDouble) * getRampPowerPerTimeslot(rampDownPower);

	if (constantEnergy < 0)
	{
		throw std::invalid_argument("No energy left for constant phase");
		return;
	}
	if (rampUpTimeslotsDouble > INT_MAX || constantTimeslotsDouble > INT_MAX || rampDownTimeslotsDouble > INT_MAX)
	{
		throw std::invalid_argument("A resulting count of timeslots is greater than max int.");
		return;
	}

	setRampUp(power, rampUpPower, timeslotBegin, rampUpTimeslots);
	setConstant(power, timeslotBegin + rampUpTimeslots, constantTimeslots);
	setRampDown(remainingPower, rampDownPower, timeslotBegin + rampUpTimeslots + constantTimeslots, rampDownTimeslots);
}


/** Returns the energy which could be transferred in the time period of the transfer when each timeslot would be fully utilized.
* @return The energy in Wh.**/
double EPPowerTrapeze::getPossibleEnergy()
{
	//TODO this is wrong becuase first setpoint is during ramp phase
	double energyPerTimeslot = (getSetpoints().front()->getSupplierPower() / 3600) * S_PER_TIMESLOT;
	double possibleEnergy = getDuration() * energyPerTimeslot;
	return possibleEnergy;//TODO Check if this is the right unit Wh and not seconds
}

/** Calculates the ramp duration.
* @param power the power of the transfer.
* @param rampPower the power increase/decrease during a second while ramping.
* @return The duration of the ramp phase in seconds.**/
double EPPowerTrapeze::getRampDuration(double power, double rampPower)
{
	return power / rampPower;
}

/** Calculates the energy transferred during ramp phase.
* @param rampPower the power increase/decrease during a second while ramping.
* @param duration The duration of the ramp phase in seconds.
* @return The energy of the ramp phase in Wh.**/
double EPPowerTrapeze::getRampEnergy(double power, double duration)
{
	return (0.5 * power * duration) / 3600;
}

/** Calculates the duration of the steady phase.
* @param power the power of the transfer.
* @param energy The energy to be transferred during steady phase in Wh.
* @return The duration of the steady phase in seconds.**/
double EPPowerTrapeze::getSteadyDuration(double power, double energy)
{
	return (energy / power) * 3600;
}

/** Calculates the power which is reached after one timeslot of ramping.
* @param rampPower the ramps power change per second.
* @return The power in watts.**/
double EPPowerTrapeze::getRampPowerPerTimeslot(double rampPower)
{
	return rampPower * S_PER_TIMESLOT;
}
/** Sets the Setpoint objects during ramp up phase.
* @param power the power after the ramp up phase..
* @param rampUpPower the ramps power increase per timeslot.
* @param begin the beginning timeslot of the ramp up phase.
* @param duration the duration of the ramp up phase.
**/
void EPPowerTrapeze::setRampUp(double power, double rampUpPower, int begin, int duration)
{
	for (int i = 0; i < duration - 1; i++)
	{
		addPowerSetpoint(getRampPowerPerTimeslot(rampUpPower) * (i + 1), begin + i, 1, Setpoint::Types::RAMP);
	}
	addPowerSetpoint(power, begin + duration - 1, 1, Setpoint::Types::RAMP);
}
/** Sets the Setpoint object for the constant phase.
* @param power the power during the constant pahse.
* @param begin the beginning timeslot of the constant pahse.
* @param duration the duration of the constant pahse.
**/
void EPPowerTrapeze::setConstant(double power, int begin, int duration)
{
	addPowerSetpoint(power, begin, duration, Setpoint::Types::CONSTANT);
}
/** Sets the Setpoint objects during ramp down phase.
* @param power the power at the beginning of the ramp down phase.
* @param rampUpPower the ramps power decrease per timeslot.
* @param begin the beginning timeslot of the ramp down phase.
* @param duration the duration of the ramp down phase.
**/
void EPPowerTrapeze::setRampDown(double power, double rampDownPower, int begin, int duration)
{
	for (int i = 0; i < duration; i++)
	{
		addPowerSetpoint(power - getRampPowerPerTimeslot(rampDownPower) * i, begin + i, 1, Setpoint::Types::RAMP);
	}
}
