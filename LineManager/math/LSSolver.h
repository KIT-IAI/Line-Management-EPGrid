#pragma once
#include "Matrix.h"
/** Solves linear equations. **/
class LSSolver
{
public:
	LSSolver();
	LSSolver(Matrix* a);
	~LSSolver();
	bool isInitialized();
	void setMatrix(Matrix* a);
	Matrix* getL();
	Matrix* getR();
	Matrix* getP();
	std::vector<double> getSolution(std::vector<double> b);
private:
	Matrix* l;
	Matrix* r;
	Matrix* p;
	void calculateLUDecomposition(Matrix* a);
	std::vector<double> forwardSubstitution(std::vector<double> b);
	std::vector<double> backwardSubstitution(std::vector<double> b);
};

