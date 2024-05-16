#include "EPCurrentRectangular.h"
#include "../../Config.h"


/** Constructor.
* @param v1id the id of the supplier.
* @param v2id the id of the absorber.
* @param current the current of the transfer.
* @param begin the beginning of the transfer.
* @param duration the duration of the transfer.**/
EPCurrentRectangular::EPCurrentRectangular(std::string v1id, std::string v2id, double current, int begin, int duration) :EP(v1id, v2id)
{
	addCurrentSetpoint(current, toTimeslot(begin), toTimeslot(duration), Setpoint::Types::CONSTANT);
}