#pragma once
#include "Solver.h"
#include "../../math/LSSolver.h"

/*** Solver Class based on nodal analysis (Knotenpotentialverfahren). This only allows current setpoints and no power setpoints. **/
class SolverNodal : public Solver
{
public:
	SolverNodal(DataModel* model);
	SolverNodal(DataModel* model, SolverLogging* logger);
	std::vector<double> calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);
	std::string getType();
private:
};
