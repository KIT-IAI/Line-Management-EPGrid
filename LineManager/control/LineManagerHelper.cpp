#include "LineManagerHelper.h"
#include "../Config.h"
#include "../model/packets/EPPowerRectangular.h"
#include "../model/packets/EPPowerTrapeze.h"
#include "../model/packets/EPCurrentRectangular.h"
#include "../model/packets/EPCurrentTrapeze.h"

/** Creates a electrical grid which can be displayed in grid layout. Each Coupler is connected to exactly one controller and two lines (except the two Coupler Objects at the grid border). The length of the lines gets equally distributed between the couplers and overall exactly adds up to 1 km.
* @param lm The LineManager object to which the nodes and lines are added.
* @param participantCount The number of participants the grid shall have. This includes one VoltageController. **/
void LineManagerHelper::setupGridNetwork(LineManager* lm, int participantCount)
{
	lm->addVoltageController(0, VOLTAGE);
	lm->addCoupler(1);
	lm->addLine(0, 1, 20);
	for (int i = 1; i < participantCount; i++)
	{
		lm->addCurrentController(i * 2);
		lm->addCoupler(i * 2 + 1);

		lm->addLine(i * 2, i * 2 + 1, 20);//Controller to Coupler
		lm->addLine((i - 1) * 2 + 1, i * 2 + 1, 1000 / participantCount);//Coupler to previous Coupler
	}
}

/** Creates a electrical grid with randomized layout. Each Coupler is connected to muliple controllers and two lines (except the two Coupler Objects at the grid border). The length of the lines gets equally distributed between the couplers and overall exactly adds up to 1 km.
* @param lm The LineManager object to which the nodes and lines are added.
* @param participantCount The number of participants the grid shall have. This includes one VoltageController. **/
void LineManagerHelper::setupPartlyRandomizedNetwork(LineManager* lm, int participantCount)
{
	initSeed();

	lm->addVoltageController(0, VOLTAGE);
	lm->addCoupler(1);
	lm->addLine(0, 1, 20);
	int couplerCount = 1;
	int controllerCount = 1;
	for (int i = 1; i < participantCount; i++)
	{
		int controllerID = controllerCount * 2;
		controllerCount++;
		lm->addCurrentController(controllerID);

		if (rand() % 100 < std::min((float)50, (float)i / controllerCount * 100 * 4))
		{
			int couplerID = couplerCount * 2 - 1 - (rand() % std::min(4, couplerCount)) * 2;
			lm->addLine(couplerID, controllerID, 20);//Controller to existing Coupler
		}
		else
		{
			int couplerID = couplerCount * 2 + 1;
			couplerCount++;
			lm->addCoupler(couplerID);
			lm->addLine(couplerID - 2, couplerID, 1000 / controllerCount);//new Coupler to previous Coulper
			lm->addLine(couplerID, controllerID, 20);//Controller to new Coupler

		}
	}
}
/** Creates a reference grid similar to DOI:10.1007/978-3-8348-9761-9 Bild 3.7 Verzweigter Ring. **/
void LineManagerHelper::setupReferenceGrid(LineManager* lm)
{
	lm->addVoltageController(0, VOLTAGE);
	lm->addCoupler(1);
	lm->addLine(0, 1, 10);
	for (int i = 1; i <= 30; i++)
	{
		std::set<int> nonExistingCurrentControllers = { 4,8,36,40,48 };
		lm->addCoupler(i * 2 + 1);
		if (nonExistingCurrentControllers.count(i * 2) == 0)
		{
			lm->addCurrentController(i * 2);
			lm->addLine(i * 2, i * 2 + 1, 20);
		}
	}
	lm->addLine(1, 3, 20);
	lm->addLine(3, 5, 20);
	lm->addLine(5, 7, 20);
	lm->addLine(7, 9, 20);
	lm->addLine(9, 11, 100);
	lm->addLine(5, 13, 80);
	lm->addLine(13, 15, 20);
	lm->addLine(15, 17, 30);
	lm->addLine(17, 19, 20);

	lm->addLine(1, 21, 20);
	lm->addLine(21, 23, 20);
	lm->addLine(23, 25, 20);
	lm->addLine(25, 27, 20);
	lm->addLine(27, 29, 20);

	lm->addLine(1, 31, 40);
	lm->addLine(31, 33, 20);
	lm->addLine(33, 35, 20);
	lm->addLine(35, 37, 20);
	lm->addLine(37, 39, 30);
	lm->addLine(39, 41, 20);
	lm->addLine(41, 43, 20);

	lm->addLine(1, 45, 40);
	lm->addLine(45, 47, 20);
	lm->addLine(47, 49, 30);
	lm->addLine(49, 51, 20);
	lm->addLine(51, 53, 20);
	lm->addLine(49, 55, 50);
	lm->addLine(55, 57, 20);
	lm->addLine(57, 59, 20);
	lm->addLine(59, 61, 80);

	//lm->addLine(41, 61, 80);//möglicher Ringschluss
}
/** Generates a random EPPowerRectangular object.
* @param lm a LineManager object to retrieve grid information.
* @param minBegin the earliest beginning time of the EP.
* @param maxBegin the latest beginning time of the EP.
* @param minPower the minimum power of the EP.
* @param maxPower the maximum power of the EP.
* @param minDuration the minimum duration of the EP.
* @param maxDuration the maximum duration of the EP.
* return the generated EPPowerRectangular.
**/
EP* LineManagerHelper::getRandomPowerRectangular(LineManager* lm, int minBegin, int maxBegin, int minPower, int maxPower, int minDuration, int maxDuration)
{
	initSeed();
	std::pair<std::string, std::string> nodeIDs = getRandomNodePair(lm);
	int begin = getRandomInt(minBegin, maxBegin);
	int duration = getRandomInt(minDuration, maxDuration);
	double power = getRandomInt(minPower, maxPower);

	//std::cout << "Begin: " << begin << ", duration: " << duration << ", power: " << power << std::endl;
	return new EPPowerRectangular(nodeIDs.first, nodeIDs.second, power, begin, duration);
}
/** Generates a random EPPowerTrapeze object.
* @param lm a LineManager object to retrieve grid information.
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
EP* LineManagerHelper::getRandomPowerTrapeze(LineManager* lm, int minBegin, int maxBegin, int minPower, int maxPower, int minRampPower, int maxRampPower, int minEnergy, int maxEnergy)
{
	initSeed();
	std::pair<std::string, std::string> nodeIDs = getRandomNodePair(lm);

	int begin;
	double energy;
	double power;
	double rampPower;
	EP* ep=NULL;
	bool success = false;
	while (!success)
	{
		begin = getRandomInt(minBegin, maxBegin);
		energy = getRandomInt(minEnergy, maxEnergy);
		power = getRandomInt(minPower, maxPower);
		rampPower = getRandomInt(minRampPower, maxRampPower);
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
* @param lm a LineManager object to retrieve grid information.
* @param minBegin the earliest beginning time of the EP.
* @param maxBegin the latest beginning time of the EP.
* @param minCurrent the minimum current of the EP.
* @param maxCurrent the maximum current of the EP.
* @param minDuration the minimum duration of the EP.
* @param maxDuration the maximum duration of the EP.
* return the generated EPCurrentRectangular.
**/
EP* LineManagerHelper::getRandomCurrentRectangular(LineManager* lm, int minBegin, int maxBegin, int minCurrent, int maxCurrent, int minDuration, int maxDuration) 
{
	initSeed();
	std::pair<std::string, std::string> nodeIDs = getRandomNodePair(lm);
	int begin = getRandomInt(minBegin, maxBegin);
	int duration = getRandomInt(minDuration, maxDuration);
	double current = getRandomInt(minCurrent, maxCurrent);

	//std::cout << "Begin: " << begin << ", duration: " << duration << ", current: " << current << std::endl;
	return new EPCurrentRectangular(nodeIDs.first, nodeIDs.second, current, begin, duration);
}
/** Generates a random EPCurrentTrapeze object.
* @param lm a LineManager object to retrieve grid information.
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
EP* LineManagerHelper::getRandomCurrentTrapeze(LineManager* lm, int minBegin, int maxBegin, int minCurrent, int maxCurrent, int minRampCurrent, int maxRampCurrent, int minDuration, int maxDuration) 
{
	initSeed();
	std::pair<std::string, std::string> nodeIDs = getRandomNodePair(lm);

	int begin;
	double duration;
	double current;
	double rampCurrent;
	EP* ep=NULL;
	bool success = false;
	while (!success)
	{
		begin = getRandomInt(minBegin, maxBegin);
		duration = getRandomInt(minDuration, maxDuration);
		current = getRandomInt(minCurrent, maxCurrent);
		rampCurrent = getRandomInt(minRampCurrent, maxRampCurrent);
		try {
			ep = new EPCurrentTrapeze(nodeIDs.first, nodeIDs.second, current, rampCurrent, begin, duration);
			success = true;
		}
		catch (std::exception e) {}
	}
	std::cout << "Begin: " << begin << " Supplier: " <<nodeIDs.first<<" Absorber: "<<nodeIDs.second << ", duration: " << duration << ", current: " << current << ", rampCurrent: " << rampCurrent << std::endl;
	return ep;
}

/** Inits the seed for randomization. **/
void LineManagerHelper::initSeed()
{
	if (!seedSet)
	{
		seed = time(NULL);
		srand(seed);
		seedSet = true;
	}
}

