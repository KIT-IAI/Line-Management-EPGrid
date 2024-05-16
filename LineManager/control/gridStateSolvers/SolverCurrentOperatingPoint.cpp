#include "SolverCurrentOperatingPoint.h"
#include "../../model/gridElements/CurrentSource.h"
#include "../../math/VectorTools.h"
#include "../../math/LSSolver.h"
#include "../../Config.h"


/** Constructor.
* @param model the GridModel of the grid.**/
SolverCurrentOperatingPoint::SolverCurrentOperatingPoint(GridModel* model) :Solver(model)
{
}
/** Constructor.
* @param model the GridModel of the grid.
* @param logger the SolverLogging object for logging.**/
SolverCurrentOperatingPoint::SolverCurrentOperatingPoint(GridModel* model, SolverLogging* logger) : Solver(model, logger)
{
}
/** Overriden method from Solver::calculateVoltages() **/
std::vector<double> SolverCurrentOperatingPoint::calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	if (!lss->isInitialized())
	{
		lss->setMatrix(model->getAdmittanceMatrix());
	}

	int loggingIndex;
	if (logger != nullptr)
	{
		loggingIndex = logger->newLog();
	}
	std::vector<double> dx(model->getIndexedNodeSize(), 1);
	std::vector<double> x = operatingPoint;
	std::vector<double> xnew;
	double equivalentAdmittance;
	int iterations = 0;

	while (VectorTools::norm(dx) > TERMINATION_NORM && iterations < MAX_ITERATIONS) //termination condition
	{
		std::vector<double> currentOperatingPoint(model->getIndexedNodeSize(), 0);
		//calculate currents with working point voltage
		for (int i = 0; i < powers.size(); i++)
		{
			currentOperatingPoint[i] = currents[i] + powers[i] / x[i];
		}

		//Solve equation	
		xnew = lss->getSolution(currentOperatingPoint);
		VectorTools::subtract(&xnew, &x, &dx);//dx = xnew-x	
		x = xnew;
		iterations++;

		if (logger != nullptr)
		{
			logger->logNormDx(VectorTools::norm(dx), loggingIndex);
		}

	}
	//std::cout << "Iterations: " << iterations << ", Norm: " << VectorTools::norm(dx) << std::endl;
	return x;
}
/** Overriden method from Solver::getType() **/
std::string SolverCurrentOperatingPoint::getType()
{
	return "COP";
}
