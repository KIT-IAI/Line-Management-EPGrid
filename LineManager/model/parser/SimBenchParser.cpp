#include"SimBenchParser.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "CSVReader.h"
#include "../GridModelHelper.h"

//10.1109 / ISGTEurope.2019.8905726
/** Parses a model of the SimBench paper (www.simbench.net) and adds nodes and lines to an LineManager object.
* @param path path the directory of the model. 
* @return the GridModel represeting the given SimBench model grid. **/
GridModel* SimBenchParser::parseModel(std::string path)
{
	GridModel* model = new GridModel();
	parsePart(path + "/Line.csv", model, SimBenchParser::parseLine);
	parsePart(path + "/Load.csv", model, SimBenchParser::parseLoad);
	parsePart(path + "/Transformer.csv", model, SimBenchParser::parseTransformer);
	return model;
}

/** Parses a individual file of SimBench modles and adds its components to an LineManager object.
* @param path the directory of the model.
* @param lm the LineManager where the model part is added to.
* @param func a function parsing and adding a specific components string to an LineManager object.**/
void SimBenchParser::parsePart(std::string path, GridModel* model, void(*func)(GridModel*, std::vector<std::string>))
{
	CSVReader* reader = new CSVReader(path, ';');
	reader->nextLine();
	reader->nextLine();

	while (!reader->isEnd())
	{
		std::vector<std::string> res = reader->getLineSplit();
		(*func)(model, res);
		reader->nextLine();
	}
	delete reader;
}
/** Parses the string vector representation of a line and adds it to an LineManager object
* @param lm the LineManager where the line is added to.
* @param lineString the string vector representation of a line.**/
void SimBenchParser::parseLine(GridModel* model, std::vector<std::string> lineString)
{
	std::string v1id = parseNode(lineString[1]);
	std::string v2id = parseNode(lineString[2]);
	double length = std::stod(lineString[4]) * 1000;
	GridModelHelper::addCoupler(model, v1id);
	GridModelHelper::addCoupler(model, v2id);
	GridModelHelper::addLine(model, v1id, v2id, length);
}
/** Parses the string vector representation of a Load and adds it to an LineManager object
* @param lm the LineManager where the line is added to.
* @param loadString the string vector representation of a load.**/
void SimBenchParser::parseLoad(GridModel* model, std::vector<std::string> loadString)
{
	std::string v1id = parseNode(loadString[0]);
	std::string v2id = parseNode(loadString[1]);
	GridModelHelper::addCurrentSource(model,v1id);
	GridModelHelper::addLine(model,v1id, v2id, 10);
}
/** Parses the string vector representation of a transformer and adds it to an LineManager object
* @param lm the LineManager where the line is added to.
* @param transformerString the string vector representation of a transformer.**/
void SimBenchParser::parseTransformer(GridModel* model, std::vector<std::string> transformerString)
{
	std::string v1id = parseNode(transformerString[0]);
	std::string v2id = parseNode(transformerString[2]);
	GridModelHelper::addVoltageSource(model,v1id, 230);
	GridModelHelper::addLine(model,v1id, v2id, 10);
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
