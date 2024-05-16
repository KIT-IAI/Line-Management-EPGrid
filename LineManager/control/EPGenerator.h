#pragma once
#include "../model/GridModel.h"
#include "../model/packets/EP.h"


static bool seedSet = false;
static int seed;

/** Static Class which provides methods to generate randomized EPs for a given grid model. **/
class EPGenerator
{
public:
	static EP* getRandomPowerRectangular(GridModel* model, int minBegin, int maxBegin, int minPower, int maxPower, int minDuration, int maxDuration);
	static EP* getRandomPowerTrapeze(GridModel* model, int minBegin, int maxBegin, int minPower, int maxPower, int minRampPower, int maxRampPower, int minEnergy, int maxEnergy);
	static EP* getRandomCurrentRectangular(GridModel* model, int minBegin, int maxBegin, int minCurrent, int maxCurrent, int minDuration, int maxDuration);
	static EP* getRandomCurrentTrapeze(GridModel* model, int minBegin, int maxBegin, int minCurrent, int maxCurrent, int minRampCurrent, int maxRampCurrent, int minDuration, int maxDuration);
private:
	static std::pair<std::string, std::string> getRandomNodePair(GridModel* model);


};
