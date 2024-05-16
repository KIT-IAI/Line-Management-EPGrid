#include <string>
#include <sstream>
#include <iomanip>
#include "Line.h"
#include "Node.h"

/** Constructor
* @param v1 the first Node.
* @param v2 the second Node.
* @param t the type from CableTypes.
* @param length the length in meters.**/
Line::Line(Node* v1, Node* v2, CableTypes::Types t, double length)
{
	this->v1 = v1;
	this->v2 = v2;
	resistance = CableTypes::getResistance(t) * length / 1000;
	limit = CableTypes::getLimit(t);
}
/** Constructor, automatically selects the CableTypes based on the types of Node objects
* @param v1 the first Node.
* @param v2 the second Node.
* @param length the length in meters.**/
Line::Line(Node* v1, Node* v2, double length)
{
	//This is not input validation! This just automatically selects the line type and ordering of the nodes.
	//TODO is it necessary to order nodes? current direction can be derived based on supply or withdraw situation 
	switch (v1->getType())
	{
	case Node::Types::CURRENT_CONTROLLER:
		*this = Line(v2, v1, CableTypes::Types::NAYY50SE, length);
		return;
	case Node::Types::VOLTAGE_CONTROLLER:
		*this = Line(v1, v2, CableTypes::Types::NAYY150SE, length);
		return;
	case Node::Types::COUPLER:
		switch (v2->getType())
		{
		case Node::Types::CURRENT_CONTROLLER:
			*this = Line(v1, v2, CableTypes::Types::NAYY50SE, length);
			return;
		case Node::Types::VOLTAGE_CONTROLLER:
			*this = Line(v2, v1, CableTypes::Types::NAYY150SE, length);
			return;
		case Node::Types::COUPLER:
			*this = Line(v1, v2, CableTypes::Types::NAYY150SE, length);
			return;
		}
	default:
		this->v1 = nullptr;
		this->v2 = nullptr;
		resistance = -1;
		limit = -1;
		return;
	}
}
std::string Line::getID()
{
	return 	v1->getID()+ ","+ v2->getID();
}
/** Returns the two Node objects.
* @return The two Node objects of a line in a vector.
**/
std::pair<Node*, Node*> Line::getNodes()
{
	std::pair<Node*, Node*> nodes;
	nodes.first=v1;
	nodes.second=v2;
	return nodes;
}
std::pair<std::string, std::string> Line::getNodeIDs()
{
	std::pair<std::string, std::string> nodes;
	nodes.first = v1->getID();
	nodes.second = v2->getID();
	return nodes;
}
/** Returns the resistance.
* @return the resistance in ohm. **/
double Line::getResistance()
{
	return resistance;
}
/** Returns the current limit.
* @return the limit in ampere. **/
int Line::getLimit()
{
	return limit;
}
/** Returns the voltage over the Line at a given time.
* @param timeslot the timeslot to be considered
* @return the voltage in volt. **/
double Line::getVoltage(int timeslot)
{
	return v1->getVoltage(timeslot) - v2->getVoltage(timeslot);
}
/** Returns the current flowing through the Line at a given time.
* @param timeslot the timeslot to be considered
* @return the current in ampere. **/
double Line::getCurrent(int timeslot)
{

	return getVoltage(timeslot) / resistance;
}
/** Returns the current flowing through the Line relative to a given node at a given time.
* @param timeslot the timeslot to be considered.
* @param v the Node to which the current shall be relatively specified.
* @return the current in ampere. **/
double Line::getCurrentRelative(int timeslot, Node* v)
{
	if (v == v1)
		return getCurrent(timeslot);
	else
		return -getCurrent(timeslot);
}
/** Returns the power over the Line at a given time.
* @param timeslot the timeslot to be considered
* @return the power in watt. **/
double Line::getPower(int timeslot)
{
	return getVoltage(timeslot) * getCurrent(timeslot);
}
/** Returns a string containing static data of the Line.
* @return String of static data.**/
std::string Line::serializeStatic()
{
	std::ostringstream streamObj;
	streamObj << std::fixed;
	streamObj << std::setprecision(0);
	streamObj << v1->getID();
	streamObj << ",";
	streamObj << v2->getID();
	streamObj << ",";
	streamObj << std::setprecision(5);
	streamObj << resistance;
	streamObj << ",";
	streamObj << std::setprecision(0);
	streamObj << limit;
	return streamObj.str();
}
/** Returns a string containing dynamic data of the Line at a given timeslot.
* @param timeslot the timeslot to be considered
* @return String of dynamic data.**/
std::string Line::serializeDynamic(int timeslot)
{
	std::ostringstream streamObj;
	streamObj << std::fixed;
	streamObj << std::setprecision(1);
	streamObj << getVoltage(timeslot);
	streamObj << ",";
	streamObj << std::setprecision(2);
	streamObj << getCurrent(timeslot);
	streamObj << ",";
	streamObj << std::setprecision(0);
	streamObj << getPower(timeslot);
	return streamObj.str();
}
/** Returns the second Node object of a Line for one Node given.
* @param v a Node object.
* @return the other Node object.**/
Node* Line::getPartnerNode(Node* v)
{
	if (v == v1)
	{
		return v2;
	}
	else if (v == v2)
	{
		return v1;
	}
	else
	{
		std::cerr << *v << " is not a node of this line" << std::endl;
		return NULL;
	}

}

std::ostream& operator<<(std::ostream& os, const Line& l)
{
	return os << "Line between " << *l.v1 << " and " << *l.v2;
}