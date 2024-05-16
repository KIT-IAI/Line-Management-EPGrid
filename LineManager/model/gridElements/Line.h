 #pragma once
#include <iostream>
#include <vector>
#include "CableTypes.h"
#include <map>
#include <set>
class Node;

/** Class representing a line of the grid. **/
class Line
{
public:
	Line(Node* v1, Node* v2, CableTypes::Types t, double length);
	Line(Node* v1, Node* v2, double length);
	//Line(Node v1, Node v2,double resistance,int limit);
	std::string getID();
	std::pair<Node*, Node*> getNodes();
	std::pair<std::string, std::string> getNodeIDs();
	double getResistance();
	int getLimit();
	double getVoltage(int timeslot);
	double getCurrent(int timeslot);
	double getCurrentRelative(int timeslot, Node* v);
	double getPower(int timeslot);
	std::string serialize();
	std::string serializeState(int timeslot);
	Node* getPartnerNode(Node* v);

	friend std::ostream& operator<<(std::ostream& os, const Line& l);
private:
	Node* v1, * v2;
	double resistance;
	int limit;
};

