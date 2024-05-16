#include "pch.h"
#include "CppUnitTest.h"

#include<control/LineManager.h>
#include<model/GridModelGenerator.h>
#include <model/packets/EPPowerRectangular.h>
#include <config.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LineManagerTest
{
	TEST_CLASS(LineManagerTest)
	{
		LineManager* lm; 
	public:
		void vectorEqual(std::vector<double> a, std::vector<double> b)
		{
			if (a.size() != b.size())
			{
				Assert::Fail();
			}

			for (int i = 0; i < a.size(); i++)
			{
				Assert::IsTrue(abs(a[i] - b[i]) < 1e-3);
			}
		}
		TEST_METHOD(GradientSolver)
		{
			lm = new LineManager(GridModelGenerator::setupGridNetwork(6),"Gradient");
		
			lm->requestEP(new EPPowerRectangular("4", "6", 20000, 0, 4));
			lm->requestEP(new EPPowerRectangular("2", "8", 15000, 1, 3));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));

			std::vector<double> voltages0 = { 230,229.992,229.922,229.922,230.963,229.853,225.687,226.823,226.823,226.823,226.823,226.823 };
			std::vector<double> voltages1 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages2 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages3 = { 230,230.504,235.508,234.692,237.779,236.701,234.742,235.834,237.069,237.88,243.669,242.091 };
			std::vector<double> voltages4 = { 230,230,230,230,230,230,230,230,230,230,230,230 };

			vectorEqual(voltages0, lm->getGridModelReference()->getNodeVoltages(0 / S_PER_TIMESLOT));
			vectorEqual(voltages1, lm->getGridModelReference()->getNodeVoltages(1 / S_PER_TIMESLOT));
			vectorEqual(voltages2, lm->getGridModelReference()->getNodeVoltages(2 / S_PER_TIMESLOT));
			vectorEqual(voltages3, lm->getGridModelReference()->getNodeVoltages(3 / S_PER_TIMESLOT));
			vectorEqual(voltages4, lm->getGridModelReference()->getNodeVoltages(4 / S_PER_TIMESLOT));
		}
		TEST_METHOD(NewtonSolver)
		{
			lm = new LineManager(GridModelGenerator::setupGridNetwork(6), "Newton");
			lm->requestEP(new EPPowerRectangular("4", "6", 20000, 0, 4));
			lm->requestEP(new EPPowerRectangular("2", "8", 15000, 1, 3));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));

			std::vector<double> voltages0 = { 230,229.992,229.922,229.922,230.963,229.853,225.687,226.823,226.823,226.823,226.823,226.823 };
			std::vector<double> voltages1 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages2 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages3 = { 230,230.504,235.508,234.692,237.779,236.701,234.742,235.834,237.069,237.88,243.669,242.091 };
			std::vector<double> voltages4 = { 230,230,230,230,230,230,230,230,230,230,230,230 };

			vectorEqual(voltages0, lm->getGridModelReference()->getNodeVoltages(0 / S_PER_TIMESLOT));
			vectorEqual(voltages1, lm->getGridModelReference()->getNodeVoltages(1 / S_PER_TIMESLOT));
			vectorEqual(voltages2, lm->getGridModelReference()->getNodeVoltages(2 / S_PER_TIMESLOT));
			vectorEqual(voltages3, lm->getGridModelReference()->getNodeVoltages(3 / S_PER_TIMESLOT));
			vectorEqual(voltages4, lm->getGridModelReference()->getNodeVoltages(4 / S_PER_TIMESLOT));
		}

		TEST_METHOD(FGradientSolver)
		{
			lm = new LineManager(GridModelGenerator::setupGridNetwork(6), "FGradient");
			lm->requestEP(new EPPowerRectangular("4", "6", 20000, 0, 4));
			lm->requestEP(new EPPowerRectangular("2", "8", 15000, 1, 3));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));

			std::vector<double> voltages0 = { 230,229.992,229.922,229.922,230.963,229.853,225.687,226.823,226.823,226.823,226.823,226.823 };
			std::vector<double> voltages1 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages2 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages3 = { 230,230.504,235.508,234.692,237.779,236.701,234.742,235.834,237.069,237.88,243.669,242.091 };
			std::vector<double> voltages4 = { 230,230,230,230,230,230,230,230,230,230,230,230 };

			vectorEqual(voltages0, lm->getGridModelReference()->getNodeVoltages(0 / S_PER_TIMESLOT));
			vectorEqual(voltages1, lm->getGridModelReference()->getNodeVoltages(1 / S_PER_TIMESLOT));
			vectorEqual(voltages2, lm->getGridModelReference()->getNodeVoltages(2 / S_PER_TIMESLOT));
			vectorEqual(voltages3, lm->getGridModelReference()->getNodeVoltages(3 / S_PER_TIMESLOT));
			vectorEqual(voltages4, lm->getGridModelReference()->getNodeVoltages(4 / S_PER_TIMESLOT));
		}
		TEST_METHOD(COP)
		{
			lm = new LineManager(GridModelGenerator::setupGridNetwork(6), "COP");
			lm->requestEP(new EPPowerRectangular("4", "6", 20000, 0, 4));
			lm->requestEP(new EPPowerRectangular("2", "8", 15000, 1, 3));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));

			std::vector<double> voltages0 = { 230,229.992,229.922,229.922,230.963,229.853,225.687,226.823,226.823,226.823,226.823,226.823 };
			std::vector<double> voltages1 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages2 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages3 = { 230,230.504,235.508,234.692,237.779,236.701,234.742,235.834,237.069,237.88,243.669,242.091 };
			std::vector<double> voltages4 = { 230,230,230,230,230,230,230,230,230,230,230,230 };

			vectorEqual(voltages0, lm->getGridModelReference()->getNodeVoltages(0 / S_PER_TIMESLOT));
			vectorEqual(voltages1, lm->getGridModelReference()->getNodeVoltages(1 / S_PER_TIMESLOT));
			vectorEqual(voltages2, lm->getGridModelReference()->getNodeVoltages(2 / S_PER_TIMESLOT));
			vectorEqual(voltages3, lm->getGridModelReference()->getNodeVoltages(3 / S_PER_TIMESLOT));
			vectorEqual(voltages4, lm->getGridModelReference()->getNodeVoltages(4 / S_PER_TIMESLOT));
		}	};
}