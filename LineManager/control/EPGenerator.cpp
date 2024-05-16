#include "EPGenerator.h"
#include "../Config.h"
#include "../math/Randomizer.h"
#include "../model/packets/EPPowerRectangular.h"
#include "../model/packets/EPPowerTrapeze.h"
#include "../model/packets/EPCurrentRectangular.h"
#include "../model/packets/EPCurrentTrapeze.h"

/** Generates a random EPPowerRectangular object.
* @param model a GridModel object to retrieve grid information.
* @param minBegin the earliest beginning time of the EP.
* @param maxBegin the latest beginning time of the EP.
* @param minPower the minimum power of the EP.
* @param maxPower the maximum power of the EP.
* @param minDuration the minimum duration of the EP.
* @param maxDuration the maximum duration of the EP.
* return the generated EPPowerRectangular.
**/
EP* EPGenerator::getRandomPowerRectangular(GridModel* model, int minBegin, int maxBegin, int minPower, int maxPower, int minDuration, int maxDuration)
{
	std::pair<std::string, std::string> nodeIDs = getRandomNodePair(model);
	int begin = Randomizer::getRandomInt(minBegin, maxBegin);
	int duration = Randomizer::getRandomInt(minDuration, maxDuration);
	double power = Randomizer::getRandomInt(minPower, maxPower);

	//std::cout << "Begin: " << begin << ", duration: " << duration << ", power: " << power << std::endl;
	return new EPPowerRectangular(nodeIDs.first, nodeIDs.second, power, begin, duration);
}
/** Generates a random EPPowerTrapeze object.
* @param model a GridModel object to retrieve grid information.
* @param minBegin the earliest beginning time of the EP.
* @param maxBegin the latest beginning time of the EP.
* @param minPower the minimum power of the EP.
* @param maxPower the maximum power of the EP.
* @param minRampPower the minimum power per second during the ramp of the EP.
* @param maxRampPower the maximum power per second during the ramp of the EP.
* @param minEnergy the minimum energy of the EP.
* @param maxEnergy the maximum energy of the EP.
* return the generated EPPowerTrapeze.
**/
EP* EPGenerator::getRandomPowerTrapeze(GridModel* model, int minBegin, int maxBegin, int minPower, int maxPower, int minRampPower, int maxRampPower, int minEnergy, int maxEnergy)
{
	std::pair<std::string, std::string> nodeIDs = getRandomNodePair(model);

	int begin;
	double energy;
	double power;
	double rampPower;
	EP* ep=NULL;
	bool success = false;
	while (!success)
	{
		begin = Randomizer::getRandomInt(minBegin, maxBegin);
		energy = Randomizer::getRandomInt(minEnergy, maxEnergy);
		power = Randomizer::getRandomInt(minPower, maxPower);
		rampPower = Randomizer::getRandomInt(minRampPower, maxRampPower);
		try {
			ep = new EPPowerTrapeze(nodeIDs.first, nodeIDs.second, power, rampPower, begin, energy);
			success = true;
		}
		catch (std::exception e) {}
	}
	std::cout << "Begin: " << begin << " Supplier: " << nodeIDs.first << " Absorber: " << nodeIDs.second<< ", energy: " << energy << ", power: " << power << ", rampPower: " << rampPower << ", duration: "<< ep->getDuration()* S_PER_TIMESLOT << std::endl;
	return ep;
}

/** Generates a random EPCurrentRectangular object.
* @param model a GridModel object to retrieve grid information.
* @param minBegin the earliest beginning time of the EP.
* @param maxBegin the latest beginning time of the EP.
* @param minCurrent the minimum current of the EP.
* @param maxCurrent the maximum current of the EP.
* @param minDuration the minimum duration of the EP.
* @param maxDuration the maximum duration of the EP.
* return the generated EPCurrentRectangular.
**/
EP* EPGenerator::getRandomCurrentRectangular(GridModel* model, int minBegin, int maxBegin, int minCurrent, int maxCurrent, int minDuration, int maxDuration)
{
	std::pair<std::string, std::string> nodeIDs = getRandomNodePair(model);
	int begin = Randomizer::getRandomInt(minBegin, maxBegin);
	int duration = Randomizer::getRandomInt(minDuration, maxDuration);
	double current = Randomizer::getRandomInt(minCurrent, maxCurrent);

	//std::cout << "Begin: " << begin << ", duration: " << duration << ", current: " << current << std::endl;
	return new EPCurrentRectangular(nodeIDs.first, nodeIDs.second, current, begin, duration);
}
/** Generates a random EPCurrentTrapeze object.
* @param model a GridModel object to retrieve grid information.
* @param minBegin the earliest beginning time of the EP.
* @param maxBegin the latest beginning time of the EP.
* @param minPower the minimum current of the EP.
* @param maxPower the maximum current of the EP.
* @param minRampPower the minimum current per second during the ramp of the EP.
* @param maxRampPower the maximum current per second during the ramp of the EP.
* @param minDuration the minimum duration of the EP.
* @param maxDuration the maximum duration of the EP.
* return the generated EPCurrentTrapeze.
**/
EP* EPGenerator::getRandomCurrentTrapeze(GridModel* model, int minBegin, int maxBegin, int minCurrent, int maxCurrent, int minRampCurrent, int maxRampCurrent, int minDuration, int maxDuration)
{
	std::pair<std::string, std::string> nodeIDs = getRandomNodePair(model);

	int begin;
	double duration;
	double current;
	double rampCurrent;
	EP* ep=NULL;
	bool success = false;
	while (!success)
	{
		begin = Randomizer::getRandomInt(minBegin, maxBegin);
		duration = Randomizer::getRandomInt(minDuration, maxDuration);
		current = Randomizer::getRandomInt(minCurrent, maxCurrent);
		rampCurrent = Randomizer::getRandomInt(minRampCurrent, maxRampCurrent);
		try {
			ep = new EPCurrentTrapeze(nodeIDs.first, nodeIDs.second, current, rampCurrent, begin, duration);
			success = true;
		}
		catch (std::exception e) {}
	}
	std::cout << "Begin: " << begin << " Supplier: " <<nodeIDs.first<<" Absorber: "<<nodeIDs.second << ", duration: " << duration << ", current: " << current << ", rampCurrent: " << rampCurrent << std::endl;
	return ep;
}

/** Return a random pair of nodes of an Line managers grid.
@return a randomized pair of node IDs.**/
std::pair<std::string, std::string> EPGenerator::getRandomNodePair(GridModel* model)
{
	std::vector<std::string> nodeIDs;
	for (Node* v : model->getNodes())
	{
		if (v->getType() != Node::Types::COUPLER)
		{
			nodeIDs.push_back(v->getID());
		}
	}
	int participants = nodeIDs.size();
	std::string v1id;
	std::string v2id;
	v1id = nodeIDs[Randomizer::getRandomInt(0, participants-1)];
	bool v1IsVoltageController = model->getNode(v1id)->getType() == Node::Types::VOLTAGE_CONTROLLER;
	v2id = v1id;
	while (v2id == v1id || (v1IsVoltageController && model->getNode(v2id)->getType() == Node::Types::VOLTAGE_CONTROLLER))
	{
		v2id = nodeIDs[Randomizer::getRandomInt(0, participants - 1)];
	}
	return { v1id,v2id };
}
