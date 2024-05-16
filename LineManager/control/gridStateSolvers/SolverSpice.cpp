#include "SolverSpice.h"
#include "../../math/VectorTools.h"

/** Constructor.
* @param model the GridModel of the grid.**/
SolverSpice::SolverSpice(GridModel* model) :Solver(model)
{
	spice = new SpiceWrapper();
}
/** Constructor.
* @param model the GridModel of the grid.
* @param logger the SolverLogging object for logging.**/
SolverSpice::SolverSpice(GridModel* model, SolverLogging* logger) : Solver(model, logger)
{
	spice = new SpiceWrapper();

}
/** Destructor. **/
SolverSpice::~SolverSpice()
{
	delete spice;
}
/** Overriden method from Solver::calculateVoltages() **/
std::vector<double> SolverSpice::calculateVoltages(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	int loggingIndex;
	if (logger != nullptr)
	{
		loggingIndex = logger->newLog();
	}

	initSpice(currents, powers, operatingPoint);
	spice->calculate();
	pvecvaluesall p = spice->getSolution();
	std::vector<double> voltages = operatingPoint;
	for (int i = 0; i < p->veccount; i++)
	{
		//std::cout << p->vecsa[i]->name << ": " << p->vecsa[i]->creal << std::endl;
		std::string vid = p->vecsa[i]->name;

		//std::cout << "vid: " << vid << std::endl;
		if (vid.find_first_of('n') == 0)
			//nodeIDS.find("n") != std::string::npos)
		{
			vid = vid.substr(1);
			int first = vid.find_first_of(")");
			vid = vid.substr(0, first);

			//	std::cout << "node: " << vid << " ,voltage: " << p->vecsa[i]->creal << std::endl;
			if (model->getNodeIndex(vid) != -1)//Filter Voltage Controllers
			{
				voltages[model->getNodeIndex(vid)] = p->vecsa[i]->creal;
			}
		}
	}
	//Spice getData
	//VectorTools::print("voltages", voltages);


	if (logger != nullptr)
	{
		std::vector<double> dx(voltages.size(), 0);
		VectorTools::subtract(&voltages, &operatingPoint, &dx);//dx = xnew-x	
		logger->logNormDx(VectorTools::norm(dx), loggingIndex);
	}


	//	VectorTools::print("voltages: ", voltages);
	return voltages;
}

/** Overriden method from Solver::getType() **/
std::string SolverSpice::getType()
{
	return "Spice";
}
/** Initializes spice with a specific model and setpoints.
* @param currents the vector of current setpoints.
* @param power the vector of power setpoints.
* @param operatingPoint the voltage vector of the operating point.**/
void SolverSpice::initSpice(std::vector<double> currents, std::vector<double> powers, std::vector<double> operatingPoint)
{
	spice->newCircuit("Line Manager");
	for (Line* l : model->getLines())
	{
		spice->addLine(l->getNodeIDs().first, l->getNodeIDs().second, l->getResistance());
	}
	for (Node* v : model->getNodes())
	{
		switch (v->getType())
		{
		case Node::Types::CURRENT_CONTROLLER:
			spice->addCC(v->getID(), v->getID(), "GND", currents[model->getNodeIndex(v)], powers[model->getNodeIndex(v)]);
			break;
		case Node::Types::VOLTAGE_CONTROLLER:
			spice->addVC(v->getID(), v->getID(), "GND", currents[model->getNodeIndex(v->getLines().front()->getPartnerNode(v))] * v->getLines().front()->getResistance());
			break;
		default:
			break;
		}
	}
}
