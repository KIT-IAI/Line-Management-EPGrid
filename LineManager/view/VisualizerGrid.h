#pragma once
#include "Visualizer.h"
#include <set>


/**  Class which provides the functionality to plot curves. **/
class VisualizerGrid : public Visualizer
{
public:
	VisualizerGrid();
	void visualizeStatic(std::string nodesStatic, std::string edgesStatic, int nodePropertyIndex, int linePropertyIndex);
	void visualizeDynamic(std::string nodesStatic, std::string nodesDynamic, std::string edgesStatic, std::string edgesDynamic,int nodePropertyIndex, int linePropertyIndex, std::string times);

private:

};
