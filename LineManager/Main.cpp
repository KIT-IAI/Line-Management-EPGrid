#include "control/LineManager.h"
#include "view/VisualizerLM.h"
#include "view/VisualizerCurve.h"
#include "control/LineManagerHelper.h"
#include "math/LSSolver.h"
#include "math/VectorTools.h"
#include "control/LineManagerBenchmark.h"
#include "model/packets/EPPowerRectangular.h"
#include "model/packets/EPPowerTrapeze.h"
#include "model/packets/EPCurrentRectangular.h"
#include "model/packets/EPCurrentTrapeze.h"
#include "Config.h"
#include "model/parser/SimBenchParser.h"
LineManager* lm;
VisualizerLM* vz;
//std::vector<std::string> solverNames = { "Newton","Gradient","FLPF","FGradient","COP","Spice" };1
//std::vector<std::string> solverNames = { "COP","FLPF","Newton"};
std::vector<std::string> solverNames = { "Newton","COP","FLPF","Spice" };
//std::vector<std::string> solverNames = { "Newton","COP","FLPF","Spice","Newton_SIM","COP_SIM","FLPF_SIM","Spice_SIM" };
//std::vector<std::string> solverNames = { "Newton_SIM","COP_SIM","FLPF_SIM","Spice_SIM" };
void example1()
{
	//Example of voltage boundaries and ability of EP to improve situation
	//don't overlook rising current at line 2  t=2 as a following of voltage improvement
	//also see reduced compensation power and current
	lm = new LineManager("Spice");
	LineManagerHelper::setupGridNetwork(lm, 8);
	lm->requestEP(new EPPowerRectangular("6", "2", 14000, 5, 4));
	lm->requestEP(new EPPowerRectangular("8", "2", 14000, 6, 3));
	lm->requestEP(new EPPowerRectangular("4", "10", 10000, 7, 2));
	lm->validateGridStates();
	vz = new VisualizerLM(lm);
	vz->showVoltages({ "0","1","2","3" });
	vz->showStatic(0, 2);
	vz->showDynamicAll(0, 1, true);
	vz->showPowers({ "0","2","4","6","8","10","12","14" });
}

void example2()
{
	lm = new LineManager("Gradient");
	LineManagerHelper::setupGridNetwork(lm, 6);
	lm->requestEP(new EPPowerRectangular("2", "6", 20000, 4, 10));

	lm->requestEP(new EPPowerRectangular("2", "6", 20000, 20, 10));
	lm->requestEP(new EPPowerRectangular("4", "8", 15000, 25, 3));

	lm->requestEP(new EPPowerRectangular("2", "6", 20000, 35, 10));
	lm->requestEP(new EPPowerRectangular("8", "4", 15000, 40, 3));

	lm->requestEP(new EPPowerRectangular("4", "8", 10000, 50, 7));
	lm->requestEP(new EPPowerRectangular("2", "8", 15000, 55, 7));

	lm->requestEP(new EPPowerRectangular("10", "0", 20000, 65, 7));

	lm->validateGridStates();

	vz = new VisualizerLM(lm);
	vz->showPowers({ "0","2","4","6","8","10" });
	vz->showVoltages({ "0","2","4","6","8","10" });
	vz->showCurrents({ "0","2","4","6","8","10" });

}

