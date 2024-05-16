#pragma once
#include "Visualizer.h"
#include <vector>
#include <map>
#include <vector>
#include <set>
#include <../../Config.h>
#include "../model/gridElements/Node.h"

/**  Class which provides the functionality to plot curves. **/
class VisualizerCurve : public Visualizer
{
public:
	VisualizerCurve();
	void addCurve(std::vector<double> curve, std::string curveName);
	void addCurve(std::map<int, double> curve, double xFactor, std::string curveName);
	void addCurve(std::map<double, double> curve, double xFactor, std::string curveName);

	void clearCurves();
	void visualize(bool scatter, bool hold,bool logscale);
	void setXAxesLabel(std::string label);
	void setYAxesLabel(std::string label);
	void setTitle(std::string title);

	template<class T>
	void buildCurve(T* object, double(T::* func)(int), std::set<int> xValues, std::string curveName, bool timeCurve)
	{
		if (timeCurve)
		{
			xValues.insert(*(xValues.rbegin()) + 1);
			if (PLOT_TIMESLOT)
			{
				setXAxesLabel("timeslot/" + std::to_string(S_PER_TIMESLOT) + "s");
			}
			else
			{
				setXAxesLabel("time/s");
			}
		}

		std::map<int, double> curve;
		for (int x : xValues) {

			curve[x] = (object->*func)(x);
		}

		if (timeCurve && !PLOT_TIMESLOT)
		{
			addCurve(curve, S_PER_TIMESLOT, curveName);
		}
		else
		{
			addCurve(curve, 1,curveName);
		}
	}

	template<class T>
	void buildCurves(std::vector<T*> objects, double(T::* func)(int), std::set<int> xValues, std::vector<std::string> curveNames, bool timeCurve)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			buildCurve(objects[i], func, xValues, curveNames[i], timeCurve);
		}
	}

private:
	std::string xValues;
	std::string yValues;
	std::string curveNames;
	std::string xAxesLabel;
	std::string yAxesLabel;
	std::string title;

};

