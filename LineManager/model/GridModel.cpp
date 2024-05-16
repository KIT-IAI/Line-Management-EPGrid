#include "GridModel.h"
#include "gridElements/CurrentSource.h"

/** Constructor. **/
GridModel::GridModel()
{
	locked = false;
}

/** Destructor. **/
GridModel::~GridModel()
{
	delete admittanceMatrix;
	for (int i = 0; i < nodes.size(); i++)
	{
		delete nodes[i];
	}
	for (int i = 0; i < lines.size(); i++)
	{
		delete lines[i];
	}

}

/** Adds a Node to the model.
* @param v the Node to be added.**/
void GridModel::addNode(Node* v)
{
	nodes.push_back(v);
}
/** Returns the Node with a given id.
* @param id the id of the Node.
* @return The node with the given id. nullptr if no node with the id exists.**/
Node* GridModel::getNode(std::string id)
{
	for (Node* v : nodes)
	{
		if (id == v->getID())
		{
			return v;
		}
	}
	return nullptr;
}
/** Checks if a Node with a given id exists.
* @param id the id of the Node.
* @return True when the Node exists and false when not.**/
bool GridModel::nodeExist(std::string id)
{
	if (getNode(id) == nullptr)
	{
		return false;
	}
	return true;
}
/** Returns all Node objects of the model.
* @return All Node objects in a list.**/
std::vector <Node*> GridModel::getNodes()
{
	return nodes;
}
/** Adds a Line to the model.
* @param l the Line to be added.**/
void GridModel::addLine(Line* l)
{
	lines.push_back(l);
}
/** Returns the Line object connecting two specified nodes.
* @param v1 object of a Node of the Line
* @param v2 object of a second Node of the Line
* @return A Line object if the Line exists in the grid or a nullptr if their is no matching Line.**/
Line* GridModel::getLine(Node* v1, Node* v2)
{
	return getLine(v1->getID(), v2->getID());
}
/** Returns the Line object connecting two specified nodes.
* @param v1id id of a Node of the Line
* @param v2id id of a second Node of the Line
* @return A Line object if the Line exists in the grid or a nullptr if their is no matching Line.**/
Line* GridModel::getLine(std::string v1id, std::string v2id)
{
	for (Line* l : lines)
	{
		if (l->getNodeIDs().first == v1id && l->getNodeIDs().second == v2id)
		{
			return l;
		}
		if (l->getNodeIDs().first == v2id && l->getNodeIDs().second == v1id)
		{
			return l;
		}
	}
	return nullptr;
}
/** Checks if a Line with the connecting the given Node objects exists.
* @param v1 the first Node of the Line.
* @param v2 the second Node of the Line.
* @return True when the Line exists and false when not.**/
bool GridModel::lineExist(Node* v1, Node* v2)
{
	return lineExist(v1->getID(), v2->getID());
}
/** Checks if a Line with the connecting the given Node ids exists.
* @param v1 the id of the first Node of the Line.
* @param v2 the id of the second Node of the Line.
* @return True when the Line exists and false when not.**/
bool GridModel::lineExist(std::string v1id, std::string v2id)
{
	for (Line* l : lines)
	{
		std::pair<Node*, Node*> nodes = l->getNodes();
		if (v1id != nodes.first->getID() && v1id != nodes.second->getID())
			continue;

		if (v2id != nodes.first->getID() && v2id != nodes.second->getID())
			continue;
		return true;
	}
	return false;
}
/** Returns all Line objects of the model.
* @return All Line objects in a list.**/
std::vector <Line*> GridModel::getLines()
{
	return lines;
}

/** Adds a timeslot to the model.
* @param timeslot the timeslot to be added.**/
void GridModel::addTimeslot(int timeslot)
{
	if (!timeslotExist(timeslot))
	{
		timeslots.insert(timeslot);
	}
}
/** Returns all timeslots of the model.
* @return All timeslots in a set (ordered asc).**/
std::set<int> GridModel::getTimeslots()
{
	return timeslots;
}

/** Checks if a timeslot at given time exists.
* @param timeslot the timeslot to be checked.
* @return True when the timeslot exists and false when not.**/
bool GridModel::timeslotExist(int timeslot)
{
	return timeslots.count(timeslot) == 1;
}/** Removes a given timeslot from the model.
* @param timeslot the timeslot to be removed.**/
void GridModel::removeTimeslot(int timeslot)
{
	timeslots.erase(timeslot);
}

