#pragma once
#include "Solver.h"
#include "../../math/LSSolver.h"

/*** Solver Class based on gradients of the power curve **/
class SolverGradient : public Solver
{
public:
	SolverGradient(GridModel* model);
	SolverGradient(GridModel* model, SolverLogging* logger);
	virtual std::vector<double> calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);
	std::string getType();
private:
};
