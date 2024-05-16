#include "VoltageSource.h"
/** Constructor
* @param id the id of the node.
* @param voltage the voltage of the voltage controller. **/
VoltageSource::VoltageSource(std::string id, double voltage) :Node(id)
{
	Node::setVoltage(voltage, 0, true);
}

/** Return the type, which is VOLTAGE_CONTROLLER.
* @return Types::VOLTAGE_CONTROLLER.**/
Node::Types VoltageSource::getType()
{
	return Types::VOLTAGE_CONTROLLER;
}
/** Returns a string containing all static data of the voltage controller.
* @return string containing all static data.**/
std::string VoltageSource::serialize()
{
	return Node::serialize() + "," + "VC";
}
