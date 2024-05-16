#include "VisualizerCurve.h"
#include <string>

/** Constructor **/
VisualizerCurve::VisualizerCurve() :Visualizer()
{

}

/** Add a curve which shall be plotted in the next call of show(). x-Values are added automatically as natural numbers beginning with 0.
* @param curve vector of the points of the curve.
* @param curveName the name of the curve used in the legend of the plot. **/
void VisualizerCurve::addCurve(std::vector<double> curve, std::string curveName)
{
	for (int i = 0; i < curve.size(); i++)
	{
		xValues += std::to_string(i) + ",";
		yValues += std::to_string(curve[i]) + ",";
	}
	xValues.pop_back();
	yValues.pop_back();
	xValues += "\n";
	yValues += "\n";
	curveNames += curveName + ",";
}

/** Add a curve which shall be plotted in the next call of show().
* @param curve a map containing the x-values as keys and the y-values as values.
* @param curveName the name of the curve used in the legend of the plot. **/
void VisualizerCurve::addCurve(std::map<int, double> curve, double xFactor, std::string curveName)
{
	for (std::map<int, double>::iterator it = curve.begin(); it != curve.end(); it++)
	{
		xValues += std::to_string((it->first) * xFactor) + ",";
		yValues += std::to_string(it->second) + ",";
	}
	xValues.pop_back();
	yValues.pop_back();
	xValues += "\n";
	yValues += "\n";
	curveNames += curveName + ",";
}

/** Add a curve which shall be plotted in the next call of show().
* @param curve a map containing the x-values as keys and the y-values as values.
* @param curveName the name of the curve used in the legend of the plot. **/
void VisualizerCurve::addCurve(std::map<double, double> curve, double xFactor, std::string curveName)
{
	for (std::map<double, double>::iterator it = curve.begin(); it != curve.end(); it++)
	{
		xValues += std::to_string((it->first) * xFactor) + ",";
		yValues += std::to_string(it->second) + ",";
	}
	xValues.pop_back();
	yValues.pop_back();
	xValues += "\n";
	yValues += "\n";
	curveNames += curveName + ",";
}

/** Deletes all functions and configurations which were staged for next visualization. **/
void VisualizerCurve::clearCurves()
{
	xValues = "";
	yValues = "";
	curveNames = "";
	xAxesLabel = "";
	yAxesLabel = "";
	title = "";
}

/** Displays all curves and configurations staged for visulization.
* @param scatter selects if plot shows just points (scatterplot) or a curve.
* @param hold selects if the value is hold between two points or linear interpolated.
**/
void VisualizerCurve::visualize(bool scatter, bool hold, bool logscale)
{
	xValues.pop_back();
	yValues.pop_back();
	curveNames.pop_back();
	/*std::cout << "x: " << xValues << std::endl;
	std::cout << "y: " << yValues << std::endl;
	std::cout << "names: " << names << std::endl;*/
	std::string scatterString = scatter ? "True" : "False";
	std::string holdString = hold ? "True" : "False";
	std::string logscaleString = logscale ? "True" : "False";
	PyObject* pValue = PyObject_CallMethod(pInstance, "renderCurves", "(s,s,s,s,s,s,s,s,s)", xValues.c_str(), yValues.c_str(), scatterString.c_str(), holdString.c_str(), logscaleString.c_str(), curveNames.c_str(), title.c_str(), xAxesLabel.c_str(), yAxesLabel.c_str());
	callerString = "renderCurves\n\n" + xValues + "\n\n" + yValues + "\n\n" + scatterString + "\n\n" + holdString + "\n\n" + logscaleString + "\n\n" + curveNames + "\n\n" + title + "\n\n" + xAxesLabel + "\n\n" + yAxesLabel;
		Visualizer::visualize();
	clearCurves();
}

/** Sets the x axes label of the plot
* @param label the x label.**/
void VisualizerCurve::setXAxesLabel(std::string label)
{
	xAxesLabel = label;
}

/** Sets the y axes label of the plot
* @param label the y label.**/
void VisualizerCurve::setYAxesLabel(std::string label)
{
	yAxesLabel = label;
}

/** Sets the title of the plot
* @param title the title.**/
void VisualizerCurve::setTitle(std::string title)
{
	this->title = title;
}