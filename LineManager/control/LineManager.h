#pragma once
#include "JobQueue.h"
#include "../model/gridElements/Node.h"
#include "../model/gridElements/Line.h"
#include "../model/DataModel.h"
#include "../model/packets/EP.h"
#include "GridStateSolvers/Solver.h"



/** Class which handles the task of line management. **/
class LineManager
{
public:
	LineManager();
	LineManager(std::string solver);
	LineManager(std::string solver, bool solverLogging);
	~LineManager();
	bool addVoltageController(std::string id, double voltage);
	bool addCurrentController(std::string id);
	bool addCoupler(std::string id);
	bool addLine(std::string v1id, std::string v2id, double length);
	bool addVoltageController(int id, double voltage);
	bool addCurrentController(int id);
	bool addCoupler(int id);
	bool addLine(int v1id, int v2id, double length);
	void initGrid();
	bool requestEP(EP* ep);
	bool validateGridStates();
	DataModel* getDataModelReference();
	SolverLogging* getSolverLoggingReference();

private:
	std::string mode = "LM";
	std::string solverType;
	DataModel* model;
	SolverLogging* solverLogger;
	JobQueue* queue;
	void saveVoltages(std::map<int, std::vector<double>> solutions, int end);
	void saveGridStates(std::list<Setpoint*> setpoints);
	void saveSetpoints(std::string v1id, std::string v2id, std::list<Setpoint*> setpoints);
	void requestSetpointSim(std::string v1id, std::string v2id, Setpoint* sp);
	void requestSetpointLM(std::string v1id, std::string v2id, Setpoint* sp);
	void addSetpointToQueue(int timeslot, std::string v1id, std::string v2id, double v1Power, double v1Current, double v2Power, double v2Current);
	bool checkLimits(std::vector<double> voltages, int timeslot);
	bool validateEPParameters(std::string v1id, std::string v2id, int begin, int duration);
	std::vector<double> calculateOperatingPoint(int timeslot);
	bool validateKirchhoff();
	bool validateSetpoints();
};