#include "SolverFastGradient.h"
#include "../../model/gridElements/CurrentController.h"
#include "../../math/VectorTools.h"
#include "../../math/LSSolver.h"
#include "../../Config.h"


/** Constructor.
* @param model the DataModel of the grid.**/
SolverFastGradient::SolverFastGradient(DataModel* model) :Solver(model)
{
}
/** Constructor.
* @param model the DataModel of the grid.
* @param logger the SolverLogging object for logging.**/
SolverFastGradient::SolverFastGradient(DataModel* model, SolverLogging* logger) : Solver(model, logger)
{
}
/** Destructor. **/
SolverFastGradient::~SolverFastGradient()
{
	delete admittanceMatrixStatic;
}
/** Overriden method from Solver::calculateVoltages() **/
std::vector<double> SolverFastGradient::calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	int loggingIndex;
	if (logger != nullptr)
	{
		loggingIndex = logger->newLog();
	}

	if (nodeIndexMap.empty())
	{
		generateNodeIndexMap();
	}
	if (admittanceMatrixStatic == NULL)
	{
		generateAdmittanceMatrixStatic();
	}

	std::vector<double> currentVector(nodeIndexMap.size(), 0);
	Line* l;
	for (Node* v : model->getNodes())//currents need to be indexed by local nodeIndexMap => rewrite given currents into local currentVector
	{
		switch (v->getType())
		{
		case Node::Types::COUPLER:
			currentVector[nodeIndexMap[v->getID()]] = currents[model->getNodeIndex(v)];
			break;
		case Node::Types::CURRENT_CONTROLLER:
			l = v->getLines().front();
			currentVector[nodeIndexMap[l->getPartnerNode(v)->getID()]] = currents[model->getNodeIndex(v)];
			break;
		case Node::Types::VOLTAGE_CONTROLLER:
			break;
		default:
			break;
		}
	}
	std::vector<double> dx(model->getIndexedNodeSize(), 1);
	std::vector<double> x = operatingPoint;
	std::vector<double> xnew = operatingPoint;
	std::vector<double> couplerVoltages;

	Matrix* admittanceMatrix;

	int iterations = 0;

	while (VectorTools::norm(dx) > TERMINATION_NORM && iterations < MAX_ITERATIONS) //termination condition
	{
		admittanceMatrix = generateAdmittanceMatrixDynamic(powers, x);

		//Solve equation
		lss->setMatrix(admittanceMatrix);
		couplerVoltages = lss->getSolution(currentVector);

		//Calculate controller voltages based on coupler voltages
		for (Node* v : model->getNodes())
		{
			if (v->getType() == Node::Types::COUPLER)
			{
				xnew[model->getNodeIndex(v)] = couplerVoltages[nodeIndexMap[v->getID()]];
			}
			if (v->getType() == Node::Types::CURRENT_CONTROLLER)
			{
				l = v->getLines().front();
				double workingPointVoltage = x[model->getNodeIndex(v)];
				double branchVoltage = couplerVoltages[nodeIndexMap[l->getPartnerNode(v)->getID()]];
				double branchResistance = l->getResistance() - pow(x[model->getNodeIndex(v)], 2) / powers[model->getNodeIndex(v)];
				double branchCurrent = branchVoltage / branchResistance;

				double resistorVoltage = branchCurrent * l->getResistance();
				double newWorkingPointVoltage = branchVoltage - resistorVoltage;
				xnew[model->getNodeIndex(v)] = newWorkingPointVoltage;
			}

		}

		VectorTools::subtract(&xnew, &x, &dx);//dx = xnew-x		
		x = xnew;
		iterations++;
		delete admittanceMatrix;
		if (logger != nullptr)
		{
			logger->logNormDx(VectorTools::norm(dx), loggingIndex);
		}

	}
	//std::cout << "Iterations: " << iterations << ", Norm: " << VectorTools::norm(dx) << std::endl;
	return x;
}
/** Overriden method from Solver::getType() **/
std::string SolverFastGradient::getType()
{
	return "FastGradient";
}

/** Generates the local nodeIndexMap of the solver. This differs from DataModel nodeIndexMap because only Coupler nodes are needed.**/
void SolverFastGradient::generateNodeIndexMap()
{
	nodeIndexMap.clear();
	for (Node* v : model->getNodes())
	{
		if (v->getType() == Node::Types::COUPLER)
		{
			nodeIndexMap[v->getID()] = (int)nodeIndexMap.size();
		}
	}
}
/** Generates the dynamic admittance matrix by adding the dynamic parts to the static admittance matrix. This involves all replacement resistances derived from the power setpoints.
* @param m the static admittance matrix.
* @param powerVector the vector of power setpoints.
* @param operatingPoint the voltage vector of the operating point.
* @param nodeIndexMap the local nodeIndexMap.**/
Matrix* SolverFastGradient::generateAdmittanceMatrixDynamic(std::vector<double> powerVector, std::vector<double> operatingPoint)
{
	Line* l;
	Node* partnerNode;
	double branchResistance;
	Matrix* admittanceMatrix = new Matrix(admittanceMatrixStatic);
	for (Node* v : model->getNodes())
	{
		if (v->getType() != Node::Types::CURRENT_CONTROLLER)
		{
			continue;
		}
		l = v->getLines().front();
		partnerNode = l->getPartnerNode(v);
		branchResistance = l->getResistance() - pow(operatingPoint[model->getNodeIndex(v)], 2) / powerVector[model->getNodeIndex(v)];
		admittanceMatrix->addValue(nodeIndexMap[partnerNode->getID()], nodeIndexMap[partnerNode->getID()], 1 / branchResistance);
	}
	return admittanceMatrix;
}
/** Generates the static part of the admittance matrix. This involves all restistances which do not change in the solving process.
* @param nodeIndexMap the local nodeIndexMap.**/
void SolverFastGradient::generateAdmittanceMatrixStatic()
{
	if (nodeIndexMap.size() < 2)
	{
		return;
	}
	admittanceMatrixStatic = new Matrix((int)nodeIndexMap.size(), (int)nodeIndexMap.size());
	Node* partnerNode;
	for (Node* v : model->getNodes())
	{
		if (v->getType() != Node::Types::COUPLER)
		{
			continue;
		}
		for (Line* l : v->getLines())
		{
			partnerNode = l->getPartnerNode(v);
			switch (partnerNode->getType())
			{
			case Node::Types::COUPLER:
				admittanceMatrixStatic->addValue(nodeIndexMap[v->getID()], nodeIndexMap[v->getID()], 1 / l->getResistance());
				admittanceMatrixStatic->setValue(nodeIndexMap[v->getID()], nodeIndexMap[partnerNode->getID()], -1 / l->getResistance());
				break;
			case Node::Types::VOLTAGE_CONTROLLER:
				admittanceMatrixStatic->addValue(nodeIndexMap[v->getID()], nodeIndexMap[v->getID()], 1 / l->getResistance());
				break;
			default:
				break;
			}
		}
	}
}
