#pragma once
#include "Solver.h"

/*** Solver Class based on gradients of the power curve **/
class SolverCurrentOperatingPoint : public Solver
{
public:
	SolverCurrentOperatingPoint(GridModel* model);
	SolverCurrentOperatingPoint(GridModel* model, SolverLogging* logger);
	std::vector<double> calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);
	std::string getType();
private:
};
