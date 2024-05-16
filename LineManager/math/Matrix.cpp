#include "Matrix.h"
#include <string>

/** Constructor, preinitializes matrix with zeros.
* @param rows number of rows.
* @param columns number of columns.
**/
Matrix::Matrix(int rows, int columns)
{
	this->rows = rows;
	this->columns = columns;
	m.reserve(rows * columns);
	for (int i = 0; i < rows * columns; i++)
	{
		m.push_back(0);
	}
}
/** Constructor which creates an identity matrix.
* @param dimension The dimension of the identity matrix.**/
Matrix::Matrix(int dimension) :Matrix(dimension, dimension)
{
	for (int i = 0; i < dimension; i++)
	{
		setValue(i, i, 1);
	}
}

/** Contructor which generates a object copy of a given matrix.
* @param a a matrix.
**/
Matrix::Matrix(Matrix* a)
{
	*this = Matrix(a->getRowCount(), a->getColumnCount());

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			this->setValue(i, j, a->getValue(i, j));
		}
	}
}
/** Returns the number of rows of the matrix.
* @return number of rows. **/
int Matrix::getRowCount()
{
	return rows;
}
/** Returns the number of columns of the matrix.
* @return number of columns. **/
int Matrix::getColumnCount()
{
	return columns;
}
/** Returns the value of the matrix at a given position.
* @param row the row index of the requested value.
* @param column the column index of the requested value.
* @return value at the position. **/
double Matrix::getValue(int row, int column)
{
	if (!checkCoordiantes(row, column))
	{
		std::cerr << "Indices out of range" << std::endl;
		return NULL;
	}
	return m[convertCoordinatestoIndex(row, column)];
}
/** Sets the value of the matrix at a given position.
* @param row the row index of the value.
* @param column the column index of the value.
* @param value value to be set at the position. **/
void Matrix::setValue(int row, int column, double value)
{
	if (checkCoordiantes(row, column))
	{
		m[convertCoordinatestoIndex(row, column)] = value;
	}
}
/** Adds a given value to the existing value of the matrix at a given position.
* @param row the row index of the value.
* @param column the column index of the value.
* @param value value to be add at the position. **/
void Matrix::addValue(int row, int column, double value)
{
	if (checkCoordiantes(row, column))
	{
		m[convertCoordinatestoIndex(row, column)] += value;
	}

}
/** Multiplies to matrices.
* @param m a matrix.
* @param n a matrix.
* @return The result matrix. **/
Matrix* Matrix::multiply(Matrix* m, Matrix* n)
{
	if (m->getColumnCount() != n->getRowCount())
	{
		std::cerr << "Matrix dimension do not allow matrix multiplication" << std::endl;
		return NULL;
	}

	Matrix* res = new Matrix(m->getRowCount(), n->getColumnCount());
	double value;
	for (int i = 0; i < m->getRowCount(); i++)
	{
		for (int j = 0; j < n->getColumnCount(); j++)
		{
			value = 0;
			for (int k = 0; k < m->getColumnCount(); k++)
			{
				value += m->getValue(i, k) * n->getValue(k, j);
			}
			res->setValue(i, j, value);
		}
	}
	return res;
}
/** Multiplies a matrix with a vector.
* @param m a matrix.
* @param v a vector.
* @return The result vector. **/
std::vector<double> Matrix::multiply(Matrix* m, std::vector<double> v)
{
	std::vector<double> res;
	if (m->getColumnCount() != v.size())
	{
		std::cerr << "Matrix dimension do not allow matrix multiplication" << std::endl;
		return res;
	}
	res.reserve(m->getRowCount());
	double value;
	for (int i = 0; i < m->getRowCount(); i++)
	{
		value = 0;
		for (int k = 0; k < m->getColumnCount(); k++)
		{
			value += m->getValue(i, k) * v[k];
		}
		res.push_back(value);
	}
	return res;
}
/**Inverts a given Matrix.
* @param matrix The matrix to be inverted.
* @return The inverted matrix.**/
Matrix* Matrix::invert(Matrix* matrix)
{
	//This method used the Gauﬂ-Jordan-Algorithm. Thereby the given matrix gets converted to an identiy matrix. Each step in this process is also done on an identity matrix which at the end of the process matches the inverse matrix.

	if (matrix->getRowCount() != matrix->getColumnCount())
	{
		std::cerr << "Matrix has to be square" << std::endl;
		return NULL;
	}

	Matrix* m = new Matrix(matrix);
	Matrix* identity = new Matrix(m->getRowCount());
	double factor;

	//Convert matrix to an upper triangular matrix with Gauﬂ elimination.
	for (int i = 0; i < m->getRowCount(); i++)
	{
		//pivot index finden
		double absMax = 0;
		double absMaxIndex = 0;
		for (int j = i; j < m->getRowCount(); j++)
		{
			if (abs(m->getValue(j, i)) > absMax)
			{
				absMax = abs(m->getValue(j, i));
				absMaxIndex = j;
			}
		}
		m->swapRows(i, absMaxIndex);
		identity->swapRows(i, absMaxIndex);

		for (int j = i + 1; j < m->getRowCount(); j++)
		{
			factor = m->getValue(j, i) / m->getValue(i, i);
			m->rowAdd(i, j, -factor);
			identity->rowAdd(i, j, -factor);
		}
	}
	//Check if one of the diagonal elements is zero because matrix can not be inverted then.
	for (int i = 0; i < m->getRowCount(); i++)
	{
		if (m->getValue(i, i) == 0)//1e-10)
		{
			std::cout << "Matrix can not be inverted" << std::endl;
			return NULL;
		}
	}

	//Convert matrix to a diagonal matrix with Gauﬂ elimination.
	for (int i = m->getRowCount() - 1; i >= 0; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			factor = m->getValue(j, i) / m->getValue(i, i);
			m->rowAdd(i, j, -factor);
			identity->rowAdd(i, j, -factor);
		}
	}

	//Convert matrix to identiy matrix by normalizing each row/diagonal element
	for (int i = 0; i < m->getRowCount(); i++)
	{
		factor = 1 / m->getValue(i, i);
		m->rowScale(i, factor);
		identity->rowScale(i, factor);
	}
	//identity=inverse now
	return identity;
}
/** Converts matrix coordinates to index of the internally used vector.
* @param row the row index.
* @param column the column index.
* @return The index in the vector object. **/
int Matrix::convertCoordinatestoIndex(int row, int column)
{
	return row * columns + column;
}
/** Checks if given matrix coordinates are valid for the matrix.
* @param row the row index.
* @param column the column index.
* @return True if coordinates are valid and false when not. **/
bool Matrix::checkCoordiantes(int row, int column)
{
	if (row > rows - 1 || row < 0)
	{
		std::cerr << "Row index out of bounds" << std::endl;
		return false;
	}
	if (column > columns - 1 || column < 0)
	{
		std::cerr << "Column index out of bounds" << std::endl;
		return false;
	}
	return true;
}
/** Swap two rows of the matrix.
* @param row1 first row to swap.
* @param row2 second row to swap.
**/
void Matrix::swapRows(int row1, int row2)
{
	if (row1 == row2)
	{
		return;
	}
	double temp = 0;
	for (int i = 0; i < columns; i++)
	{
		temp = getValue(row2, i);
		setValue(row2, i, getValue(row1, i));
		setValue(row1, i, temp);
	}
}
/** Add one row to a other row of the matrix.
* @param row1 the row which is added.
* @param row2 the row on which factor*row1 is added.
* @param factor a factor by which row1 is multiplied before addition.
**/
void Matrix::rowAdd(int row1, int row2, double factor)
{
	if (factor == 0)
	{
		return;
	}
	for (int i = 0; i < columns; i++)
	{
		addValue(row2, i, factor * getValue(row1, i));
	}
}
/** Scale a row by a factor.
* @param row the row.
* @param factor the factor by witch the row is scaled.
**/
void Matrix::rowScale(int row, double factor)
{
	if (factor == 1)
	{
		return;
	}
	for (int i = 0; i < columns; i++)
	{
		setValue(row, i, getValue(row, i) * factor);
	}
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
	std::string matrixString;
	matrixString = "Matrix: \n";
	for (int i = 0; i < matrix.rows; i++)
	{
		matrixString += "[";
		for (int j = 0; j < matrix.columns; j++)
		{
			int index = i * matrix.columns + j;
			matrixString += std::to_string(matrix.m[index]) + ",";
		}
		matrixString.pop_back();
		matrixString += "]\n";
	}
	matrixString.pop_back();
	return os << matrixString;
}