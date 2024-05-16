#pragma once
#include <iostream>
#include<list>
#include <vector>
#include <map>
#include <set>
class Line;

/** Generic class for a node in the grid **/
class Node
{
public:
	enum class Types { VOLTAGE_CONTROLLER, CURRENT_CONTROLLER, COUPLER, INVALID };
	Node(std::string id);
	std::string getID();
	virtual Types getType() = 0;
	void addLine(Line* l);
	std::vector<Line*> getLines();
	virtual std::string serializeStatic();
	virtual std::string serializeDynamic(int timeslot);
	double getVoltage(int timeslot);
	void setVoltage(double voltage, int timeslot, bool force);
	double getCurrent(int timeslot);
	double getPower(int timeslot);
	void printTimeslots();
	std::vector<double> getTimeslots();
	friend std::ostream& operator<<(std::ostream& os, const Node& n);
protected:
	bool timeslotExist(int timeslot);
	double getTimeProperty(int timeslot, std::map<int, double>* propertyMap);
	void setTimeProperty(double value, int timeslot, std::map<int, double>* propertyMap);
	void addTimeProperty(double value, int timeslot, std::map<int, double>* propertyMap);
	void addTimeProperty(double value, int begin, int duration, std::map<int, double>* propertyMap);
	void removeTimeProperty(int timeslot, std::map<int, double>* propertyMap);
	void removeTimeProperty(double value, int timeslot, std::map<int, double>* propertyMap);
	void removeTimeProperty(double value, int begin, int duration, std::map<int, double>* propertyMap);
private:
	std::string id;
	std::vector <Line*> lines;
	std::map<int, double> voltages;
};

