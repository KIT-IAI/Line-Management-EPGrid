#include "CableTypes.h"

/** Return the resistance of a given cable type 
* @param t the type of the cable.
* @return The resistance in ohm per km. **/
double CableTypes::getResistance(Types t)
{
	switch (t)
	{
	case Types::NAYY150SE:
		return 0.206;
	case Types::NAYY50SE:
		return 0.641;
	default:
		return -1;
	}
}
/** Return the inductance of a given cable type
* @param t the type of the cable.
* @return The inductance in henry per km. **/
double CableTypes::getInductance(Types t)
{
	switch (t)
	{
	case Types::NAYY150SE:
		return 0.000256;
	case Types::NAYY50SE:
		return 0.00027;
	default:
		return -1;
	}

}
/** Return the current limit of a given cable type
* @param t the type of the cable.
* @return The current limit in ampere. **/
int CableTypes::getLimit(Types t)
{
	switch (t)
	{
	case Types::NAYY150SE:
		return 275;
	case Types::NAYY50SE:
		return 144;
	default:
		return -1;
	}
}