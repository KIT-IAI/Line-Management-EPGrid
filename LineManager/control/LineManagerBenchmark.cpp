#include "LineManagerBenchmark.h"
#include "LineManagerHelper.h"
#include "../config.h"
#include "../math/VectorTools.h"
#include <chrono>
#include <string>

/** Constructor.
* @param solver a string containing a solver type to be used for the LineManager.
**/
LineManagerBenchmark::LineManagerBenchmark(std::string solver)
{
	managers.reserve(1);
	names.reserve(1);
	managers.push_back(new LineManager(solver));
	names.push_back(solver);
	visualizer = new VisualizerCurve();
	computationTimes = std::vector<std::vector<double>>(managers.size(), std::vector<double>());
	decisions = std::vector<std::vector<bool>>(managers.size(), std::vector<bool>());

}
/** Constructor.
* @param solver a list of strings each containing a solver type to be used for a LineManager.
**/
LineManagerBenchmark::LineManagerBenchmark(std::vector<std::string> solvers) :LineManagerBenchmark(solvers, false)
{
}
/** Constructor.
* @param solver a list of strings each containing a solver type to be used for a LineManager.
* @param solverLogging true when the solver iterations shall be logged.**/
LineManagerBenchmark::LineManagerBenchmark(std::vector<std::string> solvers, bool solverLogging)
{
	managers.reserve(solvers.size());
	names.reserve(solvers.size());

	for (std::string solver : solvers)
	{
		managers.push_back(new LineManager(solver, solverLogging));
		names.push_back(solver);
	}
	visualizer = new VisualizerCurve();
	computationTimes = std::vector<std::vector<double>>(managers.size(), std::vector<double>());
	decisions = std::vector<std::vector<bool>>(managers.size(), std::vector<bool>());
}

/** Destructor **/
LineManagerBenchmark::~LineManagerBenchmark()
{
	delete visualizer;
	for (LineManager* lm : managers)
	{
		delete lm;
	}
}

/** Requests a randomized set of EPPowerRectangular transfers from all instances of LineManager. Requested transfers are the same for all LineManagers.
* @param count the number of requests.
* @param maxBegin the maximum beginning time of an EPPowerRectangular.
* @param maxDuration the maximum duration of an EPPowerRectangular.
* @param maxPower the maximum power of an EPPowerRectangular.
**/
void LineManagerBenchmark::generateRequestsPowerRectangular(int count, int maxBegin, int maxDuration, int maxPower)
{
	bool decision;
	EP* ep;
	LineManagerHelper::renewSeed();
	for (int i = 0; i < managers.size(); i++)
	{
		LineManagerHelper::resetRandomization();

		for (int j = 0; j < count; j++)
		{		
			ep=LineManagerHelper::getRandomPowerRectangular(managers[i], 0,maxBegin, 1,maxDuration, 100,maxPower);
			std::cout << "Request: " << " LineManager " << i << " packet " << j + 1 << "/" << count << std::endl;
			auto t1 = std::chrono::high_resolution_clock::now();	
			decision = managers[i]->requestEP(ep);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> duration = t2 - t1;
			computationTimes[i].push_back(duration.count());
			decisions[i].push_back(decision);
			delete ep;
		}
	}
}

