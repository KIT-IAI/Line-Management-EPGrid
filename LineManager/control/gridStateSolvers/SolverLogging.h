#pragma once
#include <vector>
#include <list>
#include <mutex>

class SolverLogging
{
public:
	int newLog();
	void logNormDx(double norm,int index);
	std::vector<double> getNorms(int index);

protected:


private:
	//parameter vom solver
	//iterations

	//norm pro iteration
	//voltages? pro iteration
	std::mutex mutex;
	std::vector<int> timeslots;
	std::vector<int> suppliers;
	std::vector<int> absorbers;
	std::vector<double> setpoints;
	std::vector<std::vector<double>> norms;
};