#include "LineManagerBenchmark.h"
#include "EPGenerator.h"
#include "../model/parser/SimBenchParser.h"
#include "../model/GridModelGenerator.h"
#include "../model/GridModel.h"
#include "../math/Randomizer.h"
#include "../config.h"
#include "../math/VectorTools.h"
#include <chrono>
#include <string>

/** Constructor.**/
LineManagerBenchmark::LineManagerBenchmark()
{
	managers.reserve(1);
	names.reserve(1);
	lock = false;
	//managers.push_back(new LineManager(solver));
	//names.push_back(solver);

	visualizer = new VisualizerCurve();

	//computationTimes = std::vector<std::vector<double>>(managers.size(), std::vector<double>());
	//decisions = std::vector<std::vector<bool>>(managers.size(), std::vector<bool>());

	//managers.reserve(solvers.size());
//names.reserve(solvers.size());
//visualizer = new VisualizerCurve();
//computationTimes = std::vector<std::vector<double>>(managers.size(), std::vector<double>());
//decisions = std::vector<std::vector<bool>>(managers.size(), std::vector<bool>());

}

/** Destructor **/
LineManagerBenchmark::~LineManagerBenchmark()
{
	delete visualizer;
}

bool LineManagerBenchmark::addLineManager(LineManager* lm, std::string name)
{
	if (lock)
	{
		return false;
	}
	managers.push_back(lm);
	names.push_back(name);
	return true;
}

bool LineManagerBenchmark::addLineManagers(std::vector<std::string> solverNames, GridModel* (*func)())
{
	if (lock)
	{
		return false;
	}
	for (std::string solverName : solverNames)
	{
		addLineManager(new LineManager(func(), solverName), solverName);
	}
	return true;
}

bool LineManagerBenchmark::addLineManagers(std::vector<std::string> solverNames, GridModel* (*func)(int), int x)
{
	if (lock)
	{
		return false;
	}
	for (std::string solverName : solverNames)
	{
		addLineManager(new LineManager(func(x), solverName), solverName);
	}
	return true;
}

bool LineManagerBenchmark::addLineManagers(std::vector<std::string> solverNames, GridModel* (*func)(std::string), std::string x)
{
	if (lock)
	{
		return false;
	}
	for (std::string solverName : solverNames)
	{
		addLineManager(new LineManager(func(x), solverName), solverName);
	}
	return true;
}

void LineManagerBenchmark::reset()
{
	for (LineManager* lm : managers)
	{
		delete lm;
	}
	managers.clear();
	names.clear();
	computationTimes.clear();
	decisions.clear();
	lock = false;
}

/** Request the same EP from all instances of LineManager.
* @param ep object of EP class containing the parameters of the energy packet.**/
void LineManagerBenchmark::requestEp(EP* ep)
{
	for (int i = 0; i < managers.size(); i++)
	{
		requestEp(ep, i);
	}
}

void LineManagerBenchmark::requestEps(std::vector<EP*> eps)
{
	for (int i = 0; i < managers.size(); i++)
	{
		for (int j = 0; j < eps.size(); j++)
		{
			std::cout << "Request: " << " LineManager " << i << " packet " << j + 1 << "/" << eps.size() << std::endl;
			requestEp(eps[j], i);
		}
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
	visualizer->visualize(true, false, true);
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

	std::set<int> timeslots = managers[index]->getGridModelReference()->getTimeslots();
	int end = *(managers[index]->getGridModelReference()->getTimeslots().rbegin());
	timeslots.insert(end + 1);

	std::map<int, double> curve;
	std::vector<double> dx(managers[index]->getGridModelReference()->getNodes().size(), 0);
	std::vector<double> voltages;
	std::vector<double> refVoltages;
	for (int i = 0; i < managers.size(); i++) {
		if (!showReference && i == index)
		{
			continue;
		}
		for (int time : timeslots) {
			voltages = managers[i]->getGridModelReference()->getNodeVoltages(time);
			refVoltages = managers[index]->getGridModelReference()->getNodeVoltages(time);
			VectorTools::subtract(&voltages, &refVoltages, &dx);//dx = xnew-x	
			curve[time] = VectorTools::norm(dx);
		}
		visualizer->addCurve(curve, xFactor, names[i]);
	}
	visualizer->visualize(false, true, true);
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
		visualizer->buildCurve(managers[i]->getGridModelReference()->getNode(vid), func, managers[i]->getGridModelReference()->getTimeslots(), names[i], true);
	}
	visualizer->visualize(false, true, false);
}

/** Generic compare method for comparing voltage, current, power curve of a Line from each LineManager instance. **/
void LineManagerBenchmark::compare(std::pair<std::string, std::string> line, double(Line::* func)(int))
{
	for (int i = 0; i < managers.size(); i++) {
		visualizer->buildCurve(managers[i]->getGridModelReference()->getLine(line.first, line.second), func, managers[i]->getGridModelReference()->getTimeslots(), names[i], true);
	}
	visualizer->visualize(false, true, false);
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

void LineManagerBenchmark::requestEp(EP* ep, int managerIndex)
{
	if (!lock)
	{
		computationTimes = std::vector<std::vector<double>>(managers.size(), std::vector<double>());
		decisions = std::vector<std::vector<bool>>(managers.size(), std::vector<bool>());
		lock = true;
	}
	bool decision;
	auto t1 = std::chrono::high_resolution_clock::now();
	decision = managers[managerIndex]->requestEP(ep);
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = t2 - t1;
	computationTimes[managerIndex].push_back(duration.count());
	decisions[managerIndex].push_back(decision);
}
