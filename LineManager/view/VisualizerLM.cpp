#pragma once
#include "VisualizerLM.h"

#include <string>
#include <iostream>
#include "../model/gridElements/CurrentSource.h"
#include "../config.h"



/** Constructor
* @param lm instance of LineManager whoose data is visualized.**/
VisualizerLM::VisualizerLM(LineManager* lm)
{
	setLineManager(lm);
	vc = new VisualizerCurve();
	vg = new VisualizerGrid();
}

VisualizerLM::~VisualizerLM()
{
	delete vc;
	delete vg;
}

/** Set LineManager
* @param lm instance of LineManager whoose data is visualized.
**/
void VisualizerLM::setLineManager(LineManager* lm)
{
	model = lm->getGridModelReference();
}

/** Displays the static grid state.
* @param nodePropertyIndex the index of the property in the static node string displayed as node label.
* @param linePropertyIndex the index of the property in the static line string displayed as edge label.**/
void VisualizerLM::showStatic(int nodePropertyIndex, int linePropertyIndex)
{
	if (model->getNodes().empty())
	{
		std::cerr << "There are no nodes to visualize" << std::endl;
		return;
	}
	vg->visualizeStatic(model->serializeNodes(), model->serializeLines(), nodePropertyIndex, linePropertyIndex);
}

/** Displays dynamic grid states of given timeslots.
* @param nodePropertyIndex the index of the property in the dynamic node string displayed as node label.
* @param linePropertyIndex the index of the property in the dynamic line string displayed as edge label.
* @param times the timeslots which shall be displayed.**/
void VisualizerLM::showDynamic(int nodePropertyIndex, int linePropertyIndex, std::set<int> times)
{
	showDynamic(nodePropertyIndex, linePropertyIndex, times, false);
}

/** Displays all dynamic grid states.
* @param nodePropertyIndex the index of the property in the dynamic node string displayed as node label.
* @param linePropertyIndex the index of the property in the dynamic line string displayed as edge label.
* @param ignoreEndStates Whether idle states at the begin and end are shown in the figure or not.**/
void VisualizerLM::showDynamicAll(int nodePropertyIndex, int linePropertyIndex, bool ignoreEndStates)
{
	showDynamic(nodePropertyIndex, linePropertyIndex, model->getTimeslots(), ignoreEndStates);
}

/** Renders the plot of dynamic grid states of given time.
* @param nodePropertyIndex the index of the property in the dynamic node string displayed as node label.
* @param linePropertyIndex the index of the property in the dynamic line string displayed as edge label.
* @param times the timeslots which shall be displayed.*
* @param ignoreEndStates Whether idle states at the begin and end are shown in the figure or not.**/
void VisualizerLM::showDynamic(int nodePropertyIndex, int linePropertyIndex, std::set<int> times, bool ignoreEndStates)
{
	if (times.empty())
	{
		std::cerr << "Can not visualize empty set of times" << std::endl;
		return;
	}
	if (ignoreEndStates)
	{
		if (times.size() < 2)
		{
			std::cerr << "Ignoring begin and end of given times results in an empty set of times." << std::endl;
			return;
		}
		times = this->ignoreEndStates(times);
	}

	std::string timesString;
	for (int time : times)
	{
		timesString += std::to_string(time) + ",";
	}
	timesString.pop_back();
	vg->visualizeDynamic(model->serializeNodes(), model->serializeNodeStates(times), model->serializeLines(),model->serializeLineStates(times), nodePropertyIndex, linePropertyIndex, timesString.c_str());
}

/** Displays the current curve of all specified Node objects.
* @param vids a list with the id of each Node to be displayed.**/
void VisualizerLM::showCurrents(std::list<std::string> vids)
{
	vc->setYAxesLabel("current/A");
	vc->setTitle("Current curves of nodes");
	showNodeCurves(vids, &Node::getCurrent);
}

/** Displays the current curve of all specified Line objects.
* @param lines a list of lines each described as a pair of node ids.**/
void VisualizerLM::showCurrents(std::list<std::pair<std::string, std::string>> lines)
{
	vc->setYAxesLabel("current/A");
	vc->setTitle("Current curves of lines");
	showLineCurves(lines, &Line::getCurrent);
}

void VisualizerLM::showPowersNodes()
{
	std::list<std::string> vids;
	for (Node* v : model->getNodes())
	{
		vids.push_back(v->getID());
	}
	showPowers(vids);
}

void VisualizerLM::showPowersLines()
{
	std::list<std::pair<std::string, std::string>> lines;
	for (Line* l : model->getLines())
	{
		lines.push_back({ l->getNodeIDs().first,l->getNodeIDs().second });
	}
	showPowers(lines);
}

/** Displays the power curve of all specified Node objects.
* @param vids a list with the id of each Node to be displayed.**/
void VisualizerLM::showPowers(std::list<std::string> vids)
{
	vc->setYAxesLabel("power/W");
	vc->setTitle("Power curves of nodes");
	showNodeCurves(vids, &Node::getPower);
}

