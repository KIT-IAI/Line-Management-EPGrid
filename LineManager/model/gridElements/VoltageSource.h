#pragma once
#include "Node.h"

/** Class representing a voltage controller of the grid **/
class VoltageSource : public Node
{
public:
	VoltageSource(std::string id, double voltage);
	Types getType();
	std::string serialize();
private:
};
