#include "GridModelHelper.h"
#include "gridElements/VoltageSource.h"
#include "gridElements/CurrentSource.h"
#include "gridElements/Coupler.h"
#include "gridElements/Line.h"

/** Adds a VoltageSource to the grid.
* @param model the GridModel where the source shall be added to.
* @param id the id of the source.
* @param voltage the voltage of the source.**/
bool GridModelHelper::addVoltageSource(GridModel* model, std::string id, double voltage)
{
	if (checkNodeAdding(model, id))
	{
		model->addNode(new VoltageSource(id, voltage));
		return true;
	}
	return false;
}
/** Adds a VoltageSource to the grid.
* @param model the GridModel where the source shall be added to.
* @param id the id of the source.
* @param voltage the voltage of the source.**/
bool GridModelHelper::addVoltageSource(GridModel* model, int id, double voltage)
{
	return addVoltageSource(model,std::to_string(id), voltage);
}

/** Adds a CurrentSource to the grid.
* @param model the GridModel where the source shall be added to.
* @param id the id of the source.**/
bool GridModelHelper::addCurrentSource(GridModel* model, std::string id)
{
	if (checkNodeAdding(model, id))
	{
		model->addNode(new CurrentSource(id));
		return true;
	}
	return false;
}
/** Adds a CurrentSource to the grid.
* @param model the GridModel where the source shall be added to.
* @param id the id of the source.**/
bool GridModelHelper::addCurrentSource(GridModel* model, int id)
{
	return addCurrentSource(model, std::to_string(id));
}
/** Adds a Coupler to the grid.
* @param model the GridModel where the source shall be added to.
* @param id the id of the coupler.**/
bool GridModelHelper::addCoupler(GridModel* model, std::string id)
{
	if (checkNodeAdding(model, id))
	{
		model->addNode(new Coupler(id));
		return true;
	}
	return false;
}
/** Adds a Coupler to the grid.
* @param model the GridModel where the source shall be added to.
* @param id the id of the coupler.**/
bool GridModelHelper::addCoupler(GridModel* model, int id)
{
	return addCoupler(model, std::to_string(id));
}

/** Adds a Line to the grid.
* @param model the GridModel where the Line shall be added to.
* @param v1id the id of the first Node.
* @param v2id the id of the second Node.
* length the length of the Line.**/
bool GridModelHelper::addLine(GridModel* model, std::string v1id, std::string v2id, double length)
{
	if (model->isLocked())
	{
		std::cerr << "Adding a Line to a locked model is not possible is not allowed" << std::endl;
		return false;
	}

	if (v1id == v2id)
	{
		std::cerr << "Line between Node " << v1id << " with itself does not make sense" << std::endl;
		return false;
	}

	Node* v1 = model->getNode(v1id);
	Node* v2 = model->getNode(v2id);


	if (v1 == nullptr)
	{
		std::cerr << "Node " << v1id << " does not exist" << std::endl;
		return false;
	}

	if (v2 == nullptr)
	{
		std::cerr << "Node " << v2id << " does not exist" << std::endl;
		return false;
	}

	if (v1->getType() != Node::Types::COUPLER)
	{
		if (v2->getType() != Node::Types::COUPLER)
		{
			std::cerr << "Line between two Sources is not allowed" << std::endl;
			return false;
		}
	}

	if (model->lineExist(v1id, v2id))
	{
		std::cerr << "Line between " << v1id << " and " << v2id << " already exists" << std::endl;
		return false;
	}

	Line* l = new Line(v1, v2, length);
	model->addLine(l);
	v1->addLine(l);
	v2->addLine(l);
	return true;
}
/** Adds a Line to the grid.
* @param model the GridModel where the Line shall be added to.
* @param v1id the id of the first Node.
* @param v2id the id of the second Node.
* length the length of the Line.**/
bool GridModelHelper::addLine(GridModel* model, int v1id, int v2id, double length)
{
	return addLine(model,std::to_string(v1id), std::to_string(v2id), length);
}

/** Checks if a Node with given id can be added to the GridModel.
* @param model the GridModel where the Line shall be added to.
* @param v1 the id of the Node.**/
bool GridModelHelper::checkNodeAdding(GridModel* model, std::string id)
{
	if (model->isLocked())
	{
		std::cerr << "Adding a Node to a locked model is not possible is not allowed" << std::endl;
		return false;
	}
	if (model->nodeExist(id))
	{
		std::cerr << "Node " << id << " already exists" << std::endl;
		return false;
	}
}
