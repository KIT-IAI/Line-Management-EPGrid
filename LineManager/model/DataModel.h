#pragma once
#include <set>
#include "gridElements/Node.h"
#include "gridElements/Line.h"
#include "../math/Matrix.h"
#include "packets/Setpoint.h"

/** Class which stores all data of a LineManager. **/
class DataModel
{
public:
	enum class States { STEADY, RAMP, DYNAMIC };
	DataModel();
	~DataModel();
	void addNode(Node* v);
	Node* getNode(std::string id);
	bool nodeExist(std::string id);
	std::vector <Node*> getNodes();
	void addLine(Line* l);
	Line* getLine(Node* v1, Node* v2);
	Line* getLine(std::string v1id, std::string v2id);
	bool lineExist(Node* v1, Node* v2);
	bool lineExist(std::string v1id, std::string v2id);
	std::vector <Line*> getLines();
	void addTimeslot(int timeslot);
	std::set <int> getTimeslots();
	bool timeslotExist(int timeslot);
	void removeTimeslot(int timeslot);
	void addGridState(int timeslot, States state);
	bool gridStateExist(int timeslot);
	States getGridState(int timeslot);
	std::map<int, States> getGridStates();
	bool isLocked();
	void setLock(bool state);
	std::string getNodesStringStatic();
	std::string getLinesStringStatic();
	std::string getNodesStringDynamic(std::set<int> times);
	std::string getLinesStringDynamic(std::set<int> times);
	std::vector<double> getNodeVoltages(int timeslot);
	std::vector<double> getPowerSetpoints(int timeslot);
	std::vector<double> getCurrentSetpoints(int timeslot);
	int getNodeIndex(std::string vid);
	int getNodeIndex(Node* v);
	int getIndexedNodeSize();
	Matrix* getAdmittanceMatrix();
	double getPowerLoss(int timeslot);

private:
	std::vector <Line*> lines;
	std::vector <Node*> nodes;
	std::set <int> timeslots;
	std::map<int, States> gridStates;
	std::map<std::string, int> nodeIndexMap;
	Matrix* admittanceMatrix;
	bool locked;
	int getPreviousExistingTimeslot(int timeslot);
	void generateNodeIndexMap();
	void generateAdmittanceMatrix();
	template<typename Object>
	std::string propertyToString(std::vector<Object*> objects, std::string(Object::* func)(void));
	template<typename Object>
	std::string propertyToString(std::vector<Object*> objects, std::string(Object::* func)(int), int timeslot);

};