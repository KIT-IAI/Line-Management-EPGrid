#pragma once
#include "Solver.h"
#include "SpiceWrapper.h"

/** Solver Class based on ngSpice. **/
class SolverSpice : public Solver
{
	//TODO this solver creates a LSSolver instance because of extending Solver which is not needed
public:
	SolverSpice(GridModel* model);
	SolverSpice(GridModel* model, SolverLogging* logger);
	~SolverSpice();
	std::vector<double> calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);
	std::string getType();
private:
	SpiceWrapper* spice;
	void initSpice(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);

};
