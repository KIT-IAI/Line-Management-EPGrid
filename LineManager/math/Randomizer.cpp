#include "Randomizer.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

/** Inits the seed for randomization. **/
void Randomizer::initSeed()
{
	//std::cout << "seedSet: " << seedSet << std::endl;
	if (!Randomizer::seedSet)
	{
		Randomizer::seed = (int)time(NULL);
		srand(Randomizer::seed);
		Randomizer::seedSet = true;
	}
	//std::cout << "seed: " << seed << std::endl;
}
/** Renews the seed for randomization. This sets a new seed. **/
void Randomizer::renewSeed()
{
	Randomizer::seedSet = false;
	initSeed();
}
/** Resets the seed for randomization to its initial value. This can be used to have the same randomized parameters for next calls.**/
void Randomizer::resetRandomization()
{
	initSeed();
	srand(Randomizer::seed);
}
/** Returns a random int.
@param min minimum value of the random int.
@param max maximum value of the random int.**/
int Randomizer::getRandomInt(int min, int max)
{
	initSeed();
	return rand() % (max - min + 1) + min;
}
