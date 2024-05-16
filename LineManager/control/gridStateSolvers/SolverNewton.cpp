#include "SolverNewton.h"
#include "../../math/VectorTools.h"
#include "../../model/gridElements/CurrentSource.h"
#include "../../math/LSSolver.h"
#include "../../Config.h"

/** Constructor.
* @param model the GridModel of the grid.**/
SolverNewton::SolverNewton(GridModel* model) :Solver(model)
{
}
/** Constructor.
* @param model the GridModel of the grid.
* @param logger the SolverLogging object for logging.**/
SolverNewton::SolverNewton(GridModel* model, SolverLogging* logger) : Solver(model, logger)
{
}
/** Overriden method from Solver::calculateVoltages() **/
std::vector<double> SolverNewton::calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	for (int i = 0; i < currents.size(); i++)
	{
		if (currents[i] != 0 && powers[i] != 0)
		{
			std::cerr << "Current setpoint and power setpoint can not be used at the same time and node with Newton solver. Setpoint for each individual node at one timeslot can either be power or current." << std::endl;
			exit(1);
		}
	}

	int loggingIndex;
	if (logger != nullptr)
	{
		loggingIndex = logger->newLog();
	}

	std::vector<double> resVector = VectorTools::add(powers, currents);//Combine both vectors into one. This combination of current and power is valid because the entries of fx matrix and jacobi matrix are defined according to power or current.
	std::vector<double> dx(model->getIndexedNodeSize(), 1);
	std::vector<double> x = operatingPoint;
	std::vector<double> fx;
	Matrix* fxMatrix;
	Matrix* dfxMatrix;
	int iterations = 0;
	while (VectorTools::norm(dx) > TERMINATION_NORM && iterations < MAX_ITERATIONS) //termination condition
	{
		fxMatrix = calculateFunction(powers, x);
		fx = Matrix::multiply(fxMatrix, x);//Calculate f(voltage)
		VectorTools::subtract(&fx, &resVector, &fx);//Subtract resVector to achieve f(x)=0

		dfxMatrix = calculateJacobi(powers, x);

		//Solve linear equation f'(x0)*dx1=-f(x0)
		lss->setMatrix(dfxMatrix);
		fx = VectorTools::negate(fx);//set f(x) to -f(x)
		dx = lss->getSolution(fx);

		//Calculate xnew = dx1 + x
		VectorTools::add(x, dx, &x);
		iterations++;

		delete fxMatrix;
		delete dfxMatrix;
		if (logger != nullptr)
		{
			logger->logNormDx(VectorTools::norm(dx), loggingIndex);
		}

	}
	//std::cout << "Iterations: " << iterations << ", Norm: " << VectorTools::norm(dx) << std::endl;
	return x;
}

/** Overriden method from Solver::getType() **/
std::string SolverNewton::getType()
{
	return "Newton";
}
/** Calculates the function f(x) of the Newton equation. In this case this is f(voltage)=power.
* @param powerVector the vector of power setpoints.
* @param operatingPoint the voltage vector of the operating point.
* @return the function f(x) in a matrix representation.**/
Matrix* SolverNewton::calculateFunction(std::vector<double> powerVector, std::vector<double> operatingPoint)
{
	/*
* Calculate f(x)
* At the moment we have nodal analysis structure: f(voltages)=admittances*voltages=currents
* Newton method requires f(x)=0
* Because of power setpoints currents have to expressed as admittance*voltage=P/V
* This leads to a multiplication of the correspodning row in the admittanceMatrix with V
* Afterwards the currents vector has to be subtracted from the left side of the equation to achieve f(x)=0
*
* Assuming an admittance matrix with admittances aij the function f(x) looks similar to
* a11 *V1 + a12*V2 +a13*V3 - I1 = 0
* a21 * V1* V2 + a22 * V2^2 +a23 * V3 * V2 - P2 = 0
* a31 * V1 * V3 + a32 * V2 * V3 + a33 * V3^2 - P3 = 0
* Note the usage of I or P depending on current or power setpoints
*/
	Matrix* fxMatrix = new Matrix(model->getAdmittanceMatrix());//Start with admittanceMatrix

	for (int i = 0; i < powerVector.size(); i++)
	{
		if (powerVector[i] != 0)
		{
			fxMatrix->rowScale(i, operatingPoint[i]);//If node i has power setpoint multiply working point voltage Vi to row i
		}
	}
	return fxMatrix;
}
/** Calculates the Jacobian matrix f'(x) of the Newton equation.
* @param powerVector the vector of power setpoints.
* @param operatingPoint the voltage vector of the operating point.
* @return the Jacobian matrix**/
Matrix* SolverNewton::calculateJacobi(std::vector<double> powerVector, std::vector<double> operatingPoint)
{
	/*
	* Calculate f'(x)
	* This requires the Jacobian matrix of f(x). Because f(x) is stored as value and not in functional form,
	* the Jacobian matrix has to be calcualted based on the inital function f(voltages)=admittances*voltages=currents
	*
	* Assuming an admittance matrix with admittances aij the function f'(x) looks similar to
	*    a11    ,     a12       ,     a13
	* a21 * V2  ,  2* a22 * V2  ,  a23 * V2
	* a31 * V3  ,     a32 * V3  ,  2* a33 * V3-a33*Vx
	*
	* Each row depends if power or current is used as setpoint and thus the row in f(x) was multiplied by Vi or not
	*/
	//TODO check this matrix and explanation
	Matrix* dfxMatrix = new Matrix(model->getAdmittanceMatrix());//Start with admittanceMatrix
	for (Node* v : model->getNodes())
	{
		if (v->getType() != Node::Types::CURRENT_CONTROLLER)
		{
			continue;
		}
		int i = model->getNodeIndex(v);
		if (powerVector[i] == 0)//if node i had no power setpoint
		{
			continue;
		}
		dfxMatrix->rowScale(i, operatingPoint[i]);//multiply row by Vi
		dfxMatrix->setValue(i, i, dfxMatrix->getValue(i, i) * 2);//multiply entry i,i with 2 because f(x) had v^2 there
		Node* partner = v->getLines().front()->getPartnerNode(v);
		double voltage = operatingPoint[model->getNodeIndex(partner)];
		double admittance = 1 / v->getLines().front()->getResistance();
		dfxMatrix->addValue(i, i, -admittance * voltage);
	}

	return dfxMatrix;
}
