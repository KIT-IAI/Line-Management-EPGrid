#include "pch.h"
#include "CppUnitTest.h"

#include <model/gridElements/CurrentSource.h>
#include <model/gridElements/VoltageSource.h>
#include <model/gridElements/Coupler.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NodeTest
{
	TEST_CLASS(NodeTest)
	{
	public:
		void sameSetpointsCheck(std::map<int, double> setpoints1, std::map<int, double>  setpoints2)
		{
			if (setpoints1.size() != setpoints2.size())
			{
				Assert::Fail();
			}
			if (!std::equal(setpoints1.begin(), setpoints1.end(), setpoints2.begin()))
			{
				Assert::Fail();
			}
		}
		TEST_METHOD(NodeSerialization1)
		{
			Node* n = new CurrentSource("0");
		
			std::string res = n->serialize();
			std::cout << res << std::endl;
			std::string exp = "0,CC";
			Assert::AreEqual(exp, res);
		}
		TEST_METHOD(NodeSerialization2)
		{
			Node* n = new Coupler("0");
			std::string res = n->serialize();
			std::string exp = "0,C";
			Assert::AreEqual(exp, res);
		}
		TEST_METHOD(NodeSerialization3)
		{
			Node* n = new VoltageSource("0", 230);
			std::string res = n->serialize();
			std::string exp = "0,VC";
			Assert::AreEqual(exp, res);
		}

		TEST_METHOD(NodeSetVoltages)
		{
			Node* v = new VoltageSource("0", 230);
			v->setVoltage(1, 5, false);
			v->setVoltage(5, 10, false);
			v->setVoltage(-5, 7, false);
			v->setVoltage(10, 3, false);
			v->setVoltage(33, 6, false);
			v->setVoltage(-22, 5, false);
			v->setVoltage(-33, 6, false);
			Assert::AreEqual((double)-22, v->getVoltage(5));
			Assert::AreEqual((double)5, v->getVoltage(10));
			Assert::AreEqual((double)-5, v->getVoltage(7));
			Assert::AreEqual((double)10, v->getVoltage(3));
			Assert::AreEqual((double)-33, v->getVoltage(6));
		}

		TEST_METHOD(NodeGetVoltage)
		{
			Node* v = new VoltageSource("0", 230);
			v->setVoltage(1, 5, false);
			v->setVoltage(5, 10, false);
			v->setVoltage(-5, 15, false);
			Assert::AreEqual((double)1, v->getVoltage(7));
			Assert::AreEqual((double)5, v->getVoltage(10));
			Assert::AreEqual((double)5, v->getVoltage(13));
			Assert::AreEqual((double)-5, v->getVoltage(17));
		}
		TEST_METHOD(CCAddSetpoint)
		{
			CurrentSource* cc = new CurrentSource("0");
			cc->addPowerSetpoint(200, 1, 4);
			Assert::AreEqual((double)0, cc->getPowerSetpoint(0));
			Assert::AreEqual((double)200, cc->getPowerSetpoint(1));
			Assert::AreEqual((double)200, cc->getPowerSetpoint(2));
			Assert::AreEqual((double)200, cc->getPowerSetpoint(4));
			Assert::AreEqual((double)0, cc->getPowerSetpoint(5));

		}
		TEST_METHOD(CCAddSetpointOneSideOverlap)
		{
			CurrentSource* cc = new CurrentSource("0");
			cc->addPowerSetpoint(200, 1, 3);
			cc->addPowerSetpoint(200, 3, 3);
			Assert::AreEqual((double)0, cc->getPowerSetpoint(0));
			Assert::AreEqual((double)200, cc->getPowerSetpoint(1));
			Assert::AreEqual((double)400, cc->getPowerSetpoint(3));
			Assert::AreEqual((double)200, cc->getPowerSetpoint(4));
			Assert::AreEqual((double)200, cc->getPowerSetpoint(5));
			Assert::AreEqual((double)0, cc->getPowerSetpoint(6));
		}
		TEST_METHOD(CCAddSetpointCompleteOverlap)
		{
			CurrentSource* cc = new CurrentSource("0");
			cc->addPowerSetpoint(200, 1, 6);
			cc->addPowerSetpoint(200, 3, 2);
			Assert::AreEqual((double)0, cc->getPowerSetpoint(0));
			Assert::AreEqual((double)200, cc->getPowerSetpoint(1));
			Assert::AreEqual((double)400, cc->getPowerSetpoint(3));
			Assert::AreEqual((double)200, cc->getPowerSetpoint(5));
			Assert::AreEqual((double)0, cc->getPowerSetpoint(7));
		}

		TEST_METHOD(CCRemoveSetpoint1)
		{
			CurrentSource* cc1 = new CurrentSource("0");
			CurrentSource* cc2 = new CurrentSource("0");
			cc1->addPowerSetpoint(200, 1, 6);
			cc1->removePowerSetpoint(200, 1, 6);
			sameSetpointsCheck(cc1->getPowerSetpoints(), cc2->getPowerSetpoints());
		}

		TEST_METHOD(CCRemoveSetpoint2)
		{
		CurrentSource* cc1 = new CurrentSource("0");
			CurrentSource* cc2 = new CurrentSource("0");
			cc1->addPowerSetpoint(200, 1, 3);
			cc1->addPowerSetpoint(200, 3, 3);
			cc2->addPowerSetpoint(200, 1, 3);
			cc1->removePowerSetpoint(200, 3, 3);
			sameSetpointsCheck(cc1->getPowerSetpoints(), cc2->getPowerSetpoints());

		}
		TEST_METHOD(CCRemoveSetpoint3)
		{
			CurrentSource* cc1 = new CurrentSource("0");
			CurrentSource* cc2 = new CurrentSource("0");
			cc1->addPowerSetpoint(200, 1, 6);
			cc1->addPowerSetpoint(200, 3, 2);
			cc2->addPowerSetpoint(200, 1, 6);
			cc1->removePowerSetpoint(200, 3, 2);
			sameSetpointsCheck(cc1->getPowerSetpoints(), cc2->getPowerSetpoints());
		}
		TEST_METHOD(CCRemoveSetpoint4)
		{
			CurrentSource* cc1 = new CurrentSource("0");
			CurrentSource* cc2 = new CurrentSource("0");
			cc1->addPowerSetpoint(200, 1, 6);
			cc1->addPowerSetpoint(200, 3, 2);
			cc2->addPowerSetpoint(200, 3, 2);
			cc1->removePowerSetpoint(200, 1, 6);
			sameSetpointsCheck(cc1->getPowerSetpoints(), cc2->getPowerSetpoints());
		}


	};
}