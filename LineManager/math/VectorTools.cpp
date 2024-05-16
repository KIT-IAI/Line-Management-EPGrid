#include "VectorTools.h"
#include <iostream>
#include <algorithm>
#include <functional>
/** Calculates the norm of a given vector.
* @param v the vector.
* @param the norm of the vector. **/
double VectorTools::norm(std::vector<double> v)
{
	double sum = 0;
	for (int i = 0; i < v.size(); i++)
	{
		sum += v[i] * v[i];
	}
	return sqrt(sum);
}

/** Prints a given vector to console.
* @param name the name of the vector.
* @param v the vector.**/
void VectorTools::print(std::string name, std::vector<double> v)
{
	std::cout << name << ": " << std::endl;
	std::cout << "[";
	for (int i = 0; i < v.size(); i++) {
		std::cout << v[i] << " ";
	}
	std::cout << "]" << std::endl;
}

/** Prints a given vector to console.
* @param name the name of the vector.
* @param v the vector.
* @param precision the precision of the output.**/
void VectorTools::print(std::string name, std::vector<double> v, int precision)
{
	int prevPrecision = std::cout.precision();
	//std::cout << std::fixed;
	std::cout.precision(precision);
	print(name, v);
	std::cout.precision(prevPrecision);
}

/** Adds two vectors.
* @param a the first vector.
* @param b the second vector.
* @return The result vector. **/
std::vector<double> VectorTools::add(std::vector<double> a, std::vector<double> b)
{
	if (a.size() != b.size())
	{
		return std::vector<double>();
	}
	std::vector<double> res(a.size(), NULL);
	std::transform(a.begin(), a.end(), b.begin(), res.begin(), std::plus<double>());
	return res;
}
/** Adds two vectors and stores the result in a given vector.
* @param a the first vector.
* @param b the second vector.
* @param res the vector in which the result has to be stored. **/
void VectorTools::add(std::vector<double> a, std::vector<double> b, std::vector<double>* res)
{
	if (a.size() != b.size() || a.size() != (*res).size())
	{
		return;
	}

	std::transform(a.begin(), a.end(), b.begin(), (*res).begin(), std::plus<double>());
}
/** Subtracts two vectors.
* @param a the first vector.
* @param b the second vector.
* @return The resulting vector. **/
std::vector<double> VectorTools::subtract(std::vector<double> a, std::vector<double> b)
{
	if (a.size() != b.size())
	{
		return std::vector<double>();
	}
	std::vector<double> res(a.size(), NULL);
	std::transform(a.begin(), a.end(), b.begin(), res.begin(), std::minus<double>());
	return res;
}

/** Subtracts two vectors and stores the result in a given vector.
* @param a the first vector.
* @param b the second vector.
* @param res the vector in which the result has to be stored. **/
void VectorTools::subtract(std::vector<double>* a, std::vector<double>* b, std::vector<double>* res)
{
	if ((*a).size() != (*b).size() || (*a).size() != (*res).size())
	{
		return;
	}
	std::transform((*a).begin(), (*a).end(), (*b).begin(), (*res).begin(), std::minus<double>());
}

/** Negates a given vector.
* @param v the vector which gets negated.**/
std::vector<double> VectorTools::negate(std::vector<double> v)
{
	std::transform(v.begin(), v.end(), v.begin(), std::negate<double>());
	return v;
}

/** Checks if two specified vectors are equal.
* @param a a double vector
* @param b a second double vector
* @return True if the vectors are equal and false when not.**/
bool VectorTools::equal(std::vector<double> a, std::vector<double> b)
{
	if (a.size() != b.size())
	{
		return false;
	}
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}



