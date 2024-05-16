#pragma once
#include "SimulationController.h"
#include "../model/gridElements/Node.h"
#include "../model/gridElements/Line.h"
#include "../model/GridModel.h"
#include "../model/packets/EP.h"
#include "GridStateSolvers/Solver.h"



/** Class which handles the task of line management. **/
class LineManager
{
public:
	LineManager(GridModel* model);
	LineManager(GridModel* model, std::string solver);
	LineManager(GridModel* model, std::string solver, bool solverLogging);
	~LineManager();
	void initGrid();
	bool requestEP(EP* ep);
	bool validateGridStates();
	GridModel* getGridModelReference();
	SolverLogging* getSolverLoggingReference();

private:
	enum class GridStates { STEADY, RAMP, DYNAMIC };
	std::string mode = "LM";
	std::string solverType;
	GridModel* model;
	SolverLogging* solverLogger;
	SimulationController* simControl;
	std::map<int, GridStates> gridStates;
	void saveVoltages(std::map<int, std::vector<double>> solutions, int end);
	void saveGridStates(std::list<Setpoint*> setpoints);
	void saveSetpoints(std::string v1id, std::string v2id, std::list<Setpoint*> setpoints);
	void requestSetpointSim(std::string v1id, std::string v2id, Setpoint* sp);
	void requestSetpointLM(std::string v1id, std::string v2id, Setpoint* sp);
	void addSetpointToQueue(int timeslot, std::string v1id, std::string v2id, double v1Power, double v1Current, double v2Power, double v2Current);
	void addGridState(int timeslot, GridStates state);
	bool gridStateExist(int timeslot);
	GridStates getGridState(int timeslot);
	std::map<int, GridStates> getGridStates();
	bool checkLimits(std::vector<double> voltages, int timeslot);
	bool validateEPParameters(std::string v1id, std::string v2id, int begin, int duration);
	std::vector<double> calculateOperatingPoint(int timeslot);
	bool validateKirchhoff();
	bool validateSetpoints();
};