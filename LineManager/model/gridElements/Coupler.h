#pragma once
#include "Node.h"

/** Class representing coupling node of the grid. **/
class Coupler : public Node
{
public:
	Coupler(std::string id);
	Types getType();
	std::string serializeStatic();
private:
};