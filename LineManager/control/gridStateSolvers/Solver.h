#pragma once
#include "../../model/DataModel.h"
#include "SolverLogging.h"
#include "../../math/LSSolver.h"

/** Abstract Class for implementing solvers. **/
class Solver
{
public:
	Solver(DataModel* model);
	Solver(DataModel* model, SolverLogging* logger);
	~Solver();
	/** Calculates the grid voltages based on set points of current and power
	* @param currents a vector of current set points.
	* @param powers a vector of power set points.
	* @param operatingPoint a voltage vector as first operating point.
	* @return The calculated voltage vector.**/
	virtual std::vector<double> calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint) = 0;
	/** Returns the solver type.
* @return The type of the solver as string.**/
	virtual std::string getType() = 0;
protected:
	DataModel* model;
	SolverLogging* logger;
	LSSolver* lss = new LSSolver();

private:
};