#pragma once
#include "Logistic.h"
class Commuter : public Logistic
{
	friend ostream& operator<<(ostream& out, const Commuter& c);

public:
	Commuter();
	Commuter(string name, string destination, int hour, int min);
	void displayCommuter();
	
};
