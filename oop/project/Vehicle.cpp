#include <iostream>
#include <string>
#include <iomanip>  //for set() and setfill()
using namespace std;

#include "Vehicle.h"

Vehicle::Vehicle()
	: Logistic()
{
	this->capacity = V_SMALL;
}

Vehicle::Vehicle(string name, string destination, int hour, int min, int capacity)
	: Logistic(name, destination, hour, min)
{
	this->capacity = capacity;
}
void Vehicle::setCapacity(int capacity) {
	this->capacity = capacity;
}
int Vehicle::getCapacity() {
	return capacity;
}
void Vehicle::decCapacity() {
	capacity--;
}
void Vehicle::displayVehicle() {
	displayLogistic();
	cout << left << setw(15) << capacity;
}

ostream& operator<<(ostream& out, const Vehicle& vh) {
	const Logistic& LogObj(vh);
	out << LogObj;
	out << left << setw(15) << vh.capacity;
	return out;
}