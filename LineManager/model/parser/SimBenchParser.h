#pragma once
#include <string>
#include "../../control/LineManager.h"



/** Class for parsing models of the SimBench paper (www.simbench.net , DOI: 10.1109 / ISGTEurope.2019.8905726).**/
class SimBenchParser
{
public:
	static void parseModel(LineManager* lm, std::string path);
	static void parsePart(std::string path, LineManager* lm, void(func)(LineManager* lm, std::vector<std::string>));
	static void parseLine(LineManager* lm, std::vector<std::string> lineString);
	static void parseLoad(LineManager* lm, std::vector<std::string> loadString);
	static void parseTransformer(LineManager* lm, std::vector<std::string> transformerString);
private:
	static std::string parseNode(std::string nodeString);

};

