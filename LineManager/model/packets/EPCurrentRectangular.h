#pragma once
#include "EP.h"

/** Class for describing an energy packet with a rectangular current shape. **/
class EPCurrentRectangular : public EP
{
public:
	EPCurrentRectangular(std::string v1id, std::string v2id, double power, int begin, int duration);
private:
};
