#include "SolverFLPF.h"
#include "../../model/gridElements/CurrentSource.h"
#include "../../math/VectorTools.h"
#include "../../math/Matrix.h"
#include "../../math/LSSolver.h"
#include "../../Config.h"


/** Constructor.
* @param model the GridModel of the grid.**/
SolverFLPF::SolverFLPF(GridModel* model) :Solver(model)
{
}
/** Constructor.
* @param model the GridModel of the grid.
* @param logger the SolverLogging object for logging.**/
SolverFLPF::SolverFLPF(GridModel* model, SolverLogging* logger) : Solver(model, logger)
{
}
/** Overriden method from Solver::calculateVoltages() **/
std::vector<double> SolverFLPF::calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	for (Node* v : model->getNodes())
	{
		if (v->getType() == Node::Types::CURRENT_CONTROLLER)
		{
			if (currents[model->getNodeIndex(v)] != 0)
			{
				std::cout << *v << std::endl;
				std::cerr << "Current setpoints are not allowed for FLPF solver." << std::endl;
				exit(1);
			}
		}
	}



	int loggingIndex;
	if (logger != nullptr)
	{
		loggingIndex = logger->newLog();
	}

	for (int i = 0; i < powers.size(); i++)
	{
		powers[i] = powers[i] / (VOLTAGE * VOLTAGE);
	}

	if (!lss->isInitialized())
	{
		lss->setMatrix(model->getAdmittanceMatrix());
	}
	std::vector<double> deltaVoltages = lss->getSolution(powers);
	//VectorTools::print("deltaVoltages: ", deltaVoltages);

	//std::vector<double> errors = VectorTools::subtract(powers, Matrix::multiply(model->getAdmittanceMatrix(), deltaVoltages));
	Matrix* diag = new Matrix(deltaVoltages.size(), deltaVoltages.size());
	for (int i = 0; i < deltaVoltages.size(); i++)
	{
		diag->setValue(i, i, deltaVoltages[i]);
	}
	std::vector<double> errors = VectorTools::subtract(powers, Matrix::multiply(diag, Matrix::multiply(model->getAdmittanceMatrix(), deltaVoltages)));
	delete diag;

	//VectorTools::print("errors: ", errors);
	std::vector<double> correctedDeltaVoltages = lss->getSolution(errors);

	//VectorTools::print("correctedDeltaVoltages: ", correctedDeltaVoltages);
	std::vector<double> voltages = VectorTools::add(correctedDeltaVoltages, std::vector<double>(correctedDeltaVoltages.size(), 1));
	for (int i = 0; i < voltages.size(); i++)
	{
		voltages[i] = voltages[i] * VOLTAGE;//TODO voltage could be a param, derived by nodes
	}

	if (logger != nullptr)
	{
		std::vector<double> dx(voltages.size(), 0);
		VectorTools::subtract(&voltages, &operatingPoint, &dx);//dx = xnew-x	
		logger->logNormDx(VectorTools::norm(dx), loggingIndex);
	}
	//	VectorTools::print("voltages: ", voltages);
	return voltages;
}

/** Overriden method from Solver::getType() **/
std::string SolverFLPF::getType()
{
	return "FLPF";
}
