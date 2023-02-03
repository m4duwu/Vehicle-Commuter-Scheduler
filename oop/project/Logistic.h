#pragma once
#include<iostream>
#include<string>
using namespace std;
class Logistic
{
	friend ostream& operator<<(ostream& out, const Logistic& lg);
private:
	string name;
	string destination;
	int hour;
	int min;
public:
	Logistic(string name = "No_name", string destination = "No_location", int hour = 0, int min = 0);
	void setName(string name);
	string getName();
	void setDestination(string destination);
	string getDestination();
	void setHour(int hour);
	int getHour();
	void setMin(int min);
	int getMin();
	double getTime();
	bool getGrace(int ohour, int omin);
	void displayLogistic();

};
