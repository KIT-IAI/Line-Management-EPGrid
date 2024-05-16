#pragma once
#include <string>
#include "../../model/GridModel.h"



/** Class for parsing models of the SimBench paper (www.simbench.net , DOI: 10.1109 / ISGTEurope.2019.8905726).**/
class SimBenchParser
{
public:
	static GridModel* parseModel(std::string path);
private:
	static void parsePart(std::string path, GridModel* model, void(func)(GridModel*, std::vector<std::string>));
	static void parseLine(GridModel* model, std::vector<std::string> lineString);
	static void parseLoad(GridModel* model, std::vector<std::string> loadString);
	static void parseTransformer(GridModel* model, std::vector<std::string> transformerString);
	static std::string parseNode(std::string nodeString);

};

