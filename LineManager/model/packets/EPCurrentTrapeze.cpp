#include "EPCurrentTrapeze.h"
#include "../../Config.h"
#include <iostream>

/** Constructor.
* @param v1id the id of the supplier.
* @param v2id the id of the absorber.
* @param current the power of the transfer.
* @param rampCurrent the current increase/decrease during a second while ramping.
* @param begin the beginning of the transfer.
* @param duration the duration of the EP in s.**/
EPCurrentTrapeze::EPCurrentTrapeze(std::string v1id, std::string v2id, double current, double rampCurrent, int begin, double duration) :EPCurrentTrapeze(v1id, v2id, current, rampCurrent, rampCurrent, begin, duration) {}

/** Constructor.
* @param v1id the id of the supplier.
* @param v2id the id of the absorber.
* @param current the current of the transfer.
* @param rampUpCurrent the current increase per second while ramping up.
* @param rampDownCurrent the current decrease per second while ramping down.
* @param begin the beginning of the transfer.
* @param duration the duration of the EP in s.**/
EPCurrentTrapeze::EPCurrentTrapeze(std::string v1id, std::string v2id, double current, double rampUpCurrent, double rampDownCurrent, int begin, double duration) : EP(v1id, v2id)
{
	//std::cout << "rampUpCurrent: " << rampUpCurrent << " current: " << current << " rampDownCurrent: " << rampDownCurrent << " duration: " << duration << std::endl;
	int beginTimeslot = toTimeslot(begin);
	int rampUpTimeslots = ceil(toTimeslot(getRampDuration(current, rampUpCurrent)));
	int rampDownTimeslots = ceil(toTimeslot(getRampDuration(current, rampDownCurrent)));
	int constantTimeslots = ceil(toTimeslot(duration) - rampUpTimeslots - rampDownTimeslots);

	if (constantTimeslots < 0)
	{
		throw std::invalid_argument("No time left for constant phase");
		return;
	}
	if (rampUpTimeslots < 1 || constantTimeslots < 0 || rampDownTimeslots < 1)
	{
		throw std::invalid_argument("A resulting count of timeslots is greater than max int.");
		return;
	}
	setRampUp(current, rampUpCurrent, beginTimeslot, rampUpTimeslots);
	setConstant(current, beginTimeslot + rampUpTimeslots, constantTimeslots);
	setRampDown(current, rampDownCurrent, beginTimeslot + rampUpTimeslots + constantTimeslots, rampDownTimeslots);
}

/** Calculates the ramp duration.
* @param current the current of the transfer.
* @param rampCurrent the current increase/decrease during a second while ramping.
* @return The duration of the ramp phase in seconds.**/
double EPCurrentTrapeze::getRampDuration(double current, double rampCurrent)
{
	return current / rampCurrent;
}

/** Calculates the current which is reached after one timeslot of ramping.
* @param rampCurrent the ramps current change per second.
* @return The current in A.**/
double EPCurrentTrapeze::getRampCurrentPerTimeslot(double rampCurrent)
{
	return rampCurrent * S_PER_TIMESLOT;
}

/** Sets the Setpoint objects during ramp up phase.
* @param current the current after the ramp up phase..
* @param rampUpCurrent the ramps current increase per timeslot.
* @param begin the beginning timeslot of the ramp up phase.
* @param duration the duration of the ramp up phase.
**/
void EPCurrentTrapeze::setRampUp(double current, double rampUpCurrent, int begin, int duration)
{
	for (int i = 0; i < duration - 1; i++)
	{
		addCurrentSetpoint(getRampCurrentPerTimeslot(rampUpCurrent) * (i + 1), begin + i, 1, Setpoint::Types::RAMP);
	}
	addCurrentSetpoint(current, begin + duration - 1, 1, Setpoint::Types::RAMP);
}

/** Sets the Setpoint object for the constant phase.
* @param current the current during the constant pahse.
* @param begin the beginning timeslot of the constant pahse.
* @param duration the duration of the constant pahse.
**/
void EPCurrentTrapeze::setConstant(double current, int begin, int duration)
{
	addCurrentSetpoint(current, begin, duration, Setpoint::Types::CONSTANT);
}

/** Sets the Setpoint objects during ramp down phase.
* @param current the current at the beginning of the ramp down phase.
* @param rampUpCurrent the ramps current decrease per timeslot.
* @param begin the beginning timeslot of the ramp down phase.
* @param duration the duration of the ramp down phase.
**/
void EPCurrentTrapeze::setRampDown(double current, double rampDownCurrent, int begin, int duration)
{
	for (int i = 0; i < duration; i++)
	{
		addCurrentSetpoint(current - getRampCurrentPerTimeslot(rampDownCurrent) * i, begin + i, 1, Setpoint::Types::RAMP);
	}
}
