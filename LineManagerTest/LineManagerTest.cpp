#include "pch.h"
#include "CppUnitTest.h"

#include<control/LineManager.h>
#include<control/LineManagerHelper.h>
#include <model/gridElements/CurrentController.h>
#include <model/gridElements/VoltageController.h>
#include <model/gridElements/Coupler.h>
#include <model/gridElements/CableTypes.h>
#include <model/packets/EPPowerRectangular.h>
#include <math/Matrix.h>
#include <math/LSSolver.h>
#include <config.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LineManagerTest
{
	TEST_CLASS(LineManagerTest)
	{
		LineManager* lm = new LineManager();
	public:
		void basicSetup()
		{
			lm->addVoltageController("0", 230);
			lm->addCoupler("1");
		}
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
		TEST_METHOD(AddNode)
		{
			bool res = lm->addVoltageController("0", 230);
			Assert::IsTrue(res);
		}
		TEST_METHOD(AddExistingNode)
		{
			lm->addVoltageController("0", 230);
			bool res = lm->addVoltageController("0", 230);
			Assert::IsFalse(res);
		}

		TEST_METHOD(AddVoltageControllerAfterInit)
		{
			basicSetup();
			lm->addLine("0", "1", 20);
			lm->initGrid();
			bool res = lm->addVoltageController("2", 230);
			Assert::IsFalse(res);
		}

		TEST_METHOD(AddCurrentControllerAfterInit)
		{
			basicSetup();
			lm->addLine("0", "1", 20);
			lm->initGrid();
			bool res = lm->addCurrentController("2");
			Assert::IsFalse(res);
		}

		TEST_METHOD(AddCouplerAfterInit)
		{
			basicSetup();
			lm->addLine("0", "1", 20);
			lm->initGrid();
			bool res = lm->addCoupler("2");
			Assert::IsFalse(res);
		}
		TEST_METHOD(AddLine)
		{
			basicSetup();
			bool res = lm->addLine("0", "1", 20);
			Assert::IsTrue(res);
		}

		TEST_METHOD(AddExistingLine)
		{
			basicSetup();
			lm->addLine("0", "1", 20);
			bool res = lm->addLine("0", "1", 20);
			Assert::IsFalse(res);
		}
		TEST_METHOD(AddExistingLine1)
		{
			basicSetup();
			lm->addLine("0", "1", 20);
			bool res = lm->addLine("1", "0", 20);
			Assert::IsFalse(res);
		}
		TEST_METHOD(AddLineWithoutNode)
		{
			basicSetup();
			bool res = lm->addLine("0", "5", 20);
			Assert::IsFalse(res);
		}
		TEST_METHOD(AddLineToNodeItself)
		{
			basicSetup();
			bool res = lm->addLine("1", "1", 20);
			Assert::IsFalse(res);
		}
		TEST_METHOD(GradientSolver)
		{
			lm = new LineManager("Gradient");
			LineManagerHelper::setupGridNetwork(lm, 6);
			lm->requestEP(new EPPowerRectangular("4", "6", 20000, 0, 4));
			lm->requestEP(new EPPowerRectangular("2", "8", 15000, 1, 3));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));

			std::vector<double> voltages0 = { 230,229.992,229.922,229.922,230.963,229.853,225.687,226.823,226.823,226.823,226.823,226.823 };
			std::vector<double> voltages1 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages2 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages3 = { 230,230.504,235.508,234.692,237.779,236.701,234.742,235.834,237.069,237.88,243.669,242.091 };
			std::vector<double> voltages4 = { 230,230,230,230,230,230,230,230,230,230,230,230 };

			vectorEqual(voltages0, lm->getDataModelReference()->getNodeVoltages(0 / S_PER_TIMESLOT));
			vectorEqual(voltages1, lm->getDataModelReference()->getNodeVoltages(1 / S_PER_TIMESLOT));
			vectorEqual(voltages2, lm->getDataModelReference()->getNodeVoltages(2 / S_PER_TIMESLOT));
			vectorEqual(voltages3, lm->getDataModelReference()->getNodeVoltages(3 / S_PER_TIMESLOT));
			vectorEqual(voltages4, lm->getDataModelReference()->getNodeVoltages(4 / S_PER_TIMESLOT));
		}
		TEST_METHOD(NewtonSolver)
		{
			lm = new LineManager("Newton");
			LineManagerHelper::setupGridNetwork(lm, 6);
			lm->requestEP(new EPPowerRectangular("4", "6", 20000, 0, 4));
			lm->requestEP(new EPPowerRectangular("2", "8", 15000, 1, 3));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));

			std::vector<double> voltages0 = { 230,229.992,229.922,229.922,230.963,229.853,225.687,226.823,226.823,226.823,226.823,226.823 };
			std::vector<double> voltages1 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages2 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages3 = { 230,230.504,235.508,234.692,237.779,236.701,234.742,235.834,237.069,237.88,243.669,242.091 };
			std::vector<double> voltages4 = { 230,230,230,230,230,230,230,230,230,230,230,230 };

			vectorEqual(voltages0, lm->getDataModelReference()->getNodeVoltages(0 / S_PER_TIMESLOT));
			vectorEqual(voltages1, lm->getDataModelReference()->getNodeVoltages(1 / S_PER_TIMESLOT));
			vectorEqual(voltages2, lm->getDataModelReference()->getNodeVoltages(2 / S_PER_TIMESLOT));
			vectorEqual(voltages3, lm->getDataModelReference()->getNodeVoltages(3 / S_PER_TIMESLOT));
			vectorEqual(voltages4, lm->getDataModelReference()->getNodeVoltages(4 / S_PER_TIMESLOT));
		}

		TEST_METHOD(FGradientSolver)
		{
			lm = new LineManager("FGradient");
			LineManagerHelper::setupGridNetwork(lm, 6);
			lm->requestEP(new EPPowerRectangular("4", "6", 20000, 0, 4));
			lm->requestEP(new EPPowerRectangular("2", "8", 15000, 1, 3));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));

			std::vector<double> voltages0 = { 230,229.992,229.922,229.922,230.963,229.853,225.687,226.823,226.823,226.823,226.823,226.823 };
			std::vector<double> voltages1 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages2 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages3 = { 230,230.504,235.508,234.692,237.779,236.701,234.742,235.834,237.069,237.88,243.669,242.091 };
			std::vector<double> voltages4 = { 230,230,230,230,230,230,230,230,230,230,230,230 };

			vectorEqual(voltages0, lm->getDataModelReference()->getNodeVoltages(0 / S_PER_TIMESLOT));
			vectorEqual(voltages1, lm->getDataModelReference()->getNodeVoltages(1 / S_PER_TIMESLOT));
			vectorEqual(voltages2, lm->getDataModelReference()->getNodeVoltages(2 / S_PER_TIMESLOT));
			vectorEqual(voltages3, lm->getDataModelReference()->getNodeVoltages(3 / S_PER_TIMESLOT));
			vectorEqual(voltages4, lm->getDataModelReference()->getNodeVoltages(4 / S_PER_TIMESLOT));
		}
		TEST_METHOD(COP)
		{
			lm = new LineManager("COP");
			LineManagerHelper::setupGridNetwork(lm, 6);
			lm->requestEP(new EPPowerRectangular("4", "6", 20000, 0, 4));
			lm->requestEP(new EPPowerRectangular("2", "8", 15000, 1, 3));
			lm->requestEP(new EPPowerRectangular("10", "0", 30000, 3, 1));

			std::vector<double> voltages0 = { 230,229.992,229.922,229.922,230.963,229.853,225.687,226.823,226.823,226.823,226.823,226.823 };
			std::vector<double> voltages1 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages2 = { 230,229.973,230.58,229.746,228.418,227.295,220.688,221.85,218.624,219.504,219.504,219.504 };
			std::vector<double> voltages3 = { 230,230.504,235.508,234.692,237.779,236.701,234.742,235.834,237.069,237.88,243.669,242.091 };
			std::vector<double> voltages4 = { 230,230,230,230,230,230,230,230,230,230,230,230 };

			vectorEqual(voltages0, lm->getDataModelReference()->getNodeVoltages(0 / S_PER_TIMESLOT));
			vectorEqual(voltages1, lm->getDataModelReference()->getNodeVoltages(1 / S_PER_TIMESLOT));
			vectorEqual(voltages2, lm->getDataModelReference()->getNodeVoltages(2 / S_PER_TIMESLOT));
			vectorEqual(voltages3, lm->getDataModelReference()->getNodeVoltages(3 / S_PER_TIMESLOT));
			vectorEqual(voltages4, lm->getDataModelReference()->getNodeVoltages(4 / S_PER_TIMESLOT));
		}	};
}