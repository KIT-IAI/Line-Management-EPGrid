#pragma once
#include "EP.h"

/** Class for describing an energy packet with a rectangular power shape. **/
class EPPowerRectangular : public EP
{
public:
	EPPowerRectangular(std::string v1id, std::string v2id, double power, int begin, int duration);
	EPPowerRectangular(std::string v1id, std::string v2id, double power, int begin, double energy);
	double getPossibleEnergy();
private:
};
