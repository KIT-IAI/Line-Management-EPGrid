#pragma once
#include "../control/LineManager.h"
#include "VisualizerCurve.h"
#include "VisualizerGrid.h"
/**  Class to visualize several properties of a LineManager, including individual Node curves and Line curves.**/
class VisualizerLM
{
public:
	VisualizerLM(LineManager* lm);
	~VisualizerLM();
	void setLineManager(LineManager* lm);
	void showStatic(int nodePropertyIndex, int linePropertyIndex);
	void showDynamic(int nodePropertyIndex, int linePropertyIndex, std::set<int> times);
	void showDynamicAll(int nodePropertyIndex, int linePropertyIndex, bool ignoreEndStates);
	void showCurrents(std::list<std::string> vids);
	void showCurrents(std::list<std::pair<std::string, std::string>> lines);
	void showPowersNodes();
	void showPowersLines();
	void showPowers(std::list<std::string> vids);
	void showPowers(std::list<std::pair<std::string, std::string>> lines);
	void showVoltages(std::list<std::string> vids);
	void showVoltages(std::list<std::pair<std::string, std::string>> lines);
	void showPowerLoss();
private:
	DataModel* model;
	VisualizerCurve* vc;
	VisualizerGrid* vg;
	void showDynamic(int nodePropertyIndex, int linePropertyIndex, std::set<int> times, bool ignoreEndStates);
	std::set<int> ignoreEndStates(std::set<int> times);
	void showNodeCurves(std::list<std::string> vids, double(Node::* func)(int));
	void showLineCurves(std::list<std::pair<std::string, std::string>> lines, double(Line::* func)(int));
};