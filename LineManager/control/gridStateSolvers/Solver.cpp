#include "Solver.h"
#include "../../Config.h"
/** Constructor.
* @param model the GridModel of the grid.**/
Solver::Solver(GridModel* model)
{
	this->model = model;
	logger = nullptr;
	lss = new LSSolver();
}
/** Constructor.
* @param model the GridModel of the grid.
* @param logger the SolverLogging object for logging.**/
Solver::Solver(GridModel* model, SolverLogging* logger)
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