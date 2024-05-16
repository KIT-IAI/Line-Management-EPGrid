#include "JobQueue.h"
#include <iostream>
#ifndef __linux__
#include <windows.h>
#else // __linux__
#include <unistd.h>
#define Sleep(a) usleep((unsigned int)1000 * a)
#endif // __linux__

#include "gridStateSolvers/SolverGradient.h"
#include "gridStateSolvers/SolverNewton.h"
#include "gridStateSolvers/SolverNodal.h"
#include "gridStateSolvers/SolverFLPF.h"
#include "gridStateSolvers/SolverFastGradient.h"
#include "gridStateSolvers/SolverCurrentOperatingPoint.h"
#include "gridStateSolvers/SolverSpice.h"
#include "../math/VectorTools.h"

/** Constructor.
* @param model the DataModel of the grid.
* @param threadLimit the number of threads to be used.
* @param solver the solver type to be used for calculations.**/
JobQueue::JobQueue(DataModel* model, int threadLimit, std::string solver)
{
	this->threadLimit = threadLimit;
	solverPool.reserve(threadLimit);
	for (int i = 0; i < threadLimit; i++)
	{
		if (solver == "Gradient")
		{
			solverPool.push_back(new SolverGradient(model));
		}
		else if (solver == "Newton")
		{
			solverPool.push_back(new SolverNewton(model));
		}
		else if (solver == "FLPF")
		{
			solverPool.push_back(new SolverFLPF(model));
		}
		else if (solver == "FGradient")
		{
			solverPool.push_back(new SolverFastGradient(model));
		}
		else if (solver == "COP")
		{
			solverPool.push_back(new SolverCurrentOperatingPoint(model));
		}
		else if (solver == "Spice")
		{
			solverPool.push_back(new SolverSpice(model));
		}
		else if (solver == "Nodal")
		{
			solverPool.push_back(new SolverNodal(model));
		}
		else
		{
			std::cerr << "Unknown solver named: "<< solver << std::endl;
			exit(1);
		}
	}
}
/** Destructor. **/
JobQueue::~JobQueue()
{
	stop();
	for (int i = 0; i < threadLimit; i++)
	{
		delete solverPool[i];
	}

}
/** Adds a job to the queue
* @param timeslot the timeslot of the calculated grid state.
* @param currents a current vector of the set points of the corresponding timeslot.
* @param powers a power vector of set points of the corresponding timeslot.
* @param operatingPoint a voltage vector as first operating point.**/
void JobQueue::addJob(int timeslot, std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	//std::cout << "Queue Add timeslot: " << timeslot <<  std::endl;
	{
		std::lock_guard<std::mutex> lockGuard(setpointsMutex);
		jobsTimeslot.push_back(timeslot);
		jobsCurrents.push_back(currents);
		jobsPowers.push_back(powers);
		jobsOperatingPoint.push_back(operatingPoint);
	}
	idle = false;
}
/** Returns the next result in the queue.
* @return A pair containing the timeslot in the first place and the voltage vector of the nodes in the second place.**/
std::pair<int, std::vector<double>> JobQueue::getNextResult()
{
	while (results.size() == 0)
	{
		Sleep(20);
	}
	std::pair<int, std::vector<double>> res;
	{
		std::lock_guard<std::mutex> lockGuard(resultsMutex);
		res = results.front();
		results.pop_front();
	}
	//std::cout << "return: " << res.first << std::endl;
		//std::cout << "return job: " << " time: " << res.first;
	//VectorTools::print(" voltage", res.second);
	return res;

}
/** Checks if the queue has no further jobs pending.
* @return True when the queue is empty and false if not.**/
bool JobQueue::isEmpty()
{
	return jobsTimeslot.empty() && results.empty() && activeJobs == 0;
}

/** The internal methods which is run by the individual threads processing the jobs.**/
void JobQueue::threadMethod(int index)
{

	int timeslot;
	std::vector<double> currents;
	std::vector<double> powers;
	std::vector<double> operatingPoint;

	while (running)
	{
		if (idle)
		{
			Sleep(4);
		}

		std::pair<int, double> setpoint;
		{
			std::lock_guard<std::mutex> lockGuard(setpointsMutex);

			if (jobsTimeslot.empty())
			{
				idle = true;
				continue;
			}

			timeslot = jobsTimeslot.front();
			jobsTimeslot.pop_front();

			currents = jobsCurrents.front();
			jobsCurrents.pop_front();

			powers = jobsPowers.front();
			jobsPowers.pop_front();

			operatingPoint = jobsOperatingPoint.front();
			jobsOperatingPoint.pop_front();
		}
		activeJobs++;
		std::vector<double> voltages = solverPool[index]->calculateVoltages(currents, powers, operatingPoint);
		//	std::cout << "Time: " << setpoint.first<< "  ";
			//VectorTools::print("Voltage ", voltages);
		std::pair<int, std::vector<double>> res = { timeslot, voltages };
		{
			std::lock_guard<std::mutex> lockGuard(resultsMutex);
			results.push_back(res);
		}
		activeJobs--;
	}
}

/** Starts the threads of a JobQueue. **/
void JobQueue::run()
{
	running = true;

	for (int i = 0; i < threadLimit; i++)
	{
		threadpool.push_back(std::thread(&JobQueue::threadMethod, this, i));
	}
}

/** Stops the threads of a JobQueue. **/
void JobQueue::stop()
{
	running = false;
	for (std::list<std::thread>::iterator it = threadpool.begin(); it != threadpool.end(); it++)
	{
		it->join();
	}
}
/** Clears the JobQueue and saved results. **/
void JobQueue::clear()
{
	{
		std::lock_guard<std::mutex> lockGuard(setpointsMutex);
		jobsTimeslot.clear();
		jobsCurrents.clear();
		jobsPowers.clear();
		jobsOperatingPoint.clear();
	}
	while (activeJobs != 0)
	{
		Sleep(4);
	}
	{
		std::lock_guard<std::mutex> lockGuard(resultsMutex);
		results.clear();
	}

}

