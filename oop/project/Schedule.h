#pragma once
#include <iomanip>
class Schedule
{
	friend ostream& operator<<(ostream& out, const Schedule& sch);
private:
	int schID;
	Vehicle vehicleObj;
	Commuter commuterObj;
public:

	Schedule(int schID, Vehicle vehicle, Commuter commuter);
	void displaySchedule();
	int getSchId();
	Vehicle getVehicle();
	Commuter getCommuter();
};