void example3()
{
	lm = new LineManager("Spice");
	LineManagerHelper::setupGridNetwork(lm, 6);
	lm->requestEP(new EPPowerTrapeze("2", "6", 20000, 10000, 4, 50));

	lm->requestEP(new EPPowerTrapeze("2", "6", 20000, 10000, 20, 50));
	lm->requestEP(new EPPowerTrapeze("4", "8", 15000, 10000, 25, 10));

	lm->requestEP(new EPPowerTrapeze("2", "6", 20000, 10000, 35, 50));
	lm->requestEP(new EPPowerTrapeze("8", "4", 15000, 10000, 40, 10));

	lm->requestEP(new EPPowerTrapeze("4", "8", 10000, 5000, 50, 15));
	lm->requestEP(new EPPowerTrapeze("2", "8", 15000, 20000, 55, 15));

	lm->requestEP(new EPPowerTrapeze("4", "8", 10000, 5000, 65, 10));
	lm->requestEP(new EPPowerTrapeze("2", "8", 15000, 20000, 70, 10));

	lm->requestEP(new EPPowerTrapeze("10", "0", 20000, 50000, 80, 10));

	lm->validateGridStates();

	vz = new VisualizerLM(lm);
	vz->showPowers({ "0","2","4","6","8","10" });
	vz->showVoltages({ "0","2","4","6","8","10" });
	vz->showCurrents({ "0","2","4","6","8","10" });

}

void example4()
{
	int participants = 20;
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames);
	lmb->setupGridNetwork(participants);
	//lmb->setupPartlyRandomizedNetwork(participants);
	lmb->generateRequestsPowerRectangular(10, 80, 5, 30000);
	lmb->validateGridStates();
	lmb->validateDecisions();

	//lmb->compareCurrents(4);
	lmb->comparePowers("4");
	lmb->compareVoltages("4");
	lmb->compareComputationTimes();
}
void example5()
{
	int participants = 20;
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames);
	lmb->setupGridNetwork(participants);
	//lmb->setupPartlyRandomizedNetwork(participants);
	lmb->generateRequestsPowerTrapeze(20, 100, 30000, 50000, 50);
	lmb->validateGridStates();
	lmb->validateDecisions();

	//lmb->compareCurrents(4);
	lmb->comparePowers("4");
	lmb->compareVoltages("4");
	lmb->compareComputationTimes();
}

void example6()
{
	int participants = 20;
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames);

	lmb->setupGridNetwork(participants);

	lmb->requestAll(new EPPowerTrapeze("2", "6", 20000, 10000, 4, 50));

	lmb->requestAll(new EPPowerTrapeze("2", "6", 20000, 10000, 20, 50));
	lmb->requestAll(new EPPowerTrapeze("4", "8", 15000, 10000, 25, 10));

	lmb->requestAll(new EPPowerTrapeze("2", "6", 20000, 10000, 35, 50));
	lmb->requestAll(new EPPowerTrapeze("8", "4", 15000, 10000, 40, 10));

	lmb->requestAll(new EPPowerTrapeze("4", "8", 10000, 5000, 50, 15));
	lmb->requestAll(new EPPowerTrapeze("2", "8", 15000, 20000, 55, 15));

	lmb->requestAll(new EPPowerTrapeze("4", "8", 10000, 5000, 65, 10));
	lmb->requestAll(new EPPowerTrapeze("2", "8", 15000, 20000, 70, 10));

	lmb->requestAll(new EPPowerTrapeze("10", "0", 20000, 50000, 80, 10));
	lmb->validateGridStates();
	lmb->validateDecisions();

	lmb->compareCurrents("4");
	lmb->comparePowers("4");
	lmb->comparePowers("2");
	lmb->compareVoltages("4");
	lmb->compareComputationTimes();
}

void example7()
{
	lm = new LineManager("COP");
	LineManagerHelper::setupGridNetwork(lm, 6);
	lm->requestEP(new EPCurrentTrapeze("2", "6", 70, 100, 4, 10));

	lm->requestEP(new EPCurrentTrapeze("2", "6", 70, 100, 20, 10));
	lm->requestEP(new EPCurrentTrapeze("4", "8", 50, 50, 23, 5));

	lm->requestEP(new EPCurrentTrapeze("2", "6", 70, 100, 35, 10));
	lm->requestEP(new EPCurrentTrapeze("8", "4", 50, 50, 38, 5));

	lm->requestEP(new EPCurrentTrapeze("4", "8", 50, 25, 50, 10));
	lm->requestEP(new EPCurrentTrapeze("2", "8", 70, 150, 58, 10));

	lm->requestEP(new EPCurrentTrapeze("4", "8", 50, 20, 70, 10));
	lm->requestEP(new EPCurrentTrapeze("2", "8", 70, 150, 78, 10));

	lm->requestEP(new EPCurrentTrapeze("10", "0", 70, 50, 90, 5));

	lm->validateGridStates();

	vz = new VisualizerLM(lm);
	vz->showPowers({ "0","2","4","6","8","10" });
	vz->showVoltages({ "0","2","4","6","8","10" });
	vz->showCurrents({ "0","2","4","6","8","10" });

}

