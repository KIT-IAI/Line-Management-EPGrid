#include "EPPowerRectangular.h"
#include "../../Config.h"


/** Constructor.
* @param v1id the id of the supplier.
* @param v2id the id of the absorber.
* @param power the power of the transfer.
* @param begin the beginning of the transfer.
* @param duration the duration of the transfer.**/
EPPowerRectangular::EPPowerRectangular(std::string v1id, std::string v2id, double power, int begin, int duration) :EP(v1id, v2id)
{
	addPowerSetpoint(power, toTimeslot(begin), toTimeslot(duration), Setpoint::Types::CONSTANT);
	energy = duration * (power / 3600) * S_PER_TIMESLOT;//TODO Check if this is the right unit Wh and not seconds
}
/** Constructor.
* @param v1id the id of the supplier.
* @param v2id the id of the absorber.
* @param power the power of the transfer.
* @param begin the beginning of the transfer.
* @param energy the energy transferred with this EP in Wh. **/
EPPowerRectangular::EPPowerRectangular(std::string v1id, std::string v2id, double power, int begin, double energy) : EP(v1id, v2id)
{
	this->energy = energy;
	double energyPerTimeslot = (power / 3600) * S_PER_TIMESLOT;
	double duration = energy / energyPerTimeslot;
	int timeslotCount = ceil(duration);
	addPowerSetpoint(power, toTimeslot(begin), timeslotCount, Setpoint::Types::CONSTANT);
}

/** Returns the energy which could be transferred in the time period of the transfer when each timeslot would be fully utilized.
* @return The energy in Wh.**/
double EPPowerRectangular::getPossibleEnergy()
{
	double energyPerTimeslot = (getSetpoints().front()->getSupplierPower() / 3600) * S_PER_TIMESLOT;
	double possibleEnergy = getDuration() * energyPerTimeslot;
	return possibleEnergy;//TODO Check if this is the right unit Wh and not seconds
}