/** Checks if the GridModel is locked.
* @return True if GridModel is locked and false when not. **/
bool GridModel::isLocked()
{
	return locked;
}
/** Sets the lock state of the GridModel.
@param state the lock state to be set.**/
void GridModel::setLock(bool state)
{
	locked = state;
	if (locked)
	{
		generateNodeIndexMap();
		generateAdmittanceMatrix();
	}
}
/** Returns the static strings of all Node objects
* @return All static strings separated by \n. **/
std::string GridModel::serializeNodes()
{
	return propertyToString(nodes, &Node::serialize);
}
/** Returns the static strings of all Line objects
* @return All static strings separated by \n. **/
std::string GridModel::serializeLines()
{
	return propertyToString(lines, &Line::serialize);
}
/** Returns the dynamic strings of all Node objects
* @param times the set of timeslots to be contained in the dynamic strings
* @return All dynamic strings separated by \n. **/
std::string GridModel::serializeNodeStates(std::set<int> times)
{
	std::string nodesDynamic;
	for (int time : times)
	{
		nodesDynamic += propertyToString(nodes, &Node::serializeState, time) + "\n";
	}
	nodesDynamic.pop_back();
	//std::cout << nodesDynamic << std::endl;
	return nodesDynamic;
}
/** Returns the dynamic strings of all Line objects
* @param times the set of timeslots to be contained in the dynamic strings
* @return All dynamic strings separated by \n. **/
std::string GridModel::serializeLineStates(std::set<int> times)
{
	std::string linesDynamic;
	for (int time : times)
	{
		linesDynamic += propertyToString(lines, &Line::serializeState, time) + "\n";
	}
	linesDynamic.pop_back();
	return linesDynamic;
}

/** Returns the voltage of all Node objects at a timeslot.
* @param timeslot the timeslot
* @return A vector of all voltages in volt. **/
std::vector<double> GridModel::getNodeVoltages(int timeslot)
{
	std::vector<double> voltages;
	voltages.reserve(nodes.size());
	for (Node* v : nodes)
	{
		voltages.push_back(v->getVoltage(timeslot));
	}
	return voltages;
}
/** Returns the index of a Node in the admittance matrix.
* @param vid the id of the node.
* @return The index of the Node or -1 if the Node is not present in the admittance matrix.**/
int GridModel::getNodeIndex(std::string vid)
{
	if (nodeIndexMap.count(vid) == 0)
	{
		return -1;
	}
	return nodeIndexMap[vid];
}

/** Returns the index of a Node in the admittance matrix.
* @param v the Node object.
* @return The index of the Node or NULL if the Node is not present in the admittance matrix.**/
int GridModel::getNodeIndex(Node* v)
{
	if (v->getType() == Node::Types::VOLTAGE_CONTROLLER)
	{
		return -1;
	}
	return getNodeIndex(v->getID());
}

/** Returns the count of indexed nodes. This matches the number of all nodes excluding VoltageControllers.
* @return The size **/
int GridModel::getIndexedNodeSize()
{
	return nodeIndexMap.size();
}
/** Returns the admittance matrix of the actual grid in the model.
* @return the admittance matrix as a Matrix  object.
**/
Matrix* GridModel::getAdmittanceMatrix()
{
	return admittanceMatrix;
}
/** Returns the power setpoint of all CurrentSource objects in the grid at a given timeslot.
* @param timeslot the timeslot.
* @return The power setpoints as vector indexed based on nodeIndexMap.**/
std::vector<double> GridModel::getPowerSetpoints(int timeslot)
{
	std::vector<double> powerVector(getIndexedNodeSize(), 0);
	for (Node* v : nodes)
	{
		switch (v->getType())
		{
		case Node::Types::CURRENT_CONTROLLER:
			powerVector[getNodeIndex(v)] = ((CurrentSource*)v)->getPowerSetpoint(timeslot);
			break;
		case Node::Types::VOLTAGE_CONTROLLER:
		case Node::Types::COUPLER:
			break;
		}
	}
	return powerVector;
}
/** Returns all current setpoints of the nodes. This also includes equivalent currents of voltage controllers.
* @param timeslot the timeslot of the currents.
* @return A vector of currents and equivalent currents.**/
std::vector<double> GridModel::getCurrentSetpoints(int timeslot)
{
	std::vector<double> currentVector(nodeIndexMap.size(), 0);

	Line* l;
	for (Node* v : nodes)
	{
		switch (v->getType())
		{
		case Node::Types::VOLTAGE_CONTROLLER:
			l = v->getLines().front();
			currentVector[nodeIndexMap[l->getPartnerNode(v)->getID()]] = v->getVoltage(timeslot) / l->getResistance();
			break;
		case Node::Types::CURRENT_CONTROLLER:
			currentVector[getNodeIndex(v)] = ((CurrentSource*)v)->getCurrentSetpoint(timeslot);
			break;
		case Node::Types::COUPLER:
			break;
		}
	}
	return currentVector;
}
/** Returns the combined power loss of the lines in the grid at a timeslot.
* @param timeslot the timeslot
* @retun The power loss in watt.**/
double GridModel::getPowerLoss(int timeslot)
{
	double loss = 0;
	for (Line* l : lines)
	{
		loss += l->getPower(timeslot);
	}
	return loss;
}