/** Requests a randomized set of EPPowerTrapeze transfers from all instances of LineManager. Requested transfers are the same for all LineManagers.
* @param count the number of requests.
* @param maxBegin the maximum beginning time of an EPPowerTrapeze.
* @param maxPower the maximum power of an EPPowerTrapeze.
* @param maxRampPower the maximum ramping power of an EPPowerTrapeze.
* @param maxEnergy the maximum energy of an EPPowerTrapeze.
**/
void LineManagerBenchmark::generateRequestsPowerTrapeze(int count, int maxBegin, int maxPower, int maxRampPower, int maxEnergy)
{
	bool decision;
	EP* ep;
	LineManagerHelper::renewSeed();
	for (int i = 0; i < managers.size(); i++)
	{
		LineManagerHelper::resetRandomization();

		for (int j = 0; j < count; j++)
		{
			ep=LineManagerHelper::getRandomPowerTrapeze(managers[i],0, maxBegin,2000, maxPower,5000, maxRampPower,1, maxEnergy);
			std::cout << "Request: " << " LineManager "<<i << " packet " << j + 1 << "/" << count << std::endl;
			auto t1 = std::chrono::high_resolution_clock::now();
			decision = managers[i]->requestEP(ep);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> duration = t2 - t1;
			computationTimes[i].push_back(duration.count());
			decisions[i].push_back(decision);
			delete ep;
		}
	}
}
/** Requests a randomized set of EPCurrentRectangular transfers from all instances of LineManager. Requested transfers are the same for all LineManagers.
* @param count the number of requests.
* @param maxBegin the maximum beginning time of an EPCurrentRectangular.
* @param maxDuration the maximum duration of an EPCurrentRectangular.
* @param maxCurrent the maximum current of an EPCurrentRectangular.
**/
void LineManagerBenchmark::generateRequestsCurrentRectangular(int count, int maxBegin, int maxDuration, int maxCurrent)
{
	bool decision;
	EP* ep;
	LineManagerHelper::renewSeed();
	for (int i = 0; i < managers.size(); i++)
	{
		LineManagerHelper::resetRandomization();

		for (int j = 0; j < count; j++)
		{
			ep = LineManagerHelper::getRandomCurrentRectangular(managers[i], 0, maxBegin, 1, maxCurrent, 1, maxDuration);
			std::cout << "Request: " << " LineManager " << i << " packet " << j + 1 << "/" << count << std::endl;
			auto t1 = std::chrono::high_resolution_clock::now();
			decision = managers[i]->requestEP(ep);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> duration = t2 - t1;
			computationTimes[i].push_back(duration.count());
			decisions[i].push_back(decision);
			delete ep;
		}
	}
}
/** Requests a randomized set of EPCurrentTrapeze transfers from all instances of LineManager. Requested transfers are the same for all LineManagers.
* @param count the number of requests.
* @param maxBegin the maximum beginning time of an EPCurrentTrapeze.
* @param maxCurrent the maximum current of an EPCurrentTrapeze.
* @param maxRampCurrent the maximum ramping current of an EPCurrentTrapeze.
* @param maxDuration the maximum duration of an EPCurrentTrapeze.
**/
void LineManagerBenchmark::generateRequestsCurrentTrapeze(int count, int maxBegin, int maxCurrent, int maxRampCurrent, int maxDuration)
{
	bool decision;
	EP* ep;
	LineManagerHelper::renewSeed();
	for (int i = 0; i < managers.size(); i++)
	{
		LineManagerHelper::resetRandomization();

		for (int j = 0; j < count; j++)
		{
			ep = LineManagerHelper::getRandomCurrentTrapeze(managers[i], 0, maxBegin, 1, maxCurrent, 1, maxRampCurrent, 1, maxDuration);
			std::cout << "Request: " << " LineManager " << i << " packet " << j + 1 << "/" << count << std::endl;
			auto t1 = std::chrono::high_resolution_clock::now();
			decision = managers[i]->requestEP(ep);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> duration = t2 - t1;
			computationTimes[i].push_back(duration.count());
			decisions[i].push_back(decision);
			delete ep;
		}
	}
}

/** Request the same EP from all instances of LineManager.
* @param ep object of EP class containing the parameters of the energy packet.**/
void LineManagerBenchmark::requestAll(EP* ep)
{
	bool decision;
	for (int i = 0; i < managers.size(); i++)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		decision = managers[i]->requestEP(ep);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> duration = t2 - t1;
		computationTimes[i].push_back(duration.count());
		decisions[i].push_back(decision);
	}
}

/** Creates the same grid network for all instances of LineManager. **/
void LineManagerBenchmark::setupGridNetwork(int participantCount)
{
	for (LineManager* lm : managers)
	{
		LineManagerHelper::setupGridNetwork(lm, participantCount);
	}
}
/** Creates the same partly randomized network for all instances of LineManager. **/
void LineManagerBenchmark::setupPartlyRandomizedNetwork(int participantCount)
{

	for (LineManager* lm : managers)
	{
		LineManagerHelper::resetRandomization();
		LineManagerHelper::setupPartlyRandomizedNetwork(lm, participantCount);
	}
}
/** Creates the reference grid for all instances of LineManager. **/
void LineManagerBenchmark::setupReferenceGrid()
{
	for (LineManager* lm : managers)
	{
		LineManagerHelper::setupReferenceGrid(lm);
	}
}

/** Displays the voltage curve of a Node from each LineManager instance.
* @param id the id of the Node.**/
void LineManagerBenchmark::compareVoltages(std::string vid)
{
	visualizer->setYAxesLabel("voltage/V");
	visualizer->setTitle("Voltage of node " + vid);
	compare(vid, &Node::getVoltage);
}
/** Displays the current curve of a Node from each LineManager instance.
* @param id the id of the Node.**/
void LineManagerBenchmark::compareCurrents(std::string vid)
{
	visualizer->setYAxesLabel("current/A");
	visualizer->setTitle("Current of node " + vid);
	compare(vid, &Node::getCurrent);
}
/** Displays the power curve of a Node from each LineManager instance.
* @param id the id of the Node.**/
void LineManagerBenchmark::comparePowers(std::string vid)
{
	visualizer->setYAxesLabel("power/W");
	visualizer->setTitle("Power of node " + vid);
	compare(vid, &Node::getPower);
}

/** Displays the voltage curve of a Line from each LineManager instance.
* @param line A line described a a pair of node ids.**/
void LineManagerBenchmark::compareVoltages(std::pair<std::string, std::string> line)
{
	visualizer->setYAxesLabel("voltage/V");
	visualizer->setTitle("Voltage of line " + line.first + ":" + line.second);
	compare(line, &Line::getVoltage);
}

