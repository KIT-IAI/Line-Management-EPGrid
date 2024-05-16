#include "SolverNodal.h"
#include "../../model/gridElements/CurrentController.h"
#include "../../math/VectorTools.h"

/** Constructor.
* @param model the DataModel of the grid.**/
SolverNodal::SolverNodal(DataModel* model) :Solver(model)
{
}
/** Constructor.
* @param model the DataModel of the grid.
* @param logger the SolverLogging object for logging.**/
SolverNodal::SolverNodal(DataModel* model, SolverLogging* logger) : Solver(model, logger)
{
}
/** Overriden method from Solver::calculateVoltages() **/
std::vector<double> SolverNodal::calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	for (int i = 0; i < powers.size(); i++)
	{
		if (powers[i] != 0)
		{
			std::cerr << "Power setpoints are not allowed for Nodal solver." << std::endl;
			exit(1);
		}
	}

	if (!lss->isInitialized())
	{
		lss->setMatrix(model->getAdmittanceMatrix());
	}
	int loggingIndex;
	if (logger != nullptr)
	{
		loggingIndex = logger->newLog();
	}


	std::vector<double> x = lss->getSolution(currents);

	if (logger != nullptr)
	{
		std::vector<double> dx(x.size(), 0);
		VectorTools::subtract(&x, &operatingPoint, &dx);//dx = xnew-x	
		logger->logNormDx(VectorTools::norm(dx), loggingIndex);
	}

	return x;
}
/** Overriden method from Solver::getType() **/
std::string SolverNodal::getType()
{
	return "Nodal";
}
