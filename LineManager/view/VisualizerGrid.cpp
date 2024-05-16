#include "VisualizerGrid.h"
#include <string>

/** Constructor **/
VisualizerGrid::VisualizerGrid() :Visualizer()
{

}
/** Visualizes the static part (topology) of a grid.
* @param nodesStatic all static node strings separated by \n.
* @param edgesStatic all static edge strings separated by \n.
* @param nodePropertyIndex the index of the node string element used as node label.
* @param linePropertyIndex the index of the node string element used as edge label.
**/
void VisualizerGrid::visualizeStatic(std::string nodesStatic, std::string edgesStatic, int nodePropertyIndex, int linePropertyIndex)
{
	PyObject* pValue = PyObject_CallMethod(pInstance, "renderStatic", "(s,s,s,s)", nodesStatic.c_str(), edgesStatic.c_str(), std::to_string(nodePropertyIndex).c_str(), std::to_string(linePropertyIndex).c_str());
	callerString = "renderStatic\n\n" + nodesStatic + "\n\n" + edgesStatic + "\n\n" + std::to_string(nodePropertyIndex) + "\n\n" + std::to_string(linePropertyIndex);
	visualize();
}

/** Visualizes the dynamic part (topology+ flows) of a grid.
* @param nodesStatic all static node strings separated by \n.
* @param nodesDynamic all dynamic node strings separated by \n.
* @param edgesStatic all static edge strings separated by \n.
* @param edgesDynamic all dynamic edge strings separated by \n.
* @param nodePropertyIndex the index of the node string element used as node label.
* @param linePropertyIndex the index of the node string element used as edge label.
* @param times all timeslots of the dynamic strings separated by comma.
**/
void VisualizerGrid::visualizeDynamic(std::string nodesStatic, std::string nodesDynamic, std::string edgesStatic, std::string edgesDynamic, int nodePropertyIndex, int linePropertyIndex, std::string times)
{
	PyObject* pValue = PyObject_CallMethod(pInstance, "renderDynamic", "(s,s,s,s,s,s,s)", nodesStatic.c_str(), nodesDynamic.c_str(), edgesStatic.c_str(), edgesDynamic.c_str(), std::to_string(nodePropertyIndex).c_str(), std::to_string(linePropertyIndex).c_str(), times.c_str());
	callerString = "renderDynamic\n\n" + nodesStatic + "\n\n" + nodesDynamic + "\n\n", edgesStatic + "\n\n" + edgesDynamic + "\n\n" + std::to_string(nodePropertyIndex) + "\n\n" + std::to_string(linePropertyIndex) + "\n\n" + times;
	visualize();
}


