#include <iostream>
#include <string>
#include <iomanip>  //for set() and setfill()
using namespace std;

#include "Commuter.h"

Commuter::Commuter()
	:Logistic() {}

Commuter::Commuter(string name, string destination, int hour, int min)
	: Logistic(name, destination, hour, min) {}

void Commuter::displayCommuter() {
	displayLogistic();
}

ostream& operator<<(ostream& out, const Commuter& c) {
	const Logistic& logi(c);
	out << logi;
	return out;
}