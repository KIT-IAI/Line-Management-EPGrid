#include "pch.h"
#include "CppUnitTest.h"
#include<model/GridModel.h>
#include<model/GridModelHelper.h>



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GridModelTest
{
	TEST_CLASS(GridModelTest)
	{
		GridModel* model = new GridModel();
	public:
		void basicSetup()
		{
			GridModelHelper::addVoltageSource(model, "0", 230);
			GridModelHelper::addCoupler(model, "1");
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
			bool res = GridModelHelper::addVoltageSource(model, "0", 230);
			Assert::IsTrue(res);
		}
		TEST_METHOD(AddExistingNode)
		{
			GridModelHelper::addVoltageSource(model, "0", 230);
			bool res = GridModelHelper::addVoltageSource(model, "0", 230);
			Assert::IsFalse(res);
		}

		TEST_METHOD(AddVoltageControllerAfterInit)
		{
			basicSetup();
			GridModelHelper::addLine(model, "0", "1", 20);
			model->setLock(true);
			bool res = GridModelHelper::addVoltageSource(model, "2", 230);
			Assert::IsFalse(res);
		}

		TEST_METHOD(AddCurrentControllerAfterInit)
		{
			basicSetup();
			GridModelHelper::addLine(model, "0", "1", 20);
			model->setLock(true);
			bool res = GridModelHelper::addCurrentSource(model, "2");
			Assert::IsFalse(res);
		}

		TEST_METHOD(AddCouplerAfterInit)
		{
			basicSetup();
			GridModelHelper::addLine(model, "0", "1", 20);
			model->setLock(true);
			bool res = GridModelHelper::addCoupler(model, "2");
			Assert::IsFalse(res);
		}
		TEST_METHOD(AddLine)
		{
			basicSetup();
			bool res = GridModelHelper::addLine(model, "0", "1", 20);
			Assert::IsTrue(res);
		}

		TEST_METHOD(AddExistingLine)
		{
			basicSetup();
			GridModelHelper::addLine(model, "0", "1", 20);
			bool res = GridModelHelper::addLine(model, "0", "1", 20);
			Assert::IsFalse(res);
		}
		TEST_METHOD(AddExistingLine1)
		{
			basicSetup();
			GridModelHelper::addLine(model, "0", "1", 20);
			bool res = GridModelHelper::addLine(model, "1", "0", 20);
			Assert::IsFalse(res);
		}
		TEST_METHOD(AddLineWithoutNode)
		{
			basicSetup();
			bool res = GridModelHelper::addLine(model, "0", "5", 20);
			Assert::IsFalse(res);
		}
		TEST_METHOD(AddLineToNodeItself)
		{
			basicSetup();
			bool res = GridModelHelper::addLine(model, "1", "1", 20);
			Assert::IsFalse(res);
		}		
	};
}