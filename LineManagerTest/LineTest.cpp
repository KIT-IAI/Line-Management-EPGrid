#include "pch.h"
#include "CppUnitTest.h"

#include<model/gridElements/Line.h>
#include <model/gridElements/CurrentController.h>
#include <model/gridElements/VoltageController.h>
#include <model/gridElements/Coupler.h>



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LineTest
{
	TEST_CLASS(LineTest)
	{
	public:
		Node* c0 = new Coupler("0");
		Node* c1 = new Coupler("1");
		Node* vc = new VoltageController("2", 230);
		Node* cc = new CurrentController("3");
		TEST_METHOD(LineSerializationStatic)
		{
			Line* l = new Line(c0, c1, 1000);
			std::string res = l->serializeStatic();
			std::string exp = "0,1,0.20600,275";
			Assert::AreEqual(exp, res);
		}

		TEST_METHOD(LineGetNodes)
		{
			Line* l = new Line(c0, c1, 1000);
			Assert::AreEqual(c0->getID(), l->getNodeIDs().first);
			Assert::AreEqual(c1->getID(), l->getNodeIDs().second);
		}
		TEST_METHOD(AutoLineParameterCC_C)
		{
			Line* l = new Line(cc, c0, 1000);
			double res = l->getResistance();
			Assert::AreEqual(CableTypes::getResistance(CableTypes::Types::NAYY50SE), res);
			Assert::AreEqual(cc->getID(), l->getNodeIDs().second);
			Assert::AreEqual(c0->getID(), l->getNodeIDs().first);
		}
		TEST_METHOD(AutoLineParameterC_CC)
		{
			Line* l = new Line(c0, cc, 1000);
			double res = l->getResistance();
			Assert::AreEqual(CableTypes::getResistance(CableTypes::Types::NAYY50SE), res);
			Assert::AreEqual(c0->getID(), l->getNodeIDs().first);
			Assert::AreEqual(cc->getID(), l->getNodeIDs().second);
		}
		TEST_METHOD(AutoLineParameterVC_C)
		{
			Line* l = new Line(vc, c0, 1000);
			double res = l->getResistance();
			Assert::AreEqual(CableTypes::getResistance(CableTypes::Types::NAYY150SE), res);
			Assert::AreEqual(vc->getID(), l->getNodeIDs().first);
			Assert::AreEqual(c0->getID(), l->getNodeIDs().second);
		}
		TEST_METHOD(AutoLineParameterC_VC)
		{
			Line* l = new Line(c0, vc, 1000);
			double res = l->getResistance();
			Assert::AreEqual(CableTypes::getResistance(CableTypes::Types::NAYY150SE), res);
			Assert::AreEqual(c0->getID(), l->getNodeIDs().second);
			Assert::AreEqual(vc->getID(), l->getNodeIDs().first);
		}

		TEST_METHOD(AutoLineParameterC_C)
		{
			Line* l = new Line(c0, c1, 1000);
			double res = l->getResistance();
			Assert::AreEqual(CableTypes::getResistance(CableTypes::Types::NAYY150SE), res);
			Assert::AreEqual(c0->getID(), l->getNodeIDs().first);
			Assert::AreEqual(c1->getID(), l->getNodeIDs().second);
		}
	};
}