#pragma once
/** Class which stores different cable types and their properties. **/
class CableTypes
{	
public:
enum class Types { NAYY150SE, NAYY50SE };
	static double getResistance(Types t);
	static double getInductance(Types t);
	static int getLimit(Types t);
	private:
};