void exampleRealGrid()
{
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames);
	lmb->setupReferenceGrid();
	//lmb->generateRequestsRectangular(80, 100, 10, 30000);
	lmb->generateRequestsPowerTrapeze(5, 100, 30000, 10000, 30);
	lmb->validateGridStates();
	lmb->validateDecisions();

	vz = new VisualizerLM(lmb->getLineManager(0));
	vz->showStatic(0, 2);
	vz->showPowers({ "0","2","6","10","12","14","16","18","20","22","24","26","28","30","32","34","38","42","44","46","50","52","54","56","58","60" });

	//lmb->compareCurrents(4);
	lmb->comparePowers("10");
	//lmb->comparePowers("2");
	lmb->compareVoltages("6");
	lmb->compareComputationTimes();
	for (int i = 0; i < solverNames.size(); i++)
	{
		if (solverNames[i] == "Spice" || solverNames[i] == "Spice_SIM")
			lmb->compareVoltageDeviation(i, true);
	}
}
void exampleRealGrid2()
{
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames);
	lmb->setupReferenceGrid();
	//lmb->generateRequestsRectangular(80, 100, 10, 30000);
	lmb->generateRequestsCurrentTrapeze(10, 100, 130, 130, 30);
	lmb->validateGridStates();
	lmb->validateDecisions();

	vz = new VisualizerLM(lmb->getLineManager(0));
	//vz->showStatic(0, 2);
	vz->showPowers({ "0","2","6","10","12","14","16","18","20","22","24","26","28","30","32","34","38","42","44","46","50","52","54","56","58","60" });

	//lmb->compareCurrents(4);
	lmb->comparePowers("10");
	//lmb->comparePowers("2");
	lmb->compareVoltages("6");
	for (int i = 0; i < solverNames.size(); i++)
	{
		if (solverNames[i] == "Spice" || solverNames[i] == "Spice_SIM")
			lmb->compareVoltageDeviation(i, true);
	}
	lmb->compareComputationTimes();
}
void exampleRealGrid3()
{
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames);
	lmb->setupReferenceGrid();
	//lmb->generateRequestsRectangular(80, 100, 10, 30000);
	//lmb->generateRequestsCurrentTrapeze(4, 500, 130, 65, 30);
	lmb->generateRequestsPowerTrapeze(5, 100, 30000, 10000, 30);
	lmb->validateGridStates();
	lmb->validateDecisions();

	vz = new VisualizerLM(lmb->getLineManager(0));
	//vz->showStatic(0, 2);
	vz->showPowers({ "0","2","6","10","12","14","16","18","20","22","24","26","28","30","32","34","38","42","44","46","50","52","54","56","58","60" });

	//lmb->compareCurrents(4);
	lmb->comparePowers("10");
	//lmb->comparePowers("2");
	lmb->compareVoltages("6");
	for (int i = 0; i < solverNames.size(); i++)
	{
		if (solverNames[i] == "Spice" || solverNames[i] == "Spice_SIM")
			lmb->compareVoltageDeviation(i, true);
	}
	lmb->compareComputationTimes();
}

