#include"CSVReader.h"
#include <string>

#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <iostream>
/** Constructor of CSVReader
* @param path thepath to the file.
* @param separator the separtion character of the dataset. **/
CSVReader::CSVReader(std::string path, char separator)
{
	this->separator = separator;
	file.open(path);
	if (!file.is_open()) throw std::runtime_error("Could not open file.");
	if (!file.good()) throw std::runtime_error("Filestream not good.");
}
/** Destructor of CSVReader **/
CSVReader::~CSVReader()
{
	file.close();
}
/** Sets the virtual cursor to the next line of a file. (Realized by storing the new line in the private line variable of the class)**/
void CSVReader::nextLine()
{
	if (isEnd())
	{
		return;
	}
	std::getline(file, line);
}
/** Returns the line at the virtual cursors position.
* @return the line. **/
std::string CSVReader::getLine()
{
	return line;
}
/** Returns the actual line splitted by the separation character.
* @return a vector of the indiviudla parts of the splitted line.**/
std::vector<std::string> CSVReader::getLineSplit()
{
	std::vector<std::string> res;
	std::string colname;
	std::stringstream ss(line);

	while (std::getline(ss, colname, separator)) {
		res.push_back(colname);
	}
	return res;
}
/** Checks if the cursor is at the last line of the file.
* @return True when the cursor is in the last line.**/
bool CSVReader::isEnd()
{
	return file.eof();
}
