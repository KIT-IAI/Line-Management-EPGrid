#pragma once
#include "Solver.h"
#include "../../math/LSSolver.h"

/*** Solver Class based on Fast Linear Power Flow (DOI: 10.1109/TPWRS.2019.2914304). **/
class SolverFLPF : public Solver
{
public:
	SolverFLPF(GridModel* model);
	SolverFLPF(GridModel* model, SolverLogging* logger);
	std::vector<double> calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);
	std::string getType();
private:
};
