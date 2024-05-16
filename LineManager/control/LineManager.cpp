#include "LineManager.h"
#include <string> 
#include "../model/gridElements/Coupler.h"
#include "../model/gridElements/CurrentController.h"
#include "../model/gridElements/VoltageController.h"
#include "../math/VectorTools.h"
#include "../Config.h"

/** Constructor, using Gradient solver as default.**/
LineManager::LineManager() :LineManager("Gradient") {}

/** Constructor.
* @param solver the type of solver to be used for grid state calculation.**/
LineManager::LineManager(std::string solver) : LineManager(solver, false)
{
}
/** Constructor.
* @param solver the type of solver to be used for grid state calculation.
* @param solverLogging true when the solver iterations shall be logged.**/
LineManager::LineManager(std::string solver, bool solverLogging)
{
	model = new DataModel();
	if (solverLogging)
	{
		solverLogger = new SolverLogging();
	}
	else
	{
		solverLogger = nullptr;
	}
	if (solver.find("_SIM") != std::string::npos)
	{
		mode = "SIM";
		solver.resize(solver.size() - 4);
	}
	solverType = solver;
	queue = new JobQueue(model, THREADLIMIT, solver);
	queue->run();
}

LineManager::~LineManager()
{
	delete model;
	delete solverLogger;
	delete queue;
}

/** Adds a VoltageController to the grid.
* @param id the id of the controller.
* @param voltage the voltage of the conbtroller.**/
bool LineManager::addVoltageController(std::string id, double voltage)
{
	//std::cout << "Add Voltage Controller " << id << std::endl;
	if (model->isLocked())
	{
		std::cerr << "Adding a Voltage Controller after already requesting packets is not allowed" << std::endl;
		return false;
	}
	if (model->nodeExist(id))
	{
		std::cerr << "Node " << id << " already exists in LineManager" << std::endl;
		return false;
	}
	model->addNode(new VoltageController(id, voltage));
	return true;
}
/** Adds a CurrentController to the grid.
* @param id the id of the controller.**/
bool LineManager::addCurrentController(std::string id)
{
	//std::cout << "Add Current Controller " << id << std::endl;
	if (model->isLocked())
	{
		std::cerr << "Adding a Current Controller after already requesting packets is not allowed" << std::endl;
		return false;
	}
	if (model->nodeExist(id))
	{
		std::cerr << "Node " << id << " already exists in LineManager" << std::endl;
		return false;
	}
	model->addNode(new CurrentController(id));
	return true;
}
/** Adds a Coupler to the grid.
* @param id the id of the coupler.**/
bool LineManager::addCoupler(std::string id)
{
	//std::cout << "Add Coupler " << id << std::endl;
	if (model->isLocked())
	{
		std::cerr << "Adding a Coupler after already requesting packets is not allowed" << std::endl;
		return false;
	}
	if (model->nodeExist(id))
	{
		std::cerr << "Node " << id << " already exists in LineManager" << std::endl;
		return false;
	}
	model->addNode(new Coupler(id));
	return true;
}
/** Adds a Line to the grid.
* @param v1id the id of the first Node.
* @param v2id the id of the second Node.
* length the length of the Line.**/
bool LineManager::addLine(std::string v1id, std::string v2id, double length)
{
	//std::cout << "Add Line between Node " << v1id << " and Node " << v2id << std::endl;
	if (model->isLocked())
	{
		std::cerr << "Adding a Line after already requesting packets is not allowed" << std::endl;
		return false;
	}

	if (v1id == v2id)
	{
		std::cerr << "Line between Node " << v1id << " with itself does not make sense" << std::endl;
		return false;
	}

	Node* v1 = model->getNode(v1id);
	Node* v2 = model->getNode(v2id);

	if (v1 == nullptr)
	{
		std::cerr << "Node " << v1id << " does not exist in LineManager" << std::endl;
		return false;
	}

	if (v2 == nullptr)
	{
		std::cerr << "Node " << v2id << " does not exist in LineManager" << std::endl;
		return false;
	}

	if (v1->getType() != Node::Types::COUPLER)
	{
		if (v2->getType() != Node::Types::COUPLER)
		{
			std::cerr << "Line between two Controllers is not allowed" << std::endl;
			return false;
		}
	}

	if (model->lineExist(v1id, v2id))
	{
		std::cerr << "Line between " << v1id << " and " << v2id << " already exists" << std::endl;
		return false;
	}

	Line* l = new Line(v1, v2, length);
	model->addLine(l);
	v1->addLine(l);
	v2->addLine(l);
	return true;
}
/** Adds a VoltageController to the grid.
* @param id the id of the controller.
* @param voltage the voltage of the conbtroller.**/
bool LineManager::addVoltageController(int id, double voltage)
{
	return addVoltageController(std::to_string(id), voltage);
}
/** Adds a CurrentController to the grid.
* @param id the id of the controller.**/
bool LineManager::addCurrentController(int id)
{
	return addCurrentController(std::to_string(id));
}
/** Adds a Coupler to the grid.
* @param id the id of the coupler.**/
bool LineManager::addCoupler(int id)
{
	return addCoupler(std::to_string(id));
}

