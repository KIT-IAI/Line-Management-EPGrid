#pragma once
#include "LineManager.h"
#include "../view/VisualizerCurve.h"


/** Static Class which provides methods to initialize a LineManager and emulate EP requests. **/
class LineManagerBenchmark
{
public:
	LineManagerBenchmark();
	~LineManagerBenchmark();
	bool addLineManager(LineManager* lm, std::string name);
	bool addLineManagers(std::vector<std::string> solverNames, GridModel* (*func)());
	bool addLineManagers(std::vector<std::string> solverNames, GridModel* (*func)(int),int);
	bool addLineManagers(std::vector<std::string> solverNames, GridModel* (*func)(std::string), std::string);
	void reset();
	void requestEp(EP* ep);
	void requestEps(std::vector<EP*> eps);
	void compareVoltages(std::string vid);
	void compareCurrents(std::string vid);
	void comparePowers(std::string vid);
	void compareVoltages(std::pair<std::string, std::string> line);
	void compareCurrents(std::pair<std::string, std::string> line);
	void comparePowers(std::pair<std::string, std::string> line);
	void compareComputationTimes();
	void compareVoltageDeviation(int index,bool showReference);
	LineManager* getLineManager(int index);
	void validateGridStates();
	void validateDecisions();
private:
	std::vector<LineManager*> managers;
	std::vector<std::string> names;
	bool lock;

	VisualizerCurve* visualizer;
	std::vector<std::vector<double>> computationTimes;
	std::vector<std::vector<bool>> decisions;
	void requestEp(EP* ep, int managerIndex);
	bool compareDecision(std::vector<bool> decisions);
	void compare(std::string vid, double(Node::* func)(int));
	void compare(std::pair<std::string, std::string> line, double(Line::* func)(int));
};


