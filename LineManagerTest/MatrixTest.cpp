#include "pch.h"
#include "CppUnitTest.h"

#include <math/Matrix.h>
#include <math/LSSolver.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MatrixTest
{
	TEST_CLASS(MatrixTest)
	{
	public:
		Matrix* m;
		void initMatrix()
		{
			m = new Matrix(2, 4);
			m->setValue(0, 0, -1);
			m->setValue(0, 1, 3);
			m->setValue(0, 2, 0);
			m->setValue(0, 3, 4);
			m->setValue(1, 0, 2);
			m->setValue(1, 1, 1);
			m->setValue(1, 2, -5);
			m->setValue(1, 3, 2);
		}
		void initMatrix33()
		{
			m = new Matrix(3, 3);
			m->setValue(0, 0, -1);
			m->setValue(0, 1, 3);
			m->setValue(0, 2, 0);
			m->setValue(1, 0, 10);
			m->setValue(1, 1, 4);
			m->setValue(1, 2, -2);
			m->setValue(2, 0, -5);
			m->setValue(2, 1, 2);
			m->setValue(2, 2, 7);
		}
		void validateIdentity(Matrix* m)
		{
			if (m->getColumnCount() != m->getRowCount())
			{
				Assert::Fail();
			}
			int dimension = m->getRowCount();
			for (int i = 0; i < dimension; i++)
			{
				for (int j = 0; j < dimension; j++)
				{
					if (i == j)
					{
						Assert::AreEqual((double)1, m->getValue(i, j));
					}
					else
					{
						Assert::AreEqual((double)0, m->getValue(i, j));
					}
				}
			}
		}
		void validateIdentityTolerant(Matrix* m)
		{
			if (m->getColumnCount() != m->getRowCount())
			{
				Assert::Fail();
			}
			int dimension = m->getRowCount();
			for (int i = 0; i < dimension; i++)
			{
				for (int j = 0; j < dimension; j++)
				{
					if (i == j)
					{
						Assert::IsTrue(abs(1 - m->getValue(i, j)) < 1e-10);
					}
					else
					{
						Assert::IsTrue(abs(0 - m->getValue(i, j)) < 1e-10);
					}
				}
			}
		}
		TEST_METHOD(CreateMatrix)
		{
			initMatrix();
			Assert::AreEqual((double)-1, m->getValue(0, 0));
			Assert::AreEqual((double)3, m->getValue(0, 1));
			Assert::AreEqual((double)0, m->getValue(0, 2));
			Assert::AreEqual((double)4, m->getValue(0, 3));
			Assert::AreEqual((double)2, m->getValue(1, 0));
			Assert::AreEqual((double)1, m->getValue(1, 1));
			Assert::AreEqual((double)-5, m->getValue(1, 2));
			Assert::AreEqual((double)2, m->getValue(1, 3));
		}
		TEST_METHOD(CreateIdentiyMatrices)
		{
			Matrix* m = new Matrix(2);
			validateIdentity(m);
			m = new Matrix(5);
			validateIdentity(m);
			m = new Matrix(10);
			validateIdentity(m);

		}
		TEST_METHOD(MatrixCopyConstructor)
		{
			initMatrix();
			Matrix* n = new Matrix(m);
			m->setValue(0, 0, 9);
			m->setValue(0, 1, 7);
			m->setValue(0, 2, -5);
			m->setValue(0, 3, 8);
			m->setValue(1, 0, -2);
			m->setValue(1, 1, -16);
			m->setValue(1, 2, -3);
			m->setValue(1, 3, 5);

			Assert::AreEqual((double)-1, n->getValue(0, 0));
			Assert::AreEqual((double)3, n->getValue(0, 1));
			Assert::AreEqual((double)0, n->getValue(0, 2));
			Assert::AreEqual((double)4, n->getValue(0, 3));
			Assert::AreEqual((double)2, n->getValue(1, 0));
			Assert::AreEqual((double)1, n->getValue(1, 1));
			Assert::AreEqual((double)-5, n->getValue(1, 2));
			Assert::AreEqual((double)2, n->getValue(1, 3));
		}
		TEST_METHOD(MatrixAddValue)
		{
			Matrix* m = new Matrix(2, 2);
			m->setValue(0, 0, -1);
			m->setValue(0, 1, 3);
			m->setValue(1, 0, -50);
			m->setValue(1, 1, 4);

			m->addValue(0, 0, 5);
			m->addValue(0, 1, 5);
			m->addValue(1, 0, -5);
			m->addValue(1, 1, -5);

			Assert::AreEqual((double)4, m->getValue(0, 0));
			Assert::AreEqual((double)8, m->getValue(0, 1));
			Assert::AreEqual((double)-55, m->getValue(1, 0));
			Assert::AreEqual((double)-1, m->getValue(1, 1));

		}
		TEST_METHOD(MatrixRowAdd)
		{
			initMatrix33();
			m->rowAdd(1, 2, (double)1 / 2);
			Assert::AreEqual((double)-1, m->getValue(0, 0));
			Assert::AreEqual((double)3, m->getValue(0, 1));
			Assert::AreEqual((double)0, m->getValue(0, 2));
			Assert::AreEqual((double)10, m->getValue(1, 0));
			Assert::AreEqual((double)4, m->getValue(1, 1));
			Assert::AreEqual((double)-2, m->getValue(1, 2));
			Assert::AreEqual((double)0, m->getValue(2, 0));
			Assert::AreEqual((double)4, m->getValue(2, 1));
			Assert::AreEqual((double)6, m->getValue(2, 2));
		}
		TEST_METHOD(MatrixRowSwap)
		{
			initMatrix33();
			m->swapRows(1, 2);
			Assert::AreEqual((double)-1, m->getValue(0, 0));
			Assert::AreEqual((double)3, m->getValue(0, 1));
			Assert::AreEqual((double)0, m->getValue(0, 2));
			Assert::AreEqual((double)-5, m->getValue(1, 0));
			Assert::AreEqual((double)2, m->getValue(1, 1));
			Assert::AreEqual((double)7, m->getValue(1, 2));
			Assert::AreEqual((double)10, m->getValue(2, 0));
			Assert::AreEqual((double)4, m->getValue(2, 1));
			Assert::AreEqual((double)-2, m->getValue(2, 2));
		}

		TEST_METHOD(MatrixRowScale)
		{
			initMatrix33();
			m->rowScale(0, (double)3);
			m->rowScale(1, (double)1 / 2);
			m->rowScale(2, (double)-2);
			Assert::AreEqual((double)-3, m->getValue(0, 0));
			Assert::AreEqual((double)9, m->getValue(0, 1));
			Assert::AreEqual((double)0, m->getValue(0, 2));
			Assert::AreEqual((double)5, m->getValue(1, 0));
			Assert::AreEqual((double)2, m->getValue(1, 1));
			Assert::AreEqual((double)-1, m->getValue(1, 2));
			Assert::AreEqual((double)10, m->getValue(2, 0));
			Assert::AreEqual((double)-4, m->getValue(2, 1));
			Assert::AreEqual((double)-14, m->getValue(2, 2));
		}

		TEST_METHOD(MatrixMultiplication)
		{
			Matrix* m = new Matrix(2, 4);
			m->setValue(0, 0, -1);
			m->setValue(0, 1, 3);
			m->setValue(0, 2, 0);
			m->setValue(0, 3, 4);
			m->setValue(1, 0, 2);
			m->setValue(1, 1, 1);
			m->setValue(1, 2, -5);
			m->setValue(1, 3, 2);
			Matrix* n = new Matrix(4, 2);
			n->setValue(0, 0, 5);
			n->setValue(0, 1, 2);
			n->setValue(1, 0, -1);
			n->setValue(1, 1, 3);
			n->setValue(2, 0, 2);
			n->setValue(2, 1, 1);
			n->setValue(3, 0, 0);
			n->setValue(3, 1, 2);

			Matrix* res = Matrix::multiply(m, n);

			Assert::AreEqual((double)-8, res->getValue(0, 0));
			Assert::AreEqual((double)15, res->getValue(0, 1));
			Assert::AreEqual((double)-1, res->getValue(1, 0));
			Assert::AreEqual((double)6, res->getValue(1, 1));
		}

		TEST_METHOD(MatrixMultiplicationVector)
		{
			Matrix* m = new Matrix(2, 4);
			m->setValue(0, 0, -1);
			m->setValue(0, 1, 3);
			m->setValue(0, 2, 0);
			m->setValue(0, 3, 4);
			m->setValue(1, 0, 2);
			m->setValue(1, 1, 1);
			m->setValue(1, 2, -5);
			m->setValue(1, 3, 2);

			std::vector<double> n;
			n.push_back(5);
			n.push_back(9);
			n.push_back(-2);
			n.push_back(7);

			std::vector<double> res = Matrix::multiply(m, n);
			Assert::AreEqual((double)50, res[0]);
			Assert::AreEqual((double)43, res[1]);
			//Assert::AreEqual((double)-1, res[2]);
			//Assert::AreEqual((double)6, res[3]);
		}
		TEST_METHOD(MatrixInvert)
		{
			initMatrix33();
			Matrix* invers = Matrix::invert(m);
			Matrix* identity = Matrix::multiply(m, invers);
			validateIdentityTolerant(identity);
		}

		TEST_METHOD(RandomMatrixInvert)
		{
			srand(time(NULL));
			Matrix* invers;
			Matrix* identity;
			for (int k = 0; k < 60; k++)
			{
				int size = rand() % 101;
				m = new Matrix(size, size);

				for (int i = 0; i < m->getRowCount(); i++)
				{
					for (int j = 0; j < m->getColumnCount(); j++)
					{
						m->setValue(i, j, (rand() % 201) - 100);
					}
				}
				invers = Matrix::invert(m);
				if(invers==NULL)
				{
					continue;
				}
				identity = Matrix::multiply(m, invers);
				validateIdentityTolerant(identity);
			}
		}

		TEST_METHOD(LUDecomposition)
		{
			//Example based on https://www3.math.tu-berlin.de/Vorlesungen/WS12/NumMath1/uebung/LR-Zerlegung.pdf
			Matrix* a = new Matrix(3, 3);
			a->setValue(0, 0, 2);
			a->setValue(0, 1, 4);
			a->setValue(0, 2, -2);
			a->setValue(1, 0, 4);
			a->setValue(1, 1, 9);
			a->setValue(1, 2, -3);
			a->setValue(2, 0, -2);
			a->setValue(2, 1, -3);
			a->setValue(2, 2, 7);
			LSSolver* ls = new LSSolver(a);
			Matrix* l = ls->getL();
			Matrix* r = ls->getR();
			Matrix* p = ls->getP();

			Assert::AreEqual((double)1, l->getValue(0, 0));
			Assert::AreEqual((double)0, l->getValue(0, 1));
			Assert::AreEqual((double)0, l->getValue(0, 2));
			Assert::AreEqual((double)-0.5, l->getValue(1, 0));
			Assert::AreEqual((double)1, l->getValue(1, 1));
			Assert::AreEqual((double)0, l->getValue(1, 2));

			Assert::AreEqual((double)0.5, l->getValue(2, 0));
			Assert::AreEqual((double)-1 / 3, l->getValue(2, 1));
			Assert::AreEqual((double)1, l->getValue(2, 2));


			Assert::AreEqual((double)4, r->getValue(0, 0));
			Assert::AreEqual((double)9, r->getValue(0, 1));
			Assert::AreEqual((double)-3, r->getValue(0, 2));
			Assert::AreEqual((double)0, r->getValue(1, 0));
			Assert::AreEqual((double)3 / 2, r->getValue(1, 1));
			Assert::AreEqual((double)11 / 2, r->getValue(1, 2));
			Assert::AreEqual((double)0, r->getValue(2, 0));
			Assert::AreEqual((double)0, r->getValue(2, 1));
			Assert::AreEqual((double)4 / 3, r->getValue(2, 2));

			Assert::AreEqual((double)0, p->getValue(0, 0));
			Assert::AreEqual((double)1, p->getValue(0, 1));
			Assert::AreEqual((double)0, p->getValue(0, 2));
			Assert::AreEqual((double)0, p->getValue(1, 0));
			Assert::AreEqual((double)0 / 2, p->getValue(1, 1));
			Assert::AreEqual((double)1, p->getValue(1, 2));
			Assert::AreEqual((double)1, p->getValue(2, 0));
			Assert::AreEqual((double)0, p->getValue(2, 1));
			Assert::AreEqual((double)0, p->getValue(2, 2));
		}

		TEST_METHOD(LSSolverSolve)
		{
			//Example based on https://www3.math.tu-berlin.de/Vorlesungen/WS12/NumMath1/uebung/LR-Zerlegung.pdf
			Matrix* a = new Matrix(3, 3);
			a->setValue(0, 0, 2);
			a->setValue(0, 1, 4);
			a->setValue(0, 2, -2);
			a->setValue(1, 0, 4);
			a->setValue(1, 1, 9);
			a->setValue(1, 2, -3);
			a->setValue(2, 0, -2);
			a->setValue(2, 1, -3);
			a->setValue(2, 2, 7);
			LSSolver* ls = new LSSolver(a);



			std::vector<double> b;
			b.push_back(2);
			b.push_back(8);
			b.push_back(10);
			std::vector<double> x = ls->getSolution(b);

			Assert::IsTrue(abs((double)-1 - x[0]) < 0.1e-12);
			Assert::IsTrue(abs(2 - x[1]) < 0.1e-12);
			Assert::IsTrue(abs(2 - x[2]) < 0.1e-12);
		}


	};
}