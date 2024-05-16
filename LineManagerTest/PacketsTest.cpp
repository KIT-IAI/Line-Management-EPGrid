#include "pch.h"
#include "CppUnitTest.h"
#include "Config.h"
#include "model/packets/EP.h"
#include "model/packets/EPCurrentTrapeze.h"
#include "model/packets/EPPowerTrapeze.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EPTest
{
	TEST_CLASS(EPTest)
	{
	public:
		void checkMaxPower(EP* ep, double  maxPower)
		{
			for (Setpoint* sp : ep->getSetpoints())
			{
				if (sp->getSupplierPower() > maxPower)
				{
					Assert::Fail();
				}
				if (sp->getAbsorberPower() < -maxPower)
				{
					Assert::Fail();
				}
			}
		}
		void checkMaxCurrent(EP* ep, double maxCurrent)
		{
			for (Setpoint* sp : ep->getSetpoints())
			{
				if (sp->getSupplierCurrent() > maxCurrent)
				{
					Assert::Fail();
				}
				if (sp->getAbsorberCurrent() < -maxCurrent)
				{
					Assert::Fail();
				}

			}
		}
		void checkMinPower(EP* ep)
		{
			for (Setpoint* sp : ep->getSetpoints())
			{
				if (sp->getSupplierPower() <= 0)
				{
					Assert::Fail();
				}
				if (sp->getAbsorberPower() >= 0)
				{
					Assert::Fail();
				}
			}
		}
		void checkMinCurrent(EP* ep)
		{
			for (Setpoint* sp : ep->getSetpoints())
			{
				if (sp->getSupplierCurrent() <= 0)
				{
					Assert::Fail();
				}
				if (sp->getAbsorberCurrent() >= 0)
				{
					Assert::Fail();
				}
			}
		}

		void checkPowerRampLimit(EP* ep,double rampPower)
		{
						int hit = 0;
			double lastPower = 0;
			for (Setpoint* sp : ep->getSetpoints())
			{

				if (abs(abs(sp->getSupplierPower() - lastPower) - (rampPower * S_PER_TIMESLOT)) > 0.00001)
				{
					hit++;
				}
				lastPower = sp->getSupplierPower();
			}
			if (hit > 4)
			{
				Assert::Fail();
			}
		}
		void checkCurrentRampLimit(EP* ep, double rampCurrent)
		{
				int hit = 0;
			double lastCurrent = 0;
			for (Setpoint* sp : ep->getSetpoints())
			{

				if (abs(abs(sp->getSupplierCurrent() - lastCurrent) - (rampCurrent * S_PER_TIMESLOT)) > 0.00001)
				{
					hit++;
				}
				lastCurrent = sp->getSupplierCurrent();
			}
			if (hit > 4)
			{
				Assert::Fail();
			}
		}

		void checkEnergy(EP* ep, double energy,double power)
		{
			double actEnergy = 0;
			Setpoint* lastSp = new Setpoint(0, 0, 0, 0, 0, 0, Setpoint::Types::CONSTANT);
			for (Setpoint* sp : ep->getSetpoints())
			{
				switch (sp->getType())
				{
				case Setpoint::Types::CONSTANT:
					actEnergy += (sp->getSupplierPower() / 3600) * S_PER_TIMESLOT * sp->getDuration();
					break;
				case Setpoint::Types::RAMP:
					actEnergy += (0.5 * abs(sp->getSupplierPower() - lastSp->getSupplierPower()) * S_PER_TIMESLOT) / 3600;
					actEnergy += (lastSp->getSupplierPower() / 3600) * S_PER_TIMESLOT;
					break;
				}
				lastSp = sp;
			}
			if (abs(actEnergy - energy) > 2 * power / 3600 * S_PER_TIMESLOT)
			{
				Assert::Fail();
			}
		}
		void checkNoPower(EP* ep)
		{
			for (Setpoint* sp : ep->getSetpoints())
			{
				if (sp->getSupplierPower() != 0)
				{
					Assert::Fail();
				}
				if (sp->getAbsorberPower() != 0)
				{
					Assert::Fail();
				}
			}
		}
		void checkNoCurrent(EP* ep)
		{
			for (Setpoint* sp : ep->getSetpoints())
			{
				if (sp->getSupplierCurrent() != 0)
				{
					Assert::Fail();
				}
				if (sp->getAbsorberCurrent() != 0)
				{
					Assert::Fail();
				}
			}
		}

		void checkPowerCourse(EP* ep)
		{
			double lastPower = 0;
			double lastGradient = 1;
			double gradient;
			for (Setpoint* sp : ep->getSetpoints())
			{
				gradient = sp->getSupplierPower() - lastPower;
				if (lastGradient > 0 && gradient < 0)
				{
					Assert::Fail();
				}
				if (lastGradient == 0 && gradient > 0)
				{
					Assert::Fail();
				}
				if (lastGradient < 0 && gradient >= 0)
				{
					Assert::Fail();
				}
				lastPower = sp->getSupplierPower();
				lastGradient = gradient;
			}
		}

		void checkCurrentCourse(EP* ep)
		{
			double lastPower = 0;
			double lastGradient = 1;
			double gradient;
			for (Setpoint* sp : ep->getSetpoints())
			{
				gradient = sp->getSupplierCurrent() - lastPower;
				if (lastGradient > 0 && gradient < 0)
				{
					Assert::Fail();
				}
				if (lastGradient == 0 && gradient > 0)
				{
					Assert::Fail();
				}
				if (lastGradient < 0 && gradient >= 0)
				{
					Assert::Fail();
				}
				lastPower = sp->getSupplierCurrent();
				lastGradient = gradient;
			}
		}
		void checkDuration(EP* ep, double duration)
		{
			int durationSlots = duration / S_PER_TIMESLOT;
			if (ep->getDuration() != durationSlots)
			{
				Assert::Fail();
			}
		}
		TEST_METHOD(PowerTrapeze)
		{
			double power;
			double rampPower;
			double energy;
			EP* ep = NULL;
			srand(time(NULL));
			for (int i = 0; i < 1000; i++)
			{

				ep = NULL;
				while (ep == NULL)
				{
					power = rand() % 30000 + 1;
					rampPower = rand() % 20000 + 1;
					energy = rand() % 20000 + 1;

					try {
						ep = new EPPowerTrapeze("0", "1", power, rampPower, 0, energy);
					}
					catch (const std::exception& e) {
					}
				}
				checkNoCurrent(ep);
				checkMaxPower(ep,power);
				checkMinPower(ep);
				checkPowerCourse(ep);
				checkPowerRampLimit(ep,rampPower);
				checkEnergy(ep,energy,power);
			}
		}

		TEST_METHOD(CurrentTrapeze)
		{
			for (int i = 0; i < 1000; i++)
			{
				double current;
				double rampCurrent;
				double duration;
				EP* ep = NULL;
				srand(time(NULL));
				while (ep == NULL)
				{
					current = rand() % 130 + 1;
					std::cout << current << std::endl;
					rampCurrent = rand() % 50 + 1;
					duration = rand() % 260 + 1;
					try {
						ep = new EPCurrentTrapeze("0", "1", current, rampCurrent, 0, duration);
					}
					catch (const std::exception& e) {
					}
				}
				checkNoPower(ep);
				checkMaxCurrent(ep,current);
				checkMinCurrent(ep);
				checkCurrentCourse(ep);
				checkCurrentRampLimit(ep, rampCurrent);
				checkDuration(ep,duration);
			}
		}
	};
}