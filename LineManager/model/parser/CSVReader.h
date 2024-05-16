#pragma once
#include <string>
#include <vector>
#include <fstream>

/** Class for reading csv files. **/
class CSVReader
{
public:
	CSVReader(std::string path,char separator);
	~CSVReader();
	void nextLine();
	std::string getLine();
	std::vector<std::string> getLineSplit();

	bool isEnd();
private:
	char separator;
	std::ifstream file;
	std::string line;
};
