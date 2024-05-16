#include "LSSolver.h"
/** Constructor. Only creates the empty object. Before use setMatrix() has to be called.**/
LSSolver::LSSolver()
{
	l = NULL;
	p = NULL;
	r = NULL;
}

/** Constructor.
* @param a the matrix of the equation
**/
LSSolver::LSSolver(Matrix* a)
{
	calculateLUDecomposition(a);
}
/** Destructor.**/
LSSolver::~LSSolver()
{
	delete l;
	delete p;
	delete r;
}
/** Checks if the LSSolver is already initilized with a matrix.
@return True when LSSolver is initialized. **/
bool LSSolver::isInitialized()
{
	return l != NULL && p != NULL && r != NULL;
}

/** Sets the matrix of the equation system. Will automatically lead to calcualtion of new LUDecomposition.
@param a the matrix.**/
void LSSolver::setMatrix(Matrix* a)
{
	delete l;
	delete p;
	delete r;
	calculateLUDecomposition(a);
}
/** Calculates and stores the LU Decomposition of the given matrix
* @param a a matrix.**/
void LSSolver::calculateLUDecomposition(Matrix* a)
{
	if (a->getRowCount() != a->getColumnCount())
	{
		std::cerr << "Matrix has to be square" << std::endl;
		return;
	}

	l = new Matrix(a->getRowCount(), a->getRowCount());
	r = new Matrix(a);

	p = new Matrix(a->getRowCount());

	for (int i = 0; i < r->getRowCount() - 1; i++)
	{
		//pivot index finden
		double absMax = 0;
		double absMaxIndex = 0;
		for (int j = i; j < r->getRowCount(); j++)
		{
			if (abs(r->getValue(j, i)) > absMax)
			{
				absMax = abs(r->getValue(j, i));
				absMaxIndex = j;
			}
		}
		r->swapRows(i, absMaxIndex);
		l->swapRows(i, absMaxIndex);
		p->swapRows(i, absMaxIndex);
		double factor = 0;
		for (int j = i + 1; j < r->getRowCount(); j++)
		{
			factor = (double)r->getValue(j, i) / r->getValue(i, i);
			r->rowAdd(i, j, -factor);
			l->setValue(j, i, factor);
		}
		//std::cout << "Umformung" << *r << std::endl;

	}
	for (int i = 0; i < l->getRowCount(); i++)
	{
		l->setValue(i, i, 1);
	}
	//std::cout << "R:" << *r << std::endl;
	//std::cout << "L:" << *l << std::endl;
	//std::cout << "P:" << *p << std::endl;
}
/** Calculate the solution of the linear equation system based on the generated LU Decomposition.
* @param the solution vector b from Ax=b.
* @return the solution of the equation system.
**/
std::vector<double> LSSolver::getSolution(std::vector<double> b)
{
	//std::cerr << "Solution vector " << b << std::endl;
	if (b.size() != l->getRowCount())
	{
		std::cerr << "Solution vector does have the wrong size" << std::endl;
		std::vector<double> r;
		return r;
	}
	//std::cout << "B" << *b << std::endl;
	return backwardSubstitution(forwardSubstitution(b));
}
/** Returns the forward substitution
* @param b the vector used in substitution
* @return the solution vector.**/
std::vector<double> LSSolver::forwardSubstitution(std::vector<double> b)
{
	std::vector<double> b1 = Matrix::multiply(p, b);
	std::vector<double> y;
	y.reserve(b.size());
	double temp = 0;
	for (int i = 0; i < b.size(); i++)
	{
		temp = 0;
		for (int j = 0; j < i; j++)
		{
			temp += l->getValue(i, j) * y[j];
			//std::cout << "tempI:" << temp << std::endl;
		}
		//std::cout << "temp:" << temp << std::endl;
		//std::cout << "b1:" << b1->getValue(i, 0) << std::endl;
		//std::cout << "temp1:" << b1->getValue(i, 0) - temp << std::endl;
		y.push_back((b1[i] - temp) / l->getValue(i, i));
		//std::cout << "y:" << y->getValue(i, 0) << std::endl;
	}
	//std::cout << "y" << *y << std::endl;
	return y;

}
/** Returns the backward substitution
* @param b the vector used in substitution
* @return the solution vector.**/
std::vector<double> LSSolver::backwardSubstitution(std::vector<double> y)
{
	std::vector<double> x(y.size(), 0);

	double temp = 0;
	for (int i = y.size() - 1; i > -1; i--)
	{
		temp = 0;
		for (int j = y.size() - 1; j > i; j--)
		{
			temp += r->getValue(i, j) * x[j];
			//std::cout << "tempI: " << i<<"," << j << "," << temp << std::endl;
		}
		//std::cout << "temp:" << temp << std::endl;
		//std::cout << "y1:" << y->getValue(i, 0) << std::endl;
		//std::cout << "temp1:" << y->getValue(i, 0) - temp << std::endl;

		x[i] = (y[i] - temp) / r->getValue(i, i);
		//std::cout << "x:" << x->getValue(i, 0) << std::endl;
	}
	return x;
}
/** Returns the L matrix of the LU Decomposition.
* @return the L matrix.**/
Matrix* LSSolver::getL()
{
	return l;
}
/** Returns the R matrix of the LU Decomposition.
* @return the R matrix.**/
Matrix* LSSolver::getR()
{
	return r;
}
/** Returns the P matrix of the LU Decomposition.
* @return the P matrix.**/
Matrix* LSSolver::getP()
{
	return p;
}

