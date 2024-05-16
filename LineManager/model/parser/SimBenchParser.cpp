#include"SimBenchParser.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "CSVReader.h"
//10.1109 / ISGTEurope.2019.8905726
/** Parses a model of the SimBench paper (www.simbench.net) and adds nodes and lines to an LineManager object.
* @param lm the LineManager where the model is added to.
* @param path the directory of the model. **/
void SimBenchParser::parseModel(LineManager* lm, std::string path)
{
	parsePart(path + "/Line.csv", lm, SimBenchParser::parseLine);
	parsePart(path + "/Load.csv", lm, SimBenchParser::parseLoad);
	parsePart(path + "/Transformer.csv", lm, SimBenchParser::parseTransformer);
}
/** Parses a individual file of SimBench modles and adds its components to an LineManager object.
* @param path the directory of the model.
* @param lm the LineManager where the model part is added to.
* @param func a function parsing and adding a specific components string to an LineManager object.**/
void SimBenchParser::parsePart(std::string path, LineManager* lm, void(*func)(LineManager* lm, std::vector<std::string>))
{
	CSVReader* reader = new CSVReader(path, ';');
	reader->nextLine();
	reader->nextLine();

	while (!reader->isEnd())
	{
		std::vector<std::string> res = reader->getLineSplit();
		(*func)(lm, res);
		reader->nextLine();
	}
	delete reader;
}
/** Parses the string vector representation of a line and adds it to an LineManager object
* @param lm the LineManager where the line is added to.
* @param lineString the string vector representation of a line.**/
void SimBenchParser::parseLine(LineManager* lm, std::vector<std::string> lineString)
{
	std::string v1id = parseNode(lineString[1]);
	std::string v2id = parseNode(lineString[2]);
	double length = std::stod(lineString[4]) * 1000;
	if (!lm->getDataModelReference()->nodeExist(v1id))
	{
		lm->addCoupler(v1id);
	}
	if (!lm->getDataModelReference()->nodeExist(v2id))
	{
		lm->addCoupler(v2id);
	}
	lm->addLine(v1id, v2id, length);
}
/** Parses the string vector representation of a Load and adds it to an LineManager object
* @param lm the LineManager where the line is added to.
* @param loadString the string vector representation of a load.**/
void SimBenchParser::parseLoad(LineManager* lm, std::vector<std::string> loadString)
{
	std::string v1id = parseNode(loadString[0]);
	std::string v2id = parseNode(loadString[1]);
	lm->addCurrentController(v1id);
	lm->addLine(v1id, v2id, 10);
}
/** Parses the string vector representation of a transformer and adds it to an LineManager object
* @param lm the LineManager where the line is added to.
* @param transformerString the string vector representation of a transformer.**/
void SimBenchParser::parseTransformer(LineManager* lm, std::vector<std::string> transformerString)
{
	std::string v1id = parseNode(transformerString[0]);
	std::string v2id = parseNode(transformerString[2]);
	lm->addVoltageController(v1id, 230);
	lm->addLine(v1id, v2id, 10);
}

/** Converts a node string to an id that can be used by a LineManager. Mainly replaces spaces and enforces lower case.
* @param nodeString the nodeString.
* @retun A string suitable as node id.**/
std::string SimBenchParser::parseNode(std::string nodeString)
{
	std::string vid = nodeString;
	std::replace(vid.begin(), vid.end(), ' ', '_');
	transform(vid.begin(), vid.end(), vid.begin(), ::tolower);
	return vid;
}