void exampleSimBench(std::string name)
{
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames);
	for (int i = 0; i < solverNames.size(); i++)
	{
		SimBenchParser::parseModel(lmb->getLineManager(i), "../../LineManager/ReferenceGrids/" + name);
	}

	vz = new VisualizerLM(lmb->getLineManager(0));
	vz->showStatic(0, 2);
	//lmb->generateRequestsRectangular(10, 0, 10, 14000);
	lmb->generateRequestsPowerTrapeze(10, 50, 30000, 50000, 50);
	lmb->validateGridStates();
	lmb->validateDecisions();

	for (int i = 0; i < solverNames.size(); i++)
	{
		if (solverNames[i] == "Spice" || solverNames[i] == "Spice_SIM")
			lmb->compareVoltageDeviation(i, true);
	}
	lmb->compareComputationTimes();
}

void evalSimBench(std::vector<std::string> solvers, std::string model, std::vector<EP*> packets)
{
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solvers);
	for (int i = 0; i < solvers.size(); i++)
	{
		SimBenchParser::parseModel(lmb->getLineManager(i), "../../LineManager/ReferenceGrids/" + model);
	}
	std::string mode = solvers[0].find("_SIM") != std::string::npos ? "Simulation mode" : "Line management mode";
	for (int i = 0; i < packets.size(); i++)
	{
		std::cout << "Request: " << mode << " " << model << " packet " << i + 1 << "/" << packets.size() << std::endl;
		lmb->requestAll(packets[i]);
	}
	lmb->validateGridStates();
	lmb->validateDecisions();

	for (int i = 0; i < solvers.size(); i++)
	{

		if (solverNames[i].find("Spice") != std::string::npos)
		{
			lmb->compareVoltageDeviation(i, false);
			break;
		}
	}
	lmb->compareComputationTimes();

	vz = new VisualizerLM(lmb->getLineManager(0));
	vz->showPowersNodes();
	delete lmb;
}

void eval(int packetCount)
{
	//std::vector<std::string> models = { "1-LV-rural1--0-no_sw","1-LV-rural2--0-no_sw","1-LV-rural3--0-no_sw","1-LV-semiurb4--0-no_sw","1-LV-semiurb5--0-no_sw","1-LV-urban6--0-no_sw" };
	std::vector<std::string> models = { "1-LV-rural1--0-no_sw","1-LV-semiurb4--0-no_sw","1-LV-urban6--0-no_sw" };
	std::vector<std::string> solverNamesSim;
	for (std::string solverName : solverNames)
	{
		solverNamesSim.push_back(solverName + "_SIM");
	}

	for (std::string modelPath : models)
	{
		lm = new LineManager("Spice");
		SimBenchParser::parseModel(lm, "../../LineManager/ReferenceGrids/" + modelPath);
		std::vector<EP*> packets;
		packets.reserve(packetCount);
		for (int i = 0; i < packetCount; i++)
		{
			//std::cout << "Generating packet " << i+1 << "/" << packetCount << std::endl;
			//packets.push_back(LineManagerHelper::getRandomRectangular(lm,100, 50.0, 30000));
			//packets.push_back(LineManagerHelper::getRandomCurrentTrapeze(lm, 0, 40, 1, 120, 10, 60, 1, 50));
			packets.push_back(LineManagerHelper::getRandomPowerTrapeze(lm, 0, 60, 1000, 10000, 1000, 10000, 100, 1000));
		}
		evalSimBench(solverNames, modelPath, packets);
		evalSimBench(solverNamesSim, modelPath, packets);
	}

}

