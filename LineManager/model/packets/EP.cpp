#include "EP.h"
#include "../../config.h"

/** Constructor.
* @param v1id the id of the supplier.
* @param v2id the id of the absorber.
* @param begin the beginning of the transfer. **/
EP::EP(std::string supplierID, std::string absorberID)
{
	this->supplierID = supplierID;
	this->absorberID = absorberID;
}
/** Destructor. **/
EP::~EP()
{
	for (Setpoint* sp : setpoints)
	{
		delete sp;
	}
}

/** Returns the id of the supplier Node.
* @return The node id.**/
std::string EP::getSupplierID()
{
	return supplierID;
}

/** Returns the id of the absorber Node.
* @return The node id.**/
std::string EP::getAbsorberID()
{
	return absorberID;
}

/** Returns a list of all Setpoint objects of a transfer.
* @return The list of setpoint objects.**/
std::list<Setpoint*> EP::getSetpoints()
{
	return setpoints;
}

/** Returns the begin of the transfer.
* @return The begin. **/
int EP::getBegin()
{
	return setpoints.front()->getBegin();
}

/** Returns the duration of the transfer.
* @return The duration. **/
int EP::getDuration()
{
	return getEnd() - getBegin() + 1;
}

/** Returns the end of the transfer.
* @return The end. **/
int EP::getEnd()
{
	return setpoints.back()->getBegin() + setpoints.back()->getDuration() - 1;
}

/** Returns the amount of energy transferred with this EP.
* @return The energy in Wh. **/
double EP::getEnergy()
{
	return energy;
}
/** Adds a power setpoint to an EP.
@param power the power of the setpoint
@param begin the begin of the setpoint
@param duration the duration of the setpoint
@param type the type of the setpoint**/
void EP::addPowerSetpoint(double power, int begin, int duration, Setpoint::Types type)
{
	setpoints.push_back(new Setpoint(power, 0, -power, 0, begin, duration, type));
}
/** Adds a current setpoint to an EP.
@param current the power of the setpoint
@param begin the begin of the setpoint
@param duration the duration of the setpoint
@param type the type of the setpoint**/
void EP::addCurrentSetpoint(double current, int begin, int duration, Setpoint::Types type)
{
	setpoints.push_back(new Setpoint(0, current, 0, -current, begin, duration, type));
}
/** Adds a power and current setpoint to an EP with distinction between absorber and supplier.
@param supplierPower the power of the supplier at this setpoint.
@param supplierCurrent the current of the supplier at this setpoint.
@param absorberPower the power of the absorber at this setpoint.
@param absorberCurrent the current of the absorber at this setpoint.
@param begin the begin of the setpoint
@param duration the duration of the setpoint
@param type the type of the setpoint**/
void EP::addMixedSetpoint(double supplierPower, double supplierCurrent, double absorberPower, double absorberCurrent, int begin, int duration, Setpoint::Types type)
{
	setpoints.push_back(new Setpoint(supplierPower, supplierCurrent, absorberPower, absorberCurrent, begin, duration, type));
}

/** Converts a time in seconds to a timeslot index.
* @param time the time in seconds
* @return The index of the according timeslot. **/
double EP::toTimeslot(double time)
{
	return time / S_PER_TIMESLOT;
}