/** Renews the seed for randomization. This sets a new seed. **/
void LineManagerHelper::renewSeed()
{
	seedSet = false;
	initSeed();
}

/** Resets the seed for randomization to its initial value. This can be used to have the same randomized parameters for another LineManager instance.**/
void LineManagerHelper::resetRandomization()
{
	initSeed();
	srand(seed);
}

/** Return a random pair of nodes of an Line managers grid.
@return a randomized pair of node IDs.**/
std::pair<std::string, std::string> LineManagerHelper::getRandomNodePair(LineManager* lm)
{
	std::vector<std::string> nodeIDs;
	for (Node* v : lm->getDataModelReference()->getNodes())
	{
		if (v->getType() != Node::Types::COUPLER)
		{
			nodeIDs.push_back(v->getID());
		}
	}
	int participants = nodeIDs.size();
	std::string v1id;
	std::string v2id;
	v1id = nodeIDs[rand() % participants];
	bool v1IsVoltageController = lm->getDataModelReference()->getNode(v1id)->getType() == Node::Types::VOLTAGE_CONTROLLER;
	v2id = v1id;
	while (v2id == v1id || (v1IsVoltageController && lm->getDataModelReference()->getNode(v2id)->getType() == Node::Types::VOLTAGE_CONTROLLER))
	{
		v2id = nodeIDs[rand() % participants];
	}
	return { v1id,v2id };
}

/** Returns a random int.
@param min minimum value of the random int.
@param max maximum value of the random int.**/
int LineManagerHelper::getRandomInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}