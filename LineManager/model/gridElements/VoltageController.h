#pragma once
#include "Node.h"

/** Class representing a voltage controller of the grid **/
class VoltageController : public Node
{
public:
	VoltageController(std::string id, double voltage);
	Types getType();
	std::string serializeStatic();
private:
};
