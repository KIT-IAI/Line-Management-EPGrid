#include "Coupler.h"
/** Constructor
* @param id the id of the node.**/
Coupler::Coupler(std::string id) :Node(id)
{
}
/** Return the type, which is COUPLER.
* @return Types::COUPLER.**/
Node::Types Coupler::getType()
{
	return Types::COUPLER;
}
/** Returns a string containing all static data of the coupler.
* @return String containing all static data.**/
std::string Coupler::serialize()
{
	return 	Node::serialize() + "," + "C";
}