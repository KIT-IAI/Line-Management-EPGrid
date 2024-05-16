#include "Solver.h"
#include "../../Config.h"
/** Constructor.
* @param model the DataModel of the grid.**/
Solver::Solver(DataModel* model)
{
	this->model = model;
	logger = nullptr;
	lss = new LSSolver();
}
/** Constructor.
* @param model the DataModel of the grid.
* @param logger the SolverLogging object for logging.**/
Solver::Solver(DataModel* model, SolverLogging* logger)
{
	this->model = model;
	this->logger = logger;
	lss = new LSSolver();
}
/** Destructor. **/
Solver::~Solver()
{
	delete lss;
}