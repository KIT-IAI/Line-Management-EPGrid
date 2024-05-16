#include <string>
#include <sstream>
#include <iomanip>
#include "Node.h"
#include "Line.h"
/** Constructor
*@param id the id of the node.**/
Node::Node(std::string id)
{
	this->id = id;
	voltages[0] = 0;
}

/** Returns the id of a node
* @return The id of the node.
**/
std::string Node::getID()
{
	return id;
}
/** Adds a Line to a Node.
* @param l the line to be added.**/
void Node::addLine(Line* l)
{
	lines.push_back(l);
}
/** Returns all Line objects which involve the Node.
* @return List of all Line objects the Node is involved.
**/
std::vector<Line*> Node::getLines()
{
	return lines;
}
/** Returns a string containing all static data of the Node.
* @return String containing all static data.**/
std::string Node::serializeStatic()
{
	return id;
}
/** Returns a string containing dynamic data of the Node at a given timeslot.
* @param timeslot the timeslot to be considered
* @return String of dynamic data.**/
std::string Node::serializeDynamic(int timeslot)
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


/** Checks if a timeslot at given time exists.
* @param timeslot the timeslot to be checked.
* @return True when the timeslot exists and false when not.**/
bool Node::timeslotExist(int timeslot)
{
	return voltages.count(timeslot) == 1;
}

double Node::getTimeProperty(int timeslot, std::map<int, double>* propertyMap)
{
	std::map<int, double>::iterator it = propertyMap->upper_bound(timeslot);
	--it;
	return it->second;
}

void Node::setTimeProperty(double value, int timeslot, std::map<int, double>* propertyMap)
{
	(*propertyMap)[timeslot] = value;
}

void Node::addTimeProperty(double value, int timeslot, std::map<int, double>* propertyMap)
{
	(*propertyMap)[timeslot] = getTimeProperty(timeslot, propertyMap) + value;//Use getter here because datapoint (*propertyMap)[timeslot] may not exist. Getter method returns the value at a given time which not necessarily means that a datapoint with the given time exists
}

void Node::addTimeProperty(double value, int begin, int duration, std::map<int, double>* propertyMap)
{
	addTimeProperty(0, begin + duration, propertyMap);//Ensure that datapoint at begin+duration exists
	addTimeProperty(value, begin, propertyMap);

	for (std::map<int, double>::iterator it = propertyMap->upper_bound(begin); it != propertyMap->lower_bound(begin + duration); ++it)
	{
		it->second += value;
	}
}
void Node::removeTimeProperty(int timeslot, std::map<int, double>* propertyMap)
{
	propertyMap->erase(timeslot);
}

void Node::removeTimeProperty(double value, int timeslot, std::map<int, double>* propertyMap)
{
	(*propertyMap)[timeslot] -= value;
	if ((*propertyMap)[timeslot] == getTimeProperty(timeslot - 1, propertyMap))
	{
		removeTimeProperty(timeslot, propertyMap);
	}
	if (getTimeProperty(timeslot, propertyMap) == getTimeProperty(timeslot + 1, propertyMap))
	{
		removeTimeProperty(timeslot + 1, propertyMap);
	}
}

void Node::removeTimeProperty(double value, int begin, int duration, std::map<int, double>* propertyMap)
{
	std::list<int> toErase;
	for (std::map<int, double>::iterator it = propertyMap->lower_bound(begin); it != propertyMap->lower_bound(begin + duration); ++it)
	{
		(*propertyMap)[it->first] -= value;
		if ((*propertyMap)[it->first] == getTimeProperty((it->first) - 1, propertyMap))
		{
			toErase.push_back(it->first);
		}
	}

	for (int t : toErase)
	{
		removeTimeProperty(t, propertyMap);
	}
	if (getTimeProperty(begin + duration, propertyMap) == getTimeProperty(begin + duration - 1, propertyMap))
	{
		removeTimeProperty(begin + duration, propertyMap);
	}
}

/** Returns the voltage over a node at a given time.
* @param timeslot the timeslot to be checked.
* @return voltage in volt.**/
double Node::getVoltage(int timeslot)
{
	return getTimeProperty(timeslot, &voltages);
}
/** Sets a voltage at a given timeslot, if previous timeslots voltage is different.
* @param voltage the voltage to be set
* @param timeslot the timeslot where the voltage shall be set
* @param force force the creation of the timeslot even if previous timeslot has the same voltage.**/
void Node::setVoltage(double voltage, int timeslot, bool force)
{
	if (getVoltage(timeslot) != voltage || force)
	{
		setTimeProperty(voltage, timeslot, &voltages);
	}
}

/** Returns the current flowing through a Node at given timeslot.
* @param timeslot the timeslot of which the current shall be returned.
* @return The current in ampere.
**/
double Node::getCurrent(int timeslot)
{
	double sum = 0;
	for (Line* l : lines)
	{
		sum += l->getCurrentRelative(timeslot, this);
	}
	return sum;
}

/** Returns the power of a Node at given timeslot.
* @param timeslot the timeslot of which the power shall be returned.
* @return The power in watt.
**/
double Node::getPower(int timeslot)
{
	return getVoltage(timeslot) * getCurrent(timeslot);//Power interpreted as "source", for typical interpretation as household remove -
}

/** Prints all timeslots of a Node.**/
void Node::printTimeslots()
{
	for (std::map<int, double>::iterator it = voltages.begin(); it != voltages.end(); ++it)
	{
		std::cout << "Time " << it->first << " = > Voltage " << it->second << std::endl;
	}

}
/** Returns all timeslots of a Node.
* @return All timeslots of the node as a vector.**/
std::vector<double> Node::getTimeslots()
{
	std::vector <double> timeslots;
	timeslots.reserve(voltages.size());
	for (std::map<int, double>::iterator it = voltages.begin(); it != voltages.end(); ++it)
	{
		timeslots.push_back(it->first);
	}
	return timeslots;
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
	return os << "Node " << n.id;
}