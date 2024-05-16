#pragma once
#include "LineManager.h"
#include "../view/VisualizerCurve.h"

/** Static Class which provides methods to initialize a LineManager and emulate EP requests. **/
class LineManagerBenchmark
{
public:
	LineManagerBenchmark(std::string solver);
	LineManagerBenchmark(std::vector<std::string> solvers);
	LineManagerBenchmark(std::vector<std::string> solvers, bool solverLogging);
	~LineManagerBenchmark();
	void generateRequestsPowerRectangular(int count, int maxBegin, int maxDuration, int maxPower);
	void generateRequestsPowerTrapeze(int count, int maxBegin, int maxPower, int maxRampPower, int maxEnergy);
	void generateRequestsCurrentRectangular(int count, int maxBegin, int maxDuration, int maxCurrent);
	void generateRequestsCurrentTrapeze(int count, int maxBegin, int maxCurrent, int maxRampCurrent, int maxDuration);
	void requestAll(EP* ep);
	void setupGridNetwork(int participantCount);
	void setupPartlyRandomizedNetwork(int participantCount);
	void setupReferenceGrid();
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
	VisualizerCurve* visualizer;
	std::vector<std::vector<double>> computationTimes;
	std::vector<std::vector<bool>> decisions;
	bool compareDecision(std::vector<bool> decisions);
	void compare(std::string vid, double(Node::* func)(int));
	void compare(std::pair<std::string, std::string> line, double(Line::* func)(int));
};
