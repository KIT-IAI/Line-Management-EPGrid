#include "SolverGradient.h"
#include "../../model/gridElements/CurrentController.h"
#include "../../math/VectorTools.h"
#include "../../Config.h"


/** Constructor.
* @param model the DataModel of the grid.**/
SolverGradient::SolverGradient(DataModel* model) :Solver(model)
{
}

/** Constructor.
* @param model the DataModel of the grid.
* @param logger the SolverLogging object for logging.**/
SolverGradient::SolverGradient(DataModel* model, SolverLogging* logger) : Solver(model, logger)
{
}

/** Overriden method from Solver::calculateVoltages() **/
std::vector<double> SolverGradient::calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	int loggingIndex;
	if (logger != nullptr)
	{
		loggingIndex = logger->newLog();
	}

	std::vector<double> dx(model->getIndexedNodeSize(), 1);
	std::vector<double> x = operatingPoint;
	std::vector<double> xnew;
	double equivalentAdmittance;
	Matrix* admittanceMatrixCopy;
	int iterations = 0;

	while (VectorTools::norm(dx) > TERMINATION_NORM && iterations < MAX_ITERATIONS) //termination condition
	{
		//Define current vector (The solution side b of linear system Ax=b) of actual grid situation
		admittanceMatrixCopy = new Matrix(model->getAdmittanceMatrix());

		//Calculate gradient and set equivalent admittance in admittance matrix
		for (int i = 0; i < powers.size(); i++)
		{
			equivalentAdmittance = -powers[i] / pow(x[i], 2);
			admittanceMatrixCopy->addValue(i, i, equivalentAdmittance);

		}

		//Solve equation
		lss->setMatrix(admittanceMatrixCopy);
		xnew = lss->getSolution(currents);
		VectorTools::subtract(&xnew, &x, &dx);//dx = xnew-x		
		x = xnew;
		iterations++;
		delete admittanceMatrixCopy;
		if (logger != nullptr)
		{
			logger->logNormDx(VectorTools::norm(dx), loggingIndex);
		}
	}
	//std::cout << "Iterations: " << iterations << ", Norm: " << VectorTools::norm(dx) << std::endl;
	return x;
}
/** Overriden method from Solver::getType() **/
std::string SolverGradient::getType()
{
	return "Gradient";
}
