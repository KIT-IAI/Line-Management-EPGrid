#include "SolverLogging.h"
#include <iostream>

int SolverLogging::newLog()
{
	{
		std::lock_guard<std::mutex> lockGuard(mutex);
		norms.push_back(std::vector<double>());
		return norms.size() - 1;
	}
}

void SolverLogging::logNormDx(double norm, int index)
{
	//std::cout << "size: " << norms.size() << " index: " << index << std::endl;
	norms[index].push_back(norm);
}

std::vector<double> SolverLogging::getNorms(int index)
{
	return norms[index];
}