bool GridModel::validateSetpoints()
{
	bool valid = true;

	for (int timeslot : timeslots)
	{
		if (!validateSetpoints(timeslot))
		{
			valid = false;
		}
	}
	return valid;
}

bool GridModel::validateSetpoints(int timeslot)
{
	bool valid = true;
	double deviation;
	double powerSetpoint;
	double currentSetpoint;
	double resultingPowerSetpoint;
	double power;
	double voltage;

	for (Node* v : nodes)
	{
		if (v->getType() == Node::Types::CURRENT_CONTROLLER) {
			powerSetpoint = ((CurrentSource*)v)->getPowerSetpoint(timeslot);
			currentSetpoint = ((CurrentSource*)v)->getCurrentSetpoint(timeslot);
			power = v->getPower(timeslot);
			voltage = v->getVoltage(timeslot);

			resultingPowerSetpoint = powerSetpoint + voltage * currentSetpoint;
			if (abs(power) < 1e-3)//if power close to zero set it to zero because deviation otherwise is 100%
			{
				power = 0;
			}
			if (abs(resultingPowerSetpoint) < 1e-3)//if power close to zero set it to zero because deviation otherwise is 100%
			{
				resultingPowerSetpoint = 0;
			}
			deviation = (1 - resultingPowerSetpoint / power);//relative deviation
			if (abs(deviation) > 0.02)
			{
				std::cerr << "Setpoint mismatch" << " " << *v << ", Timeslot: " << timeslot << ", Power calculated: " << power << ", Combined power setpoint: " << resultingPowerSetpoint << ", Power setpoint: " << powerSetpoint << ", Current setpoint: " << currentSetpoint << ", Relative deviation: " << deviation * 100 << "%" << std::endl;
				valid = false;
			}
		}
	}
	return valid;
}

bool GridModel::validateKirchhoff()
{
	bool valid = true;
	double currentSum;
	//TODO rewnew this old line and adapt to new structure
	//double kirchhoffCurrentLimit = solverType == "Spice" ? 0.4 : 0.1e-6;
	double kirchhoffCurrentLimit = 0.1e-6;
	for (int timeslot : timeslots)
	{
		for (Node* v : nodes)
		{
			if (v->getType() == Node::Types::COUPLER) {
				currentSum = 0;
				for (Line* l : v->getLines())
				{
					currentSum += l->getCurrentRelative(timeslot, v);
				}
				if (abs(currentSum) > kirchhoffCurrentLimit)
				{
					std::cerr << "Kirchhoff violated" << "  " << "timeslot: " << timeslot << ", " << *v << ", current: " << currentSum << std::endl;
					valid = false;
				}
			}
		}
	}
	return valid;
}

bool GridModel::validateKirchhoff(int timeslot)
{
	return false;
}

/** Return the closest existing timeslot before a given time.
* @param timeslot the timeslot from which the the closest existing timeslot before shall be returned.
* @return The closest existing timeslot. **/
int GridModel::getPreviousExistingTimeslot(int timeslot)
{
	std::set<int>::iterator it = timeslots.upper_bound(timeslot);
	--it;
	return *it;
}

/** Generates the nodeIndexMap **/
void GridModel::generateNodeIndexMap()
{
	nodeIndexMap.clear();
	for (Node* v : nodes)
	{
		if (v->getType() != Node::Types::VOLTAGE_CONTROLLER)
		{
			nodeIndexMap[v->getID()] = (int)nodeIndexMap.size();
		}
	}
}
/** Generates the admittance matrix of the grid. **/
void GridModel::generateAdmittanceMatrix()
{
	if (nodeIndexMap.size() < 2)
	{
		return;
	}
	admittanceMatrix = new Matrix((int)nodeIndexMap.size(), (int)nodeIndexMap.size());
	for (Node* v : nodes)
	{
		if (v->getType() == Node::Types::VOLTAGE_CONTROLLER)
		{
			continue;
		}
		for (Line* l : v->getLines())
		{
			admittanceMatrix->addValue(getNodeIndex(v->getID()), getNodeIndex(v), 1 / l->getResistance());
			if (l->getPartnerNode(v)->getType() != Node::Types::VOLTAGE_CONTROLLER)
			{
				admittanceMatrix->setValue(getNodeIndex(v), getNodeIndex(l->getPartnerNode(v)), -1 / l->getResistance());
			}
		}
	}
}

template<typename Object>
std::string GridModel::propertyToString(std::vector <Object* > objects, std::string(Object::* func)(void))
{
	std::string res;
	for (Object* o : objects) {
		res += (o->*func)() + "\n";
	}
	res.pop_back();
	return res;
}
template<typename Object>
std::string GridModel::propertyToString(std::vector <Object* > objects, std::string(Object::* func)(int), int timeslot)
{
	std::string res;
	for (Object* o : objects) {
		res += (o->*func)(timeslot) + "\n";
	}
	res.pop_back();
	return res;
}