#pragma once
#include "GridModel.h"

/** Static Class which provides methods to generate a predefined or randomizeda GridModel. **/
class GridModelGenerator
{
public:
	static GridModel* setupGridNetwork(int participantCount);
	static GridModel* setupPartlyRandomizedNetwork(int participantCount);
	static GridModel* setupReferenceGrid();

private:
};