/** Displays the power curve of all specified Line objects.
* @param lines a list of lines each described as a pair of node ids.**/
void VisualizerLM::showPowers(std::list<std::pair<std::string, std::string>> lines)
{
	vc->setYAxesLabel("power/W");
	vc->setTitle("Power curves of lines");
	showLineCurves(lines, &Line::getPower);
}

/** Displays the voltage curve of all specified Node objects.
* @param vids a list with the id of each Node to be displayed.**/
void VisualizerLM::showVoltages(std::list<std::string> vids)
{
	vc->setYAxesLabel("voltage/V");
	vc->setTitle("Voltage curves of nodes");
	showNodeCurves(vids, &Node::getVoltage);
}

/** Displays the voltage curve of all specified Line objects.
* @param lines a list of lines each described as a pair of node ids.**/
void VisualizerLM::showVoltages(std::list<std::pair<std::string, std::string>> lines)
{
	vc->setYAxesLabel("voltage/V");
	vc->setTitle("Voltage curves of lines");
	showLineCurves(lines, &Line::getVoltage);
}

/** Displays the overall powerloss curve of the grid. **/
void VisualizerLM::showPowerLoss()
{
	double xFactor;
	if (PLOT_TIMESLOT)
	{
		vc->setXAxesLabel("timeslot/" + std::to_string(S_PER_TIMESLOT) + "s");
		xFactor = 1;
	}
	else
	{
		vc->setXAxesLabel("time/s");
		xFactor = S_PER_TIMESLOT;
	}

	std::map<int, double> lossCurve;
	for (int timeslot : model->getTimeslots())
	{
		lossCurve[timeslot] = model->getPowerLoss(timeslot);
	}
	lossCurve[*model->getTimeslots().rbegin() + 1] = model->getPowerLoss(*model->getTimeslots().rbegin() + 1);

	vc->setXAxesLabel("timeslot/" + std::to_string(S_PER_TIMESLOT) + "s");
	vc->setYAxesLabel("power/W");
	vc->setTitle("Power loss of all lines");
	vc->addCurve(lossCurve, xFactor,"");
	vc->visualize(false, true,false);
}


/** Displays all dynamic grid states.
* @param times Removes the timeslot at begin and/or end if the corresponding grid state is idle.**/
std::set<int>  VisualizerLM::ignoreEndStates(std::set<int> times)
{
	bool idleBegin = true;
	bool idleEnd = true;
	CurrentSource* cc;
	for (Node* v : model->getNodes())
	{
		if (v->getType() == Node::Types::CURRENT_CONTROLLER)
		{
			cc = (CurrentSource*)v;
			if (cc->getPowerSetpoint(*times.begin()) != 0 || cc->getCurrentSetpoint(*times.begin()) != 0)
			{
				idleBegin = false;
				break;
			}
			if (cc->getPowerSetpoint(*times.rbegin()) != 0 || cc->getCurrentSetpoint(*times.rbegin()) != 0)
			{
				idleEnd = false;
				break;
			}
			if (!idleBegin && !idleEnd)
			{
				break;
			}
		}
	}
	if (idleBegin)
	{
		times.erase(*times.begin());
	}

	if (idleEnd)
	{
		times.erase(*times.rbegin());
	}
	return times;
}

/** Generic method to display various curves of Node objects.
* @param vids a list with the id of each Node to be displayed.
* @param func pointer to a function which returns a curve of a Node.**/
void VisualizerLM::showNodeCurves(std::list<std::string> vids, double(Node::* func)(int))
{
	std::vector<Node* > nodeObjects;
	nodeObjects.reserve(vids.size());
	std::vector<std::string > nodeIDs;
	nodeIDs.reserve(vids.size());
	for (std::string vid : vids)
	{
		nodeObjects.push_back(model->getNode(vid));
		nodeIDs.push_back(vid);
	}
	vc->buildCurves(nodeObjects, func, model->getTimeslots(), nodeIDs, true);
	vc->visualize(false, true,false);
}

/** Generic method to display various curves of Line objects.
* @param lines a list of lines each described as a pair of node ids.
* @param func pointer to a function which returns a curve of a Line.**/
void VisualizerLM::showLineCurves(std::list<std::pair<std::string, std::string>> lines, double(Line::* func)(int))
{

	std::vector<Line*> lineObjects;
	lineObjects.reserve(lines.size());
	std::vector<std::string> lineIDs;
	lineIDs.reserve(lines.size());
	for (std::pair<std::string, std::string> line : lines)
	{
		lineObjects.push_back(model->getLine(line.first, line.second));
		lineIDs.push_back(lineObjects[lineObjects.size() - 1]->getID());
	}
	vc->buildCurves(lineObjects, func, model->getTimeslots(), lineIDs, true);
	vc->visualize(false, true,false);
}