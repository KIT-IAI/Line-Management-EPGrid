#pragma once
#include <iostream>
#include <vector>
/** Represents a matrix and offers methods for editing and calcualting with matrices.**/
class Matrix
{
public:
	Matrix(int rows, int columns);
	Matrix(int dimension);
	Matrix(Matrix* a);
	double getValue(int row, int column);
	void setValue(int row, int column, double value);
	void addValue(int row, int column, double value);
	int getRowCount();
	int getColumnCount();
	void swapRows(int row1, int row2);
	void rowAdd(int row1, int row2, double factor);
	void rowScale(int row1, double factor);
	static Matrix* multiply(Matrix* m, Matrix* n);
	static std::vector<double> multiply(Matrix* m, std::vector<double> v);
	static Matrix* invert(Matrix* m);
	friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
private:
	std::vector<double> m;
	int rows, columns;
	int convertCoordinatestoIndex(int row, int column);
	bool checkCoordiantes(int row, int column);

};