int main(void)
{
	/*std::vector<std::string> solverNames = { "Newton_SIM","COP_SIM","Spice_SIM" };
	LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames);
	for (int i = 0; i < solverNames.size(); i++)
	{
		SimBenchParser::parseModel(lmb->getLineManager(i), "../../LineManager/ReferenceGrids/1-LV-rural1--0-no_sw");
	}

	lmb->requestAll(new EPCurrentTrapeze("lv1.101_load_2", "lv1.101_load_6", 142, 20, 0, 30));
	lmb->requestAll(new EPPowerTrapeze("lv1.101_load_3", "lv1.101_load_11", 20000, 5000, 10, 30));

	lmb->requestAll(new EPCurrentTrapeze("lv1.101_load_3", "lv1.101_load_11", 100, 20, 50, 20));
	lmb->requestAll(new EPPowerTrapeze("lv1.101_load_2", "lv1.101_load_6", 30000, 5000, 40, 300));

	//lmb->requestAll(new EPpowerTrapeze(0));

	lmb->validateGridStates();
	lmb->validateDecisions();

	/*for (int i = 0; i < solverNames.size(); i++)
	{

		if (solverNames[i].find("Spice") != std::string::npos)
		{
			lmb->compareVoltageDeviation(i, false);
			break;
		}
	}*/
	//lmb->compareComputationTimes();
	/*vz = new VisualizerLM(lmb->getLineManager(0));
	vz->showStatic(0, 2);
	lmb->compareVoltages("lv1.101_load_1");
	vz->showPowers({ "lv1.101_load_1","lv1.101_load_2","lv1.101_load_3","lv1.101_load_11","lv1.101_load_5","lv1.101_load_6","mv1.101-lv1.101-trafo_1" });
	vz->showCurrents({ "lv1.101_load_1","lv1.101_load_2","lv1.101_load_3","lv1.101_load_11","lv1.101_load_5","lv1.101_load_6","mv1.101-lv1.101-trafo_1" });
	//vz->showVoltages({ 0,2,4,6,8 ,10 });
	//vz->showCurrents({ 0,2,4,6,8 ,10 });
	exit(0);*/


	//exampleRealGrid();
	//exampleRealGrid3();
	//exit(0);
	eval(40);
	exit(0);

	/*example1();
	example2();
	example3();
	example4();
	example5();
	example6();
	exampleRealGrid();*/
	//exampleSimBench("1-LV-rural1--0-no_sw");
	//exampleSimBench("1-LV-rural2--0-no_sw");
	//exampleSimBench("1-LV-rural3--0-no_sw");
	//exampleSimBench("1-LV-semiurb4--0-no_sw");
	//exampleSimBench("1-LV-semiurb5--0-no_sw");
	//exampleSimBench("1-LV-urban6--0-no_sw");
	exit(0);

	/*	int participants = 8;
		std::vector<std::string> solverNames = { "Newton","Gradient","FLPF","FGradient" };//,"Gradient","FLPF"
		LineManagerBenchmark* lmb = new LineManagerBenchmark(solverNames, true);
		lmb->setupGridNetwork(participants);
		lmb->requestAll(new EPPowerRectangular(6, 2, 14000, 5, 4));
		lmb->requestAll(new EPPowerRectangular(8, 2, 14000, 6, 3));
		lmb->requestAll(new EPPowerRectangular(4, 10, 10000, 7, 2));
		lmb->requestAll(new EPPowerTrapeze(2, 6, 20000, 30000, 4, 40));


		VisualizerCurve* test = new VisualizerCurve();
		int index = 2;
		for (index = 0; index < 5; index++)
		{
			for (int i = 0; i < solverNames.size(); i++)
			{
				test->addCurve(lmb->getLineManager(i)->getSolverLoggingReference()->getNorms(index), solverNames[i]);
				std::cout << solverNames[i] << ": " << lmb->getLineManager(i)->getSolverLoggingReference()->getNorms(index)[0] << " " << lmb->getLineManager(i)->getSolverLoggingReference()->getNorms(index)[lmb->getLineManager(i)->getSolverLoggingReference()->getNorms(index).size() - 1] << std::endl;
				//	std::cout << solverNames[i] << std::endl;
			}
			test->show(true, false);
		}

		lmb->compareComputationTimes();*/
	return 0;
}
