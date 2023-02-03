#pragma once

#include "Logistic.h"

class Vehicle : public Logistic
{
	friend ostream& operator<<(ostream& out, const Vehicle& vh);
private:
	int capacity;
	static const int V_SMALL;
	static const int V_LARGE;
	static const int V_SUPER;

public:
	Vehicle();								//default constructor
	Vehicle(string name, string destination, int hour, int min, int capacity = V_SMALL); //non-default constructor
	void setCapacity(int capacity);
	int getCapacity();
	void decCapacity();
	void displayVehicle();
};

