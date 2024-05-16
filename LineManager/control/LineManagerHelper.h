#pragma once
#include "LineManager.h"


static bool seedSet = false;
static int seed;

/** Static Class which provides methods to initialize a LineManager and emulate EP requests. **/
class LineManagerHelper
{
public:
	static void setupGridNetwork(LineManager* lm, int participantCount);
	static void setupPartlyRandomizedNetwork(LineManager* lm, int participantCount);
	static void setupReferenceGrid(LineManager* lm);
	static EP* getRandomPowerRectangular(LineManager* lm, int minBegin, int maxBegin, int minPower, int maxPower, int minDuration, int maxDuration);
	static EP* getRandomPowerTrapeze(LineManager* lm, int minBegin, int maxBegin, int minPower, int maxPower, int minRampPower, int maxRampPower, int minEnergy, int maxEnergy);
	static EP* getRandomCurrentRectangular(LineManager* lm, int minBegin, int maxBegin, int minCurrent, int maxCurrent, int minDuration, int maxDuration);
	static EP* getRandomCurrentTrapeze(LineManager* lm, int minBegin, int maxBegin, int minCurrent, int maxCurrent, int minRampCurrent, int maxRampCurrent, int minDuration, int maxDuration);
	static void initSeed();
	static void renewSeed();
	static void resetRandomization();

private:
	static std::pair<std::string, std::string> getRandomNodePair(LineManager* lm);
	static int getRandomInt(int min, int max);
	static double getRandomDouble(double min, double max);
};
