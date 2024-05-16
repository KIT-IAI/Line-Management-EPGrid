#pragma once

#include <vector>
#include <string>

/** Class which provides some methods in context of std::vector. Some of the methods are wrappers. **/
class VectorTools
{
public:
	static double norm(std::vector<double> v);
	static void print(std::string name, std::vector<double> v);
	static void print(std::string name, std::vector<double> v, int precision);
	static std::vector<double> add(std::vector<double> a, std::vector<double> b);
	static void add(std::vector<double> a, std::vector<double> b, std::vector<double>* res);
	static std::vector<double> subtract(std::vector<double> a, std::vector<double> b);
	static void subtract(std::vector<double> *a, std::vector<double> *b, std::vector<double> *res);
	static std::vector<double> negate(std::vector<double> v);
	static bool equal(std::vector<double> a, std::vector<double> b);
private:
};

