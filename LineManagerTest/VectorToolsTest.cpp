#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <math/VectorTools.h>



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VectorToolsTest
{
	TEST_CLASS(VectorToolsTest)
	{
	public:
		void assertVectorEqual(std::vector<double> v1, std::vector<double>  v2)
		{
			if (v1.size() != v2.size())
			{
				Assert::Fail();
			}
			if (!std::equal(v1.begin(), v1.end(), v2.begin()))
			{
				Assert::Fail();
			}
		}
		std::vector<double> v1{ 10, 10, -10 ,-10 };
		std::vector<double> v2{ 10, -10, 10 ,-10 };
		std::vector<double> vs{ 10, -10, 10 };

		TEST_METHOD(VectorSubtract)
		{
			std::vector<double> res(4, 1);
			VectorTools::subtract(&v1, &v2, &res);
			std::vector<double> resAssumed{ 0, 20, -20, 0 };
			assertVectorEqual(resAssumed, res);
		}
		TEST_METHOD(VectorSubtractReturn)
		{
			std::vector<double> res(4, 1);
			res = VectorTools::subtract(v1, v2);
			std::vector<double> resAssumed{ 0, 20, -20, 0 };
			assertVectorEqual(resAssumed, res);
		}


		TEST_METHOD(VectorAdd)
		{
			std::vector<double> res(4, 1);
			VectorTools::add(v1, v2, &res);
			std::vector<double> resAssumed{ 20, 0, 0,-20 };
			assertVectorEqual(resAssumed, res);
		}
		TEST_METHOD(VectorAddReturn)
		{
			std::vector<double> res(4, 1);
			res = VectorTools::add(v1, v2);
			std::vector<double> resAssumed{ 20, 0, 0,-20 };
			assertVectorEqual(resAssumed, res);
		}

		TEST_METHOD(DifferentLengthVectorSubtract)
		{
			std::vector<double> res(4, 1);
			VectorTools::subtract(&v1, &vs, &res);
			std::vector<double> resAssumed(4, 1);
			assertVectorEqual(resAssumed, res);
		}
		TEST_METHOD(DifferentLengthVectorSubtractReturn)
		{
			std::vector<double> res(4, 1);
			res = VectorTools::subtract(v1, vs);
			std::vector<double> resAssumed;
			assertVectorEqual(resAssumed, res);
		}
		TEST_METHOD(DifferentLengthVectorAdd)
		{
			std::vector<double> res(4, 1);
			VectorTools::add(v1, vs, &res);
			std::vector<double> resAssumed(4, 1);
			assertVectorEqual(resAssumed, res);
		}
		TEST_METHOD(DifferentLengthVectorAddReturn)
		{
			std::vector<double> res(4, 1);
			res = VectorTools::add(v1, vs);
			std::vector<double> resAssumed;
			assertVectorEqual(resAssumed, res);
		}

		TEST_METHOD(Equal)
		{
			Assert::IsFalse(VectorTools::equal(v1, v2));
			Assert::IsFalse(VectorTools::equal(v1, vs));
			Assert::IsFalse(VectorTools::equal(v1, { 10, 10.0005, -10 ,-10 }));
			Assert::IsTrue(VectorTools::equal(v1, { 10, 10, -10 ,-10 }));
		}

	};
}