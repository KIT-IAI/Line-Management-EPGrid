#include "CurrentController.h"
/** Constructor
* @param id the id of the node.**/
CurrentController::CurrentController(std::string id) :Node(id)
{
	powerSetpoints[0] = 0;
	currentSetpoints[0] = 0;
}
/** Return the type, which is CURRENT_CONTROLLER.
* @return Types::CURRENT_CONTROLLER.**/
Node::Types CurrentController::getType()
{
	return Types::CURRENT_CONTROLLER;
}
/** Returns a string containing all static data of the current controller.
* @return String containing all static data.**/
std::string CurrentController::serializeStatic()
{
	return Node::serializeStatic() + "," + "CC";
}
/** Adds a setpoint to the controller.
* @param setpoint the value of the setpoint.
* @param begin the beginning timeslot of the setpoint.
* @param duration the duration to hold the setpoint.**/
void CurrentController::addPowerSetpoint(double setpoint, int begin, int duration)
{
	addTimeProperty(setpoint, begin, duration, &powerSetpoints);
}
/** Removes a setpoint from the controller.
* @param setpoint the value of the setpoint.
* @param begin the beginning timeslot of the setpoint. 
* @param duration the duration to hold the setpoint.**/
void CurrentController::removePowerSetpoint(double setpoint, int begin, int duration)
{
	removeTimeProperty(setpoint, begin, duration, &powerSetpoints);
}

/** Returns the setpoint of the controller at given time.
* @param time the timeslot of the setpoint.
* @return The value of the setpoint at given time.**/
double CurrentController::getPowerSetpoint(int timeslot)
{
	return getTimeProperty(timeslot, &powerSetpoints);
}

/** Adds a setpoint to the controller.
* @param setpoint the value of the setpoint.
* @param begin the beginning timeslot of the setpoint.
* @param duration the duration to hold the setpoint.**/
void CurrentController::addCurrentSetpoint(double setpoint, int begin, int duration)
{
	addTimeProperty(setpoint, begin, duration, &currentSetpoints);
}
/** Removes a setpoint from the controller.
* @param setpoint the value of the setpoint.
* @param begin the beginning timeslot of the setpoint.
* @param duration the duration to hold the setpoint.**/
void CurrentController::removeCurrentSetpoint(double setpoint, int begin, int duration)
{
	removeTimeProperty(setpoint, begin, duration, &currentSetpoints);
}

/** Returns the setpoint of the controller at given time.
* @param time the timeslot of the setpoint.
* @return The value of the setpoint at given time.**/
double CurrentController::getCurrentSetpoint(int timeslot)
{
	return getTimeProperty(timeslot, &currentSetpoints);
}

/** Returns a map of all power setpoints of the controller. 
* @return A map with timeslots as keys and power setpoints as values.
**/
std::map<int, double> CurrentController::getPowerSetpoints()
{
	return powerSetpoints;
}
/** Returns a map of all current setpoints of the controller.
* @return A map with timeslots as keys and current setpoints as values.
**/
std::map<int, double> CurrentController::getCurrentSetpoints()
{
	return currentSetpoints;
}
