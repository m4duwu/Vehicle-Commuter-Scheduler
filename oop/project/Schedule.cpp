#include <iostream>
#include <string>
#include <iomanip>  //for set() and setfill()
using namespace std;

#include "Vehicle.h"
#include "Commuter.h"
#include "Schedule.h"

Schedule::Schedule(int schID, Vehicle vehicle, Commuter commuter)
	: vehicleObj(vehicle), commuterObj(commuter)
{
	this->schID = schID;
}

void Schedule::displaySchedule() {
	cout << left << setw(15) << schID;
	vehicleObj.displayVehicle();
	commuterObj.displayCommuter();
	cout << endl;
}

ostream& operator<<(ostream& out, const Schedule& sch) {
	out << left << setw(15) << sch.schID;
	out << sch.vehicleObj;
	out << sch.commuterObj;
	return out;

}

int Schedule::getSchId()
{
	return schID;
}

Vehicle Schedule::getVehicle()
{
	return vehicleObj;
}

Commuter Schedule::getCommuter()
{
	return commuterObj;
}