/** Adds a Line to the grid.
* @param v1id the id of the first Node.
* @param v2id the id of the second Node.
* length the length of the Line.**/
bool LineManager::addLine(int v1id, int v2id, double length)
{
	return addLine(std::to_string(v1id), std::to_string(v2id), length);
}

/** Initializes the grid state at timeslot 0. **/
void LineManager::initGrid()
{
	model->setLock(true);
	if (model->getIndexedNodeSize() < 2)
	{
		return;
	}
	model->addTimeslot(0);
	model->addGridState(0, DataModel::States::STEADY);

	std::cout << "Calculating initial grid state" << std::endl;

	std::vector<double> zeros(model->getIndexedNodeSize(), 0);
	std::vector<double> voltages(model->getIndexedNodeSize(), VOLTAGE);
	queue->addJob(0, model->getCurrentSetpoints(0), zeros, voltages);
	std::pair<int, std::vector<double>> solution = queue->getNextResult();

	for (Node* v : model->getNodes())
	{
		if (v->getType() != Node::Types::VOLTAGE_CONTROLLER)
		{
			v->setVoltage(solution.second[model->getNodeIndex(v)], 0, true); //Set calculated voltages of all nodes at timeslot 0
		}
	}
	std::cout << "Initial grid state set" << std::endl;
}

/** Saves grid voltages.
* @param voltages a touple of a timeslot and a voltages vector of all non voltage controller nodes.
* @param end the timeslot when the state ends.
**/
void LineManager::saveVoltages(std::map<int, std::vector<double>> voltages, int end) {
	//	if (end != -1)
		//{
	int nextAfterEnd = end + 1;
	model->addTimeslot(nextAfterEnd);
	for (Node* v : model->getNodes())
	{
		if (v->getType() != Node::Types::VOLTAGE_CONTROLLER)
		{
			//std::cout << "Endtime: " << nextAfterEnd << " Voltage: " << v->getVoltage(nextAfterEnd) << std::endl;
			v->setVoltage(v->getVoltage(nextAfterEnd), nextAfterEnd, true); //Insert new voltage point to the Controller. Timeslot is the first slot after the inserted grid state and voltage value is the same as before the transfer. 
		}
	}
	//	}

		//std::cout << "slots: " << voltages.size() << " nodes: " << model->getNodes().size() << std::endl;
	for (std::pair<const int, std::vector<double>> voltagePoint : voltages)
	{
		model->addTimeslot(voltagePoint.first); //needed if timeslots are not added in requestEP
	//	std::cout << "Time: " << voltagePoint.first << std::endl;
		for (Node* v : model->getNodes())
		{
			if (v->getType() != Node::Types::VOLTAGE_CONTROLLER)
			{
				v->setVoltage(voltagePoint.second[model->getNodeIndex(v)], voltagePoint.first, false);
			}
		}
	}
}
/** Calculates and saves the states of the grid.
* @param setpoints a list of setpoints.
**/
void LineManager::saveGridStates(std::list<Setpoint*> setpoints)
{
	int nextAfterEnd = setpoints.back()->getBegin() + setpoints.back()->getDuration();
	model->addGridState(nextAfterEnd, model->getGridState(nextAfterEnd));


	std::map<int, DataModel::States> gridStates;
	DataModel::States last = DataModel::States::STEADY;
	for (Setpoint* sp : setpoints)
	{
		if (sp->getType() == Setpoint::Types::CONSTANT)
		{
			if (model->getGridState(sp->getBegin()) == DataModel::States::STEADY)
			{
				gridStates[sp->getBegin()] = DataModel::States::STEADY;//add new steady because of change (so still steady but change state)
				last = DataModel::States::STEADY;
			}
			/*if (model->getGridState(sp->getBegin() + sp->getDuration() - 1) != DataModel::States::DYNAMIC)
			{
				gridStates[sp->getBegin() + sp->getDuration() - 1] = DataModel::States::RAMP;
			}*/
		}
		else if (sp->getType() == Setpoint::Types::RAMP)
		{
			switch (model->getGridState(sp->getBegin()))
			{
			case DataModel::States::STEADY:
				if (last != DataModel::States::RAMP)
				{
					gridStates[sp->getBegin()] = DataModel::States::RAMP;
					last = DataModel::States::RAMP;
				}
				break;
			case DataModel::States::RAMP:
				if (last != DataModel::States::DYNAMIC)
				{
					gridStates[sp->getBegin()] = DataModel::States::DYNAMIC;
					last = DataModel::States::DYNAMIC;
				}
				break;
			case DataModel::States::DYNAMIC:
				break;
			}

		}
	}
	for (std::pair<int, DataModel::States> state : gridStates)
	{
		model->addGridState(state.first, state.second);
	}
}
/** Saves the setpoints of individual nodes of the grid.
* @param v1id the id of the first node.
* @param v2id the id of the second node.
* @param setpoints a list of Setpoint objects.
**/
void LineManager::saveSetpoints(std::string v1id, std::string v2id, std::list<Setpoint*> setpoints)
{
	CurrentController* v1 = NULL;
	CurrentController* v2 = NULL;
	if (model->getNode(v1id)->getType() == Node::Types::CURRENT_CONTROLLER)
	{
		v1 = (CurrentController*)model->getNode(v1id);
	}
	if (model->getNode(v2id)->getType() == Node::Types::CURRENT_CONTROLLER)
	{
		v2 = (CurrentController*)model->getNode(v2id);
	}

	for (Setpoint* sp : setpoints)
	{
		if (v1 != NULL)
		{
			if (sp->getSupplierPower() != 0)
			{
				v1->addPowerSetpoint(sp->getSupplierPower(), sp->getBegin(), sp->getDuration());
			}
			if (sp->getSupplierCurrent() != 0)
			{
				v1->addCurrentSetpoint(sp->getSupplierCurrent(), sp->getBegin(), sp->getDuration());
			}
		}

		if (v2 != NULL)
		{
			if (sp->getAbsorberPower() != 0)
			{
				v2->addPowerSetpoint(sp->getAbsorberPower(), sp->getBegin(), sp->getDuration());
			}
			if (sp->getAbsorberCurrent() != 0)
			{
				v2->addCurrentSetpoint(sp->getAbsorberCurrent(), sp->getBegin(), sp->getDuration());
			}
		}
	}
}
/** Requests an EP (energy packet)
* @param ep object of EP class containing the parameters of the energy packet.
* @return Return true or false whether the ep request is accepted or denied.
**/
bool LineManager::requestEP(EP* ep)
{
	std::string v1id = ep->getSupplierID();
	std::string v2id = ep->getAbsorberID();

	if (!validateEPParameters(v1id, v2id, ep->getBegin(), ep->getDuration()))
	{
		//std::cout << "Request rejected" << std::endl;
		return false;
	}
	//std::cout << "transfer v1: " << ep->getV1ID() << " v2: " << ep->getV2ID() << " begin:" << ep->getBegin() << std::endl;
	if (!model->isLocked())
	{
		initGrid();
	}

	//queue->run();
	for (Setpoint* sp : ep->getSetpoints())
	{
		if (mode == "LM")
		{
			requestSetpointLM(v1id, v2id, sp);
		}
		else if (mode == "SIM")
		{
			requestSetpointSim(v1id, v2id, sp);
		}
	}

	std::map<int, std::vector<double>> voltagesOverall;
	std::pair<int, std::vector<double>> voltageVector;


	while (!queue->isEmpty())
	{
		voltageVector = queue->getNextResult();
		if (!checkLimits(voltageVector.second, voltageVector.first))
		{
			queue->clear();
			return false;
		}
		voltagesOverall[voltageVector.first] = voltageVector.second;
	}
	//queue->stop();

	//EP Request is accepted.Save the voltages in each node.
	//std::cout << "Request accepted" << std::endl;
	saveGridStates(ep->getSetpoints());
	saveSetpoints(v1id, v2id, ep->getSetpoints());
	saveVoltages(voltagesOverall, ep->getEnd());
	return true;
}
/** Requests a single setpoint of an EP in simulation mode. This breaks down the Setpoint object to all relevant timeslots and adds them to JobQueue.
* @param v1id the id of the first node.
* @param v2id the id of the second node.
* @param setpoint a Setpoint object.
**/
void LineManager::requestSetpointSim(std::string v1id, std::string v2id, Setpoint* sp)
{
	int begin = sp->getBegin();
	std::set<int> times = model->getTimeslots();

	if (times.count(begin) == 0)
	{
		times.insert(begin);
	}
	for (std::set<int> ::iterator it = times.lower_bound(begin); it != times.lower_bound(begin + sp->getDuration()); ++it)
	{
		addSetpointToQueue(*it, v1id, v2id, sp->getSupplierPower(), sp->getSupplierCurrent(), sp->getAbsorberPower(), sp->getAbsorberCurrent());
	}
}
/** Requests a single Setpoint object of an EP in line management mode. This breaks down the Setpoint object to all relevant timeslots and adds them to JobQueue.
* @param v1id the id of the first node.
* @param v2id the id of the second node.
* @param setpoint a Setpoint object.
**/
void LineManager::requestSetpointLM(std::string v1id, std::string v2id, Setpoint* sp)
{
	int begin = sp->getBegin();
	//wenn Setpoint constant dann rechne
	//während duration rechne constant und sim ramp=> existierende slots
	//wenn Setpoint rampe dann rechne wenn netz rampe or dynamic
	std::set<int> times;
	switch (sp->getType())
	{
	case Setpoint::Types::CONSTANT:
		times = model->getTimeslots();
		if (times.count(begin) == 0)
		{
			times.insert(sp->getBegin());
		}
		for (std::set<int>::iterator it = times.lower_bound(begin); it != times.lower_bound(begin + sp->getDuration()); ++it)
		{
			addSetpointToQueue(*it, v1id, v2id, sp->getSupplierPower(), sp->getSupplierCurrent(), sp->getAbsorberPower(), sp->getAbsorberCurrent());
		}
		break;
	case Setpoint::Types::RAMP:
		if (model->timeslotExist(begin) || model->getGridState(begin) != DataModel::States::STEADY)
		{
			addSetpointToQueue(begin, v1id, v2id, sp->getSupplierPower(), sp->getSupplierCurrent(), sp->getAbsorberPower(), sp->getAbsorberCurrent());
		}
		break;
	}
}
/** Adds a single setpoint to JobQueue.
* @param timeslot the timeslot of the setpoint.
* @param v1id the id of the first node.
* @param v2id the id of the second node.
* @param v1Power the power setpoint of the first node.
* @param v1Current the current setpoint of the first node.
* @param v2Power the power setpoint of the second node.
* @param v2Current the current setpoint of the second node.
**/
void LineManager::addSetpointToQueue(int timeslot, std::string v1id, std::string v2id, double v1Power, double v1Current, double v2Power, double v2Current)
{
	std::vector<double> currents = model->getCurrentSetpoints(timeslot);
	std::vector<double> powers = model->getPowerSetpoints(timeslot);

	if (model->getNode(v1id)->getType() != Node::Types::VOLTAGE_CONTROLLER)
	{
		powers[model->getNodeIndex(v1id)] += v1Power;
		currents[model->getNodeIndex(v1id)] += v1Current;
	}
	if (model->getNode(v2id)->getType() != Node::Types::VOLTAGE_CONTROLLER)
	{
		powers[model->getNodeIndex(v2id)] += v2Power;
		currents[model->getNodeIndex(v2id)] += v2Current;
	}

	queue->addJob(timeslot, currents, powers, calculateOperatingPoint(timeslot));
}

