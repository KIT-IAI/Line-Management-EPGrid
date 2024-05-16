#include "GridModelGenerator.h"
#include "GridModelHelper.h"
#include "../Config.h"
#include "../math/Randomizer.h"

/** Creates a electrical grid which can be displayed in grid layout. Each Coupler is connected to exactly one controller and two lines (except the two Coupler Objects at the grid border). The length of the lines gets equally distributed between the couplers and overall exactly adds up to 1 km.
* @param participantCount The number of participants the grid shall have. This includes one VoltageSource. **/
GridModel* GridModelGenerator::setupGridNetwork(int participantCount)
{
	GridModel* model=new GridModel();
	GridModelHelper::addVoltageSource(model, 0, VOLTAGE);
	GridModelHelper::addCoupler(model, 1);
	GridModelHelper::addLine(model, 0, 1, 20);

	for (int i = 1; i < participantCount; i++)
	{
		GridModelHelper::addCurrentSource(model, i * 2);
		GridModelHelper::addCoupler(model, i * 2 + 1);
		GridModelHelper::addLine(model, i * 2, i * 2 + 1, 20);//Controller to Coupler
		GridModelHelper::addLine(model, (i - 1) * 2 + 1, i * 2 + 1, 1000 / participantCount);//Coupler to previous Coupler
	}
	return model;
}
/** Creates a electrical grid with randomized layout. Each Coupler is connected to muliple controllers and two lines (except the two Coupler Objects at the grid border). The length of the lines gets equally distributed between the couplers and overall exactly adds up to 1 km.
* @param participantCount The number of participants the grid shall have. This includes one VoltageSource. **/
GridModel* GridModelGenerator::setupPartlyRandomizedNetwork(int participantCount)
{
	GridModel* model = new GridModel();
	GridModelHelper::addVoltageSource(model, 0, VOLTAGE);
	GridModelHelper::addCoupler(model, 1);
	GridModelHelper::addLine(model, 0, 1, 20);
	int couplerCount = 1;
	int controllerCount = 1;
	for (int i = 1; i < participantCount; i++)
	{
		int controllerID = controllerCount * 2;
		controllerCount++;
		GridModelHelper::addCurrentSource(model, controllerID);
		//TODO: validate change of randimzaition method here
		if (Randomizer::getRandomInt(0,99) < std::min((float)50, (float)i / controllerCount * 100 * 4))
		{
			int couplerID = couplerCount * 2 - 1 - (Randomizer::getRandomInt(0,std::min(3, couplerCount-1)) * 2);
			GridModelHelper::addLine(model, couplerID, controllerID, 20);//Controller to existing Coupler
		}
		else
		{
			int couplerID = couplerCount * 2 + 1;
			couplerCount++;
			GridModelHelper::addCoupler(model, couplerID);
			GridModelHelper::addLine(model, couplerID - 2, couplerID, 1000 / controllerCount);//new Coupler to previous Coulper
			GridModelHelper::addLine(model, couplerID, controllerID, 20);//Controller to new Coupler

		}
	}
	return model;
}

/** Creates a reference grid similar to DOI:10.1007/978-3-8348-9761-9 Bild 3.7 Verzweigter Ring. **/
GridModel* GridModelGenerator::setupReferenceGrid()
{
	GridModel* model = new GridModel();
	GridModelHelper::addVoltageSource(model, 0, VOLTAGE);
	GridModelHelper::addCoupler(model, 1);
	GridModelHelper::addLine(model, 0, 1, 10);
	for (int i = 1; i <= 30; i++)
	{
		std::set<int> nonExistingCurrentControllers = { 4,8,36,40,48 };
		GridModelHelper::addCoupler(model, i * 2 + 1);
		if (nonExistingCurrentControllers.count(i * 2) == 0)
		{
			GridModelHelper::addCurrentSource(model,i * 2);
			GridModelHelper::addLine(model, i * 2, i * 2 + 1, 20);
		}
	}
	GridModelHelper::addLine(model, 1, 3, 20);
	GridModelHelper::addLine(model, 3, 5, 20);
	GridModelHelper::addLine(model, 5, 7, 20);
	GridModelHelper::addLine(model, 7, 9, 20);
	GridModelHelper::addLine(model, 9, 11, 100);
	GridModelHelper::addLine(model, 5, 13, 80);
	GridModelHelper::addLine(model, 13, 15, 20);
	GridModelHelper::addLine(model, 15, 17, 30);
	GridModelHelper::addLine(model, 17, 19, 20);

	GridModelHelper::addLine(model, 1, 21, 20);
	GridModelHelper::addLine(model, 21, 23, 20);
	GridModelHelper::addLine(model, 23, 25, 20);
	GridModelHelper::addLine(model, 25, 27, 20);
	GridModelHelper::addLine(model, 27, 29, 20);

	GridModelHelper::addLine(model, 1, 31, 40);
	GridModelHelper::addLine(model, 31, 33, 20);
	GridModelHelper::addLine(model, 33, 35, 20);
	GridModelHelper::addLine(model, 35, 37, 20);
	GridModelHelper::addLine(model, 37, 39, 30);
	GridModelHelper::addLine(model, 39, 41, 20);
	GridModelHelper::addLine(model, 41, 43, 20);

	GridModelHelper::addLine(model, 1, 45, 40);
	GridModelHelper::addLine(model, 45, 47, 20);
	GridModelHelper::addLine(model, 47, 49, 30);
	GridModelHelper::addLine(model, 49, 51, 20);
	GridModelHelper::addLine(model, 51, 53, 20);
	GridModelHelper::addLine(model, 49, 55, 50);
	GridModelHelper::addLine(model, 55, 57, 20);
	GridModelHelper::addLine(model, 57, 59, 20);
	GridModelHelper::addLine(model, 59, 61, 80);
	return model;
}
