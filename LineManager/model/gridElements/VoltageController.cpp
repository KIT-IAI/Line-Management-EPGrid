#include "VoltageController.h"
/** Constructor
* @param id the id of the node.
* @param voltage the voltage of the voltage controller. **/
VoltageController::VoltageController(std::string id, double voltage) :Node(id)
{
	Node::setVoltage(voltage, 0, true);
}

/** Return the type, which is VOLTAGE_CONTROLLER.
* @return Types::VOLTAGE_CONTROLLER.**/
Node::Types VoltageController::getType()
{
	return Types::VOLTAGE_CONTROLLER;
}
/** Returns a string containing all static data of the voltage controller.
* @return string containing all static data.**/
std::string VoltageController::serializeStatic()
{
	return Node::serializeStatic() + "," + "VC";
}
