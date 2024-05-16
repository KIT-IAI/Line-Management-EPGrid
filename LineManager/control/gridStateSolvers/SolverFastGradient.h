#pragma once
#include "Solver.h"

/*** Solver Class based on gradients of the power curve **/
class SolverFastGradient : public Solver
{
public:
	SolverFastGradient(GridModel* model);
	SolverFastGradient(GridModel* model, SolverLogging* logger);
	~SolverFastGradient();
	std::vector<double> calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);
	std::string getType();
private:
	std::map<std::string, int> nodeIndexMap;
	Matrix* admittanceMatrixStatic;
	void generateNodeIndexMap();
	Matrix* generateAdmittanceMatrixDynamic(std::vector<double> powerVector, std::vector<double> operatingPoint);
	void generateAdmittanceMatrixStatic();
};
