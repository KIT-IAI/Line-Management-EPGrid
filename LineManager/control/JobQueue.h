#pragma once

#include <thread>  
#include <mutex>
#include "GridStateSolvers/Solver.h"


class JobQueue
{
public:
	JobQueue(DataModel* model, int threadLimit, std::string solver);
	~JobQueue();
	void addJob(int timeslot, std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint);
	std::pair<int, std::vector<double>> getNextResult();
	bool isEmpty();
	void run();
	void stop();
	void clear();
private:
	int threadLimit;
	std::atomic<int> activeJobs = 0;
	bool running = false;
	bool idle = true;
	std::list<int> jobsTimeslot;

	std::list<std::vector<double>> jobsCurrents;
	std::list<std::vector<double>> jobsPowers;
	std::list<std::vector<double>> jobsOperatingPoint;


	std::list<std::pair<int, std::vector<double>>> results;
	std::list<std::thread> threadpool;
	std::vector<Solver*> solverPool;
	std::mutex resultsMutex;
	std::mutex setpointsMutex;
	void threadMethod(int index);
};

