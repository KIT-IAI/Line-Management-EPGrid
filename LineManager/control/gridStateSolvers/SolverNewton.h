#pragma once
#include "Solver.h"

/*** Solver Class based on Newton–Raphson method **/
class SolverNewton : public Solver
{
public:
	SolverNewton(GridModel* model);
	SolverNewton(GridModel* model, SolverLogging* logger);
	std::vector<double> calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);
	std::string getType();
private:
	Matrix* calculateFunction(std::vector<double> powerVector, std::vector<double> operatingPoint);
	Matrix* calculateJacobi(std::vector<double> powerVector, std::vector<double> operatingPoint);
};
