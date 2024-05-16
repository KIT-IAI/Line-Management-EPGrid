#include "EP.h"

/** Class for describing an energy packet with a trapezoidal current shape. **/
class EPCurrentTrapeze : public EP
{
public:
	EPCurrentTrapeze(std::string v1id, std::string v2id, double current, double rampCurrent, int begin, double duration);
	EPCurrentTrapeze(std::string v1id, std::string v2id, double current, double rampUpCurrent, double rampDownCurrent, int begin, double duration);

private:
	double getRampDuration(double current, double rampCurrent);
	double getRampCurrentPerTimeslot(double rampCurrent);
	void setRampUp(double current, double rampUpCurrent,int begin, int duration);
	void setConstant(double current, int begin, int duration);
	void setRampDown(double current, double rampDownCurrent, int begin, int duration);
};