/** Displays the current curve of a Line from each LineManager instance.
* @param line A line described a a pair of node ids.**/
void LineManagerBenchmark::compareCurrents(std::pair<std::string, std::string> line)
{
	visualizer->setYAxesLabel("current/A");
	visualizer->setTitle("Currents of line " + line.first + ":" + line.second);
	compare(line, &Line::getCurrent);
}

/** Displays the power curve of a Line from each LineManager instance.
* @param line A line described a a pair of node ids.**/
void LineManagerBenchmark::comparePowers(std::pair<std::string, std::string> line)
{
	visualizer->setYAxesLabel("power/W");
	visualizer->setTitle("Power of line " + line.first + ":" + line.second);
	compare(line, &Line::getPower);
}

/** Displays the computation time of each request for each LineManager instance.**/
void LineManagerBenchmark::compareComputationTimes()
{
	visualizer->setXAxesLabel("request");
	visualizer->setYAxesLabel("computation time/ms");
	visualizer->setTitle("Computation Times of requests");
	for (int i = 0; i < computationTimes.size(); i++)
	{
		visualizer->addCurve(computationTimes[i], names[i]);
	}
	visualizer->visualize(true, false,true);
}

void LineManagerBenchmark::compareVoltageDeviation(int index, bool showReference)
{
	double xFactor;
	if (PLOT_TIMESLOT)
	{
		visualizer->setXAxesLabel("timeslot/" + std::to_string(S_PER_TIMESLOT) + "s");
		xFactor = 1;
	}
	else
	{
		visualizer->setXAxesLabel("time/s");
		xFactor = S_PER_TIMESLOT;
	}
	visualizer->setYAxesLabel("||x-x_spice||_2");
	visualizer->setTitle("Accuracy");

	std::set<int> timeslots = managers[index]->getDataModelReference()->getTimeslots();
	int end = *(managers[index]->getDataModelReference()->getTimeslots().rbegin());
	timeslots.insert(end + 1);

	std::map<int, double> curve;
	std::vector<double> dx(managers[index]->getDataModelReference()->getNodes().size(), 0);
	std::vector<double> voltages;
	std::vector<double> refVoltages;
	for (int i = 0; i < managers.size(); i++) {
		if (!showReference && i == index)
		{
			continue;
		}
		for (int time : timeslots) {
			voltages = managers[i]->getDataModelReference()->getNodeVoltages(time);
			refVoltages = managers[index]->getDataModelReference()->getNodeVoltages(time);
			VectorTools::subtract(&voltages, &refVoltages, &dx);//dx = xnew-x	
			curve[time] = VectorTools::norm(dx);
		}
		visualizer->addCurve(curve,xFactor, names[i]);
	}
	visualizer->visualize(false, true,true);
}

/** Return a LineManager instance of the LineManagerBenchmark.
* @param index the index of the LienManager. Matches the order of solvers in the constructor.
* @Return The requested LineManager instance.**/
LineManager* LineManagerBenchmark::getLineManager(int index)
{
	return managers[index];
}

/** Calls the validateGridStates() method of all LineManager objects.**/
void LineManagerBenchmark::validateGridStates()
{
	for (LineManager* lm : managers)
	{
		lm->validateGridStates();
	}
}

/** Validates wether all Line Managers have made the same decision on each request.**/
void LineManagerBenchmark::validateDecisions()
{
	std::vector<bool> singleRequestDecisions;
	singleRequestDecisions.reserve(managers.size());
	for (int i = 0; i < decisions[0].size(); i++)
	{
		for (int j = 0; j < managers.size(); j++)
		{
			singleRequestDecisions.push_back(decisions[j][i]);
		}
		compareDecision(singleRequestDecisions);
	}
}

/** Generic compare method for comparing voltage, current, power curve of a Node from each LineManager instance. **/
void LineManagerBenchmark::compare(std::string vid, double(Node::* func)(int))
{
	for (int i = 0; i < managers.size(); i++) {
		visualizer->buildCurve(managers[i]->getDataModelReference()->getNode(vid), func, managers[i]->getDataModelReference()->getTimeslots(), names[i], true);
	}
	visualizer->visualize(false, true,false);
}

/** Generic compare method for comparing voltage, current, power curve of a Line from each LineManager instance. **/
void LineManagerBenchmark::compare(std::pair<std::string, std::string> line, double(Line::* func)(int))
{
	for (int i = 0; i < managers.size(); i++) {
		visualizer->buildCurve(managers[i]->getDataModelReference()->getLine(line.first, line.second), func, managers[i]->getDataModelReference()->getTimeslots(), names[i], true);
	}
	visualizer->visualize(false, true,false);
}

/** Compares the decisions of all LineManager objects.
* @Return True when all LineManager objects made the same decision.**/
bool LineManagerBenchmark::compareDecision(std::vector<bool> decisions)
{
	bool last = decisions[0];
	for (int i = 1; i < managers.size(); i++)
	{
		if (decisions[i] != last)
		{
			std::cerr << "Decision of different Line Managers not the same." << std::endl;
			exit(1);
			return false;
		}
		last = decisions[i];
	}
	return true;
}