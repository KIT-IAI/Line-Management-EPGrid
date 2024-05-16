#pragma once
#include "GridModel.h"

/** Static Class which provides methods to initialize a GridModel. **/
class GridModelHelper
{
public:
	static bool addVoltageSource(GridModel* model,std::string id, double voltage);
	static bool addVoltageSource(GridModel* model, int id, double voltage);
	static bool addCurrentSource(GridModel* model, std::string id);
	static bool addCurrentSource(GridModel* model, int id);
	static bool addCoupler(GridModel* model, std::string id);
	static bool addCoupler(GridModel* model, int id);
	static bool addLine(GridModel* model, std::string v1id, std::string v2id, double length);
	static bool addLine(GridModel* model, int v1id, int v2id, double length);

private:
	static bool checkNodeAdding(GridModel* model,std::string id);
};