/** Checks if line or voltage limits are exceeded.
* @param voltages the voltages of all non voltage controller nodes.
* @param timeslot the timeslot of the grid state.
* @return False when a limit is exceeded and true when not.**/
bool LineManager::checkLimits(std::vector<double> voltages, int timeslot)
{
	if (SKIP_LIMIT_CHECK)
	{
		return true;
	}
	//Check for voltage at nodes exceeding limits
	for (int i = 0; i < voltages.size(); i++)
	{
		if (voltages[i] > MAX_VOLTAGE)
		{
			std::cout << "Voltage is exceeding upper bounds" << std::endl;
			return false;
		}
		if (voltages[i] < MIN_VOLTAGE)
		{
			std::cout << "Voltage is exceeding lower bounds" << std::endl;
			return false;
		}
		if (!isfinite(voltages[i])) {
			std::cerr << "Voltage is " << voltages[i] << ". This is a calculation issue and should not happen." << std::endl;
			exit(1);
		}
	}
	//Check for currents exceeding line limits
	for (Line* l : model->getLines())
	{
		std::pair<Node*, Node*> lineNodes = l->getNodes();
		double u1 = 0;
		double u2 = 0;
		if (lineNodes.first->getType() != Node::Types::VOLTAGE_CONTROLLER)
		{
			u1 = voltages[model->getNodeIndex(lineNodes.first)];
		}
		else
		{
			u1 = lineNodes.first->getVoltage(timeslot);
		}
		if (lineNodes.second->getType() != Node::Types::VOLTAGE_CONTROLLER)
		{
			u2 = voltages[model->getNodeIndex(lineNodes.second)];
		}
		else
		{
			u2 = lineNodes.second->getVoltage(timeslot);
		}
		if (u1 == 0 || u2 == 0)
		{
			continue;
		}

		double current = abs(u1 - u2) / l->getResistance();
		if (current > l->getLimit())
		{
			std::cout << "Line capacity exceeded" << std::endl;
			return false;
		}
	}
	return true;
}
/** Checks parameters of a requested energy packet transfer.
* @param v1id the id of the supplier.
* @param v2id the id of the absorber.
* @param begin the beginning timeslot of the packet.
* @param duration the duration of the packet.
* @return True when parameters are valid and false when not.**/
bool LineManager::validateEPParameters(std::string v1id, std::string v2id, int begin, int duration)
{
	Node* v1 = model->getNode(v1id);
	Node* v2 = model->getNode(v2id);
	if (v1 == nullptr)
	{
		std::cerr << "Node " << v1id << " does not exist in LineManager" << std::endl;
		return false;
	}

	if (v2 == nullptr)
	{
		std::cerr << "Node " << v2id << " does not exist in LineManager" << std::endl;
		return false;
	}

	if (v1->getType() == Node::Types::COUPLER)
	{
		std::cerr << "Coupler can not be a participant in an EP transfer" << std::endl;
		return false;
	}
	if (v2->getType() == Node::Types::COUPLER)
	{
		std::cerr << "Coupler can not be a participant in an EP transfer" << std::endl;
		return false;
	}
	if (v1->getType() == Node::Types::VOLTAGE_CONTROLLER && v2->getType() == Node::Types::VOLTAGE_CONTROLLER)
	{
		std::cerr << "An EP transfer between two voltage controllers is not possible at the moment" << std::endl;
		//In theroy this should be possible by changing the cotnrollers voltage.
		return false;
	}

	if (v1id == v2id)
	{
		std::cerr << "Node can not exchange an EP with itself" << std::endl;
		return false;
	}
	if (begin < 0)
	{
		std::cerr << "Begin of EP transfer can not be negative" << std::endl;
		return false;
	}
	if (duration < 1)
	{
		std::cerr << "Transfer hast to last at least one period" << std::endl;
		return false;
	}
	return true;
}
/** Calculates the operating point voltages at a given timeslot.
* @param timeslot the timeslot of the operating point.
* @param the voltage vector at the operating point.**/
std::vector<double> LineManager::calculateOperatingPoint(int timeslot)
{
	std::vector<double> voltageWorkingPoint(model->getIndexedNodeSize(), VOLTAGE);
	for (Node* v : model->getNodes())
	{
		if (v->getType() == Node::Types::VOLTAGE_CONTROLLER)
		{
			continue;
		}
		if (v->getVoltage(timeslot) == NULL)//if node has no voltage yet
		{
			continue;
		}
		voltageWorkingPoint[model->getNodeIndex(v)] = v->getVoltage(timeslot);
	}
	return voltageWorkingPoint;
}
/** Validates wether Kirchhoff rules are fullfilled at all timeslots of the grid. 
* @return true when rule is fullfilled all the time and false when not.**/
bool LineManager::validateKirchhoff()
{
	bool valid = true;
	double currentSum;
	double kirchhoffCurrentLimit = solverType == "Spice" ? 0.4 : 0.1e-6;
	for (int timeslot : model->getTimeslots())
	{
		for (Node* v : model->getNodes())
		{
			if (v->getType() == Node::Types::COUPLER) {
				currentSum = 0;
				for (Line* l : v->getLines())
				{
					currentSum += l->getCurrentRelative(timeslot, v);
				}
				if (abs(currentSum) > kirchhoffCurrentLimit)
				{
					std::cerr << "Kirchhoff violated" << "  " << "	solver: " << solverType << ", timeslot: " << timeslot << ", " << *v << ", current: " << currentSum << std::endl;
					valid = false;
				}
			}
		}
	}
	return valid;
}
/** Validates wether all setpoints are fullfilled at all timeslots of the grid.
* @return true when setpoints are fullfilled all the time and false when not.**/
bool LineManager::validateSetpoints()
{
	bool valid = true;
	double deviation;
	double powerSetpoint;
	double currentSetpoint;
	double resultingPowerSetpoint;
	double power;
	double voltage;
	if (mode != "SIM")
	{
		std::cout << "Skip validation of setpoints because LM is not in SIM mode. " << std::endl;
		return true;
	}
	for (int timeslot : model->getTimeslots())
	{
		for (Node* v : model->getNodes())
		{
			if (v->getType() == Node::Types::CURRENT_CONTROLLER) {
				powerSetpoint = ((CurrentController*)v)->getPowerSetpoint(timeslot);
				currentSetpoint = ((CurrentController*)v)->getCurrentSetpoint(timeslot);
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
					std::cerr << "Setpoint mismatch" << " " << "	Solver: " << solverType << ", " << *v << ", Timeslot: " << timeslot << ", Power calculated: " << power << ", Combined power setpoint: " << resultingPowerSetpoint << ", Power setpoint: " << powerSetpoint << ", Current setpoint: " << currentSetpoint << ", Relative deviation: " << deviation * 100 << "%" << std::endl;
					valid = false;
				}
			}
		}
	}
	return valid;
}

/** Checks if every stored grid state fullfills kirchhoff law and all endpoints achieve their setpoint. Exits the programm when a problem is detected.
* @return True when every grid state is valid.**/
bool LineManager::validateGridStates()
{
	bool kirchhoff = validateKirchhoff();
	bool setpoints = validateSetpoints();
	return 	kirchhoff && setpoints;
}
/** Returns the DataModel used by the LineManager.
* @return A DataModel.**/
DataModel* LineManager::getDataModelReference()
{
	return model;
}
/** Returns the SolverLogging used by the LineManager.
* @return A SolverLogging.**/
SolverLogging* LineManager::getSolverLoggingReference()
{
	return solverLogger;
	//TODO this will probably not work due to changes in JobQueue
}
