#include <iostream>         //cout cin
#include <string>           //string
#include <iomanip>			//For set() and setfill()
#include <vector>           /* Vector functions*/
#include <fstream>			//For file access
#include <sstream>			//For stringstream
#include <thread>			//For thread control - sleep
#include <algorithm>		//For sorting
#include <conio.h>
#include <iterator>
#include <windows.h>        //for console aesthetics
#include <regex>             //For formating of string

using namespace std;

#include "Vehicle.h"
#include "Commuter.h"
#include "Schedule.h"

/****************** Constants*************************************************/
static constexpr int VEHICLE_SMALL = 2;
static constexpr int VEHICLE_LARGE = 4;
static constexpr int VEHICLE_SUPER = 6;

/****************** class static constant ************************************/
const int Vehicle::V_SMALL = VEHICLE_SMALL;
const int Vehicle::V_LARGE = VEHICLE_LARGE;
const int Vehicle::V_SUPER = VEHICLE_SUPER;


//********global variables**********//
string vn;
string dt;
string temp;
int cp;
int hr;
int mn;

//Global variables for Path
string pathf_vehicle;
string pathf_commuter;
string line; //to get string for each line of file
//********END OF global variables**********//

//assigns these names to integer values
/****************** Enumeration ***********************************/ 
enum {
	M_MENU,
	M_DISP_VEH,
	M_DISP_COMM,
	M_SCH_ARRIVAL_TIME,
	M_SCH_MIN_VEHICLE,
	displayAllVehicle_NOT_FULL,
	M_DISP_REMAINING_COMMUTERS,
	M_DISP_SCH_PLAN,
	M_SAVE_SCH_PLAN,
	M_MODIFY_VEHICLES,
	M_MODIFY_COMMUTERS,
	M_CSV,
	M_SEARCH_VEH,
	M_SEARCH_COMM,
	M_EXIT = -1
}Menu;
/******************END OF Enumeration ***********************************/

/****************** Function Prototypes ***************************/
void vehiclePath(vector <Vehicle>& vv);                //Spects: to open vehicle text file
void commuterPath(vector <Commuter>& vc);              //Spects: to open commuter text file

//menus //
void m_welcome();                                       //Spects: welcome screen, shows members name
int mainMenu();                                         //Spects: main menu of our project
int modifyMenu();                                       //Spects: Modification menu for cases 9 and 10
void changeMenu();                                      //Spects: to seamlessly change menus to prevent any sorts of hiccups or bugs (For ALL cases)
int changeMenuEdit();                                   //Spects: to seamlessly change menus to prevent any sorts of hiccups or bugs (For modify menu)

//display objects //
void displayAllVehicle(vector <Vehicle>& vv);           //Spects: to display all vehicles in the vehicle vector
void displayAllCommuter(vector <Commuter>& vc);         //Spects: to display all commuters in the commuter vector
void displaySchedule(vector<Schedule>& vs);             //Spects: to display all schedules in the schedule vector

//for scheduling //
vector<Schedule> generateSchedule(vector<Vehicle>& vv, vector<Commuter>& vc, int input); //Spects: To generate schedule vector by using the Vehicle and Commuter Vectors. (input is used to toggle bettween cases 3 and 4)
void saveSchedule(vector<Schedule>& vs);                //Spects: To print out schedule into a txt file

//for modify vehicle //
int addVehicle(vector<Vehicle>& vv);                    //Spects: Add vehicle obj into the vehicle vector
int deleteVehicle(vector<Vehicle>& vv);                 //Spects: Delete vehicle obj into the vehicle vector
int editVehicle(vector<Vehicle>& vv);                   //Spects: Edit vehicle obj in the vehicle vector (Includes all variables- name, dest, capacity and time)

//for modify commuter //
int addCommuter(vector<Commuter>& vc);                  //Spects: Add Commuter obj into the Commuter vector
int deleteCommuter(vector<Commuter>& vc);               //Spects: Delete Commuter obj into the Commuter vector
int editCommuter(vector<Commuter>& vc);                 //Spects: Edit Commuter obj in the Commuter vector (Includes all variables- name, dest and time)

//for sorting 
bool compVehDestAscTime(Vehicle v1, Vehicle v2);        //Spects: to sort vehicles via destination ascending time
bool compVehDestDescendCap(Vehicle v1, Vehicle v2);     //Spects: to sort vehicles via destination descending time
bool compComDestTime(Commuter c1, Commuter c2);         //Spects: to sort commuters via destination time

//for error checking and filtering (VALIDATION) //
bool check_number(string str);                          //Spects: To check if string input is a number or not
bool setTime(string& enteredTime, int& hour, int& min); //Spects: to ensure the time format is correctly entered (only [0-9]{2}:[0-9]{2} is accepted) and rejects all other inputs
string checkVnameFormat(int i);                         //Spects: to ensure Vehicle name is formatted correctly (only [V]{1}[0-9]{2} is accepted) and rejects all other inputs *converts to uppercase for user input convience
string checkCnameFormat(int i);                         //Spects: to ensure Commuter name is formatted correctly (only [C]{1}[0-9]{2} is accepted) and rejects all other inputs *converts to uppercase for user input convience
string checkDest(int i);                                //Spects: to Check if user input Destination is the same as the ones provided *converts to uppercase for user input convience

//ADDITIONAL FEATURES //
void saveScheduleCSV(vector<Schedule>& vs);             //Spects: To print out Schedule vector into a csv file
void searchVehicle(vector<Vehicle>& vv);                //Spects: For user to search for Vehicle and displays the contents
void searchComm(vector<Commuter>& vc);                  //Spects: For user to search for Commuter and displays the contents
/****************** END OF Function Prototypes ***************************/




HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //to be used for console text colors
/****************** Main ******************************************/
int main() { //
	
	vector <Vehicle> vehList;				//original vehicle list
	vector <Vehicle> vehUnderCap;			//vehicles still under capacity
	vector <Commuter> comList;				//original commuter list
	vector <Commuter> comLeft;				//commuters left behind with no vehicle 
	vector <Schedule> schList;				//schedule list

	bool exit_menu = false;
	int userInput = 0;

	system("CLS");                          //Clear Console
	m_welcome();                            //Shows welcome screen
	vehiclePath(vehList);					//read vehicle file and update vector list
	commuterPath(comList);					//read commuter file and update vector list
	changeMenu();                           //Change menu to main menu
	while (1)
	{
		userInput = 0;
		switch (mainMenu()) {
		case  M_DISP_VEH:
			displayAllVehicle(vehList);     //Prints out vehicle vector
			break;
		case M_DISP_COMM:
			displayAllCommuter(comList);    //Prints out commuter vector
			break;
		case M_SCH_ARRIVAL_TIME:
			vehUnderCap = vehList;			//clone a copy of vehicle vector list
			comLeft = comList;				//clone a copy of commuter vector list
			schList.clear();				//empty previous schedule plan
			schList = generateSchedule(vehUnderCap, comLeft, 0); //call for function and 0 is used to toggle for case 3
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			for (auto i : schList)i.displaySchedule(); //prints out schedule
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
			break;
		case M_SCH_MIN_VEHICLE:
			vehUnderCap = vehList;			//clone a copy of vehicle vector list
			comLeft = comList;				//clone a copy of commuter vector list
			schList.clear();				//empty previous schedule plan
			schList = generateSchedule(vehUnderCap, comLeft, 1); //call for function and 0 is used to toggle for case 4
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			for (auto i : schList)i.displaySchedule(); //prints out schedule
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
			break;
		case displayAllVehicle_NOT_FULL:
			displayAllVehicle(vehUnderCap);  //To display all Vehicles under Capacity
			break;
		case M_DISP_REMAINING_COMMUTERS:
			displayAllCommuter(comLeft);     //To display all Commuters without a Vehicle
			break;
		case M_DISP_SCH_PLAN:
			displaySchedule(schList);        //Display Latest Schedule plan
			break;
		case M_SAVE_SCH_PLAN:
			saveSchedule(schList);           //Prints out Latest Schedule plan onto txt file
			break;
		case M_MODIFY_VEHICLES:
			while (1) {
				userInput = 0;              //userInput is being used to toggle between cases
				switch (modifyMenu())       //Switch case here to access modify portion of the program (nested in main for ease of coding)
				{
				case 1:
					userInput = addVehicle(vehList);    //Add vehicle to vehicle vector
					break;
				case 2:
					userInput = deleteVehicle(vehList); //Delete vehicle from vehicle vector
					break;
				case 3:
					userInput = editVehicle(vehList);   //Edit vehicle from vehicle vector
					break;
				case 4: userInput = 2; break;           //Set userInput=2 to prepare to break from while(1) loop
				case 5:exit(0); break;                  //exit(0) to exit program
				default: system("cls"); 
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
					cout << "please use valid input!\n" << endl;  
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
					cin.clear(); cin.ignore(1000, '\n'); //cin.clear and cin.ignore is used to put the stream back into a good state and remove the bad input remaining in the input buffer. without this, there will be an infinite loop bug
					break;
					break;
				}
				if (userInput == 2)break;               //Break from this this while(1) loop and switch case and go back to Main menu
			}
			break;
		case M_MODIFY_COMMUTERS:
			while (1) {
				userInput = 0;              //userInput is being used to toggle between cases
				switch (modifyMenu())       //Switch case here to access modify portion of the program (nested in main for ease of coding)
				{
				case 1:
					userInput = addCommuter(comList);    //Add vehicle to vehicle vector
					break;
				case 2:
					userInput = deleteCommuter(comList); //Delete vehicle from vehicle vector
					break;
				case 3:
					userInput = editCommuter(comList);   //Edit vehicle from vehicle vector
					break;
				case 4: userInput = 2; break;            //Set userInput=2 to prepare to break from while(1) loop
				case 5:exit(0); break;                   //exit(0) to exit program
				default:  system("cls");
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
					cout << "please use valid input!\n" << endl;
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
					cin.clear(); cin.ignore(1000, '\n'); //cin.clear and cin.ignore is used to put the stream back into a good state and remove the bad input remaining in the input buffer. without this, there will be an infinite loop bug
					break;
					break;
				}
				if (userInput == 2)break;               //Break from this this while(1) loop and switch case and go back to Main menu
			}
			break;
		case M_CSV:
			saveScheduleCSV(schList);      //Prints out Schedule onto CSV file
			break;
		case M_SEARCH_VEH:
			searchVehicle(vehList);        //Search vehicle and displays its contents
			break;
		case M_SEARCH_COMM:
			searchComm(comList);           //Search commuter and displays its contents
			break;
		case M_EXIT:exit(0); break;		  //exit from program
		default: system("cls");
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "please use valid input!\n" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			cin.clear(); cin.ignore(1000, '\n'); //cin.clear and cin.ignore is used to put the stream back into a good state and remove the bad input remaining in the input buffer. without this, there will be an infinite loop bug
			break;
			break;
		}
		if (userInput != 2)              //To ensure the program stays in the modify menu as long as userinput is not 2 
			changeMenu();                //change menu to main menu
	}

	return 0;
}


//********************menus************************************////(DONE BY: JIN TIAN 2103163)
void m_welcome() //
{
	//Basically prints out some details about the group and the project
	system("CLS");
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "*****************************************************************" << endl;
	cout << "*****************************************************************" << endl;
	cout << "**                   TLM1008 OOP PROJECT                       **" << endl;
	cout << "**            Auto-Schedule Transport Vehicle                  **" << endl;
	cout << "*****************************************************************" << endl;
	cout << "*****************************************************************" << endl;
}
int mainMenu() { //(DONE BY: JIN TIAN 2103163)
	int userInput = 0;

	system("CLS");
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "MAIN MENU:" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "1. Display all vehicles" << "\n2. Display all commuters" << "\n3. Schedule commuter by arrival time" << "\n4. Schedule commuter by minimum vehicles" << endl;
	cout << "5. Display vehicle that is under capacity" "\n6. Display remaining commuters" << "\n7. Display scheduling plan" << "\n8. Save scheduling plan" << "\n9. Modify vehicle list" << endl;
	cout << "10. Modify commuter list" << "\n\nADDITIONAL FEATURES: \n" << "11. Save Scheduling plan in CSV\n" << "12. Search Vehicle\n" << "13. Search Commuter\n";
	cout << "\n\nKey -1 To End Program" << "\n\nPlease enter your option:\n>";
	
	cin >> userInput; //asking for user input to select switch case back in main
	return userInput;
}
int modifyMenu() { //
	string uiStr;
	int userInput = 0, exitCheck = 0;
	system("CLS");
	while (1)
	{
		//print menu
		cout << "-----------------------------------------------------------------------" << endl;
		cout << "1. Add" << endl;
		cout << "2. Delete" << endl;
		cout << "3. Edit" << endl;
		cout << "4. Back to Main Menu" << endl;
		cout << "Key 5 to End Program" << endl;
		cout << "Please enter your option: ";

		cin >> uiStr;

		if (check_number(uiStr) == true) //checking if userinput is a number
		{
			userInput = stoi(uiStr); //string to integer
			if (userInput == 1 || userInput == 2 || userInput == 3 || userInput == 4 || userInput == 5) //If integer is any of these numbers, break from while loop
				break;
			else
			{
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << "invalid input, try again!" << endl;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			}
		}
		else if (check_number(uiStr) == false) {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
			cout << "Please use number!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
	return userInput;
}
void changeMenu() { //
	while (1) {
		cout << "Enter any number key to return to main menu: " << "\n>";
		cin >> temp;
		if (check_number(temp))        //checking if user input is a number or not
			break;                     //break to change menu
		else
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "enter number please" << endl; //if user input is not a NUMBER, ask the user to type in valid input
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
	system("cls"); // clear console to display clean menu
}
int changeMenuEdit() { ////
	string uiStr;
	int userInput;
	while (1)
	{
		cout << "\n\nRETURN" << "\n-----------------------------------------------------------------------" << endl;
		cout << "1.Back to Previous Menu" << "\n2.Back to Main Menu" << endl;
		cin >> uiStr;

		if (check_number(uiStr) == true)  //check if user input is integer or not
		{
			userInput = stoi(uiStr);      //string to integer
			if (userInput == 1 || userInput == 2)
				break;
			else
			{
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
				cout << "invalid input, try again!" << endl;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			}
		}
		else if (check_number(uiStr) == false)
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
			cout << "Please use number!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
	return userInput;
}

//****************Read Files********************************////
void vehiclePath(vector <Vehicle>& vv) { //
	while (1) {
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "Enter folder/path-name for available ";
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "Vehicles";
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << " (Press Enter For Default)\n" << "> ";
		getline(cin, pathf_vehicle);       //use getline instead of cin so that WHITESPACES are included
		if (pathf_vehicle == "")           //check if userinput is blank or not (basically checking for enter)
			pathf_vehicle = "vehicle.txt"; //assigning path
	
		ifstream file(pathf_vehicle);      //open file using path given
		if (file.is_open())
		{
			
			while (getline(file, line)) { //going through file line by line

				//store file details into vehicle object
				stringstream sstream(line);
				getline(sstream, vn, ' ');
				getline(sstream, temp, ' ');
				cp = stoi(temp);
				getline(sstream, dt, ' ');
				getline(sstream, temp, ':');
				hr = stoi(temp);
				getline(sstream, temp, ':');
				mn = stoi(temp);

				Vehicle v(vn, dt, hr, mn, cp);
				vv.push_back(v); //adding object into vector list
			}
			file.close();
			break;
		}
		else {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Unable to open file" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}

	}
}
void commuterPath(vector <Commuter>& vc) { //(DONE BY: JOEL PHOON 2102880 && ZI YAO 2101711)
	while (1) {
		//get user input for file path
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "Enter folder/path-name for available ";
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "Commuters";
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << " (Press Enter For Default)\n" << "> ";
		getline(cin, pathf_commuter);         //use getline instead of cin so that WHITESPACES are included
		if (pathf_commuter == "")             //check if userinput is blank or not (basically checking for enter)
			pathf_commuter = "commuter.txt";  //assigning path
		


		//open file using path given
		ifstream file(pathf_commuter);
		if (file.is_open())
		{
			//going through file line by line
			while (getline(file, line)) {

				//store file details into vehicle object
				stringstream sstream(line);
				getline(sstream, vn, ' ');
				getline(sstream, dt, ' ');
				getline(sstream, temp, ':');
				hr = stoi(temp);
				getline(sstream, temp, ':');
				mn = stoi(temp);

				Commuter c(vn, dt, hr, mn);
				vc.push_back(c);
			}
			file.close();
			break;
		}
		else {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Unable to open file" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
}


//*****************schedule function*********************////
vector<Schedule> generateSchedule(vector<Vehicle>& vvv, vector<Commuter>& vcc, int input) { //

	system("CLS");
	vector<Schedule> sched;
	int z = 0;
	size_t i = 0;
	size_t j = 0;
	if (input == 0)      //toggle for case 3
	{
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Display Schedule By Minimum Vehicle:" << "\n-----------------------------------------------------------------------" << endl;

		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

		//Sort accordingly to get expected output
		sort(vvv.begin(), vvv.end(), compVehDestAscTime);
		sort(vcc.begin(), vcc.end(), compComDestTime);
	}
	else if (input == 1) //toggle for case 4
	{
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Display Schedule By Minimum Vehicle:" << "\n-----------------------------------------------------------------------" << endl;

		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

		//Sort accordingly to get expected output
		sort(vvv.begin(), vvv.end(), compVehDestDescendCap);
		sort(vcc.begin(), vcc.end(), compComDestTime);
	}
	while (i < vvv.size())
	{
		j = 0;
		while (j < vcc.size())
		{
			int c_hr = vcc.at(j).getHour(); //to be used to calculate for grace time
			int c_min = vcc.at(j).getMin(); //to be used to calculate for grace time

			if ((vvv.at(i).getDestination() == vcc.at(j).getDestination()) && (vvv.at(i).getGrace(c_hr, c_min) == 1) && (vvv.at(i).getCapacity() > 0)) //Checking if Vehicle and Commuter time is OK and destination is the same
			{
				Schedule s(++z, vvv.at(i), vcc.at(j)); //creating Schedule object with schedule ID, vechicle object and commuter object 
				sched.push_back(s);                    //Add schedule object into Schedule vector
				vvv.at(i).decCapacity();               //Decrease Capacity for vehicle as +1 Commuter
				vcc.erase(vcc.begin() + j);            //Erase commuter that has already boarded the vehicle above

			}
			else
				j++;                                   //Positon of commuter should stay unless otherwise

		}
		if (vvv.at(i).getCapacity() == 0)              //Check if capacity of vehicle is 0
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "vehicle: " << vvv.at(i).getName() << " Capacity Full!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			vvv.erase(vvv.begin() + i);                //If capacity of vehicle is 0, erase it
		}
		else
			i++;                                       //Position of vehicle should stay unless otherwise
	}
	cout << "\n-----------------------------------------------------------------------" << endl;
	return sched;
}
//*****************Save Scheduling Plan****************//
void saveSchedule(vector<Schedule>& vs) { //
	system("CLS");
	ofstream file;

	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Save Schedule Plan:" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	if (vs.size() == 0) { //To check if there's a schedule created already or not
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "\nSchedule  has not been planned" << endl;
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	else {
		file.open("Schedule.txt"); //open up txt file to get ready to write
		file << "Schedule Plan:" << "\n---------------------------------------------------------------------------------------------------------------" << endl;

		//Header
		file << left << setw(15) << "ID" << left << setw(15) << "Vehicle" << left << setw(15) << "Destination" << left << setw(15) << "Time" << left << setw(15) << "Capacity" << left << setw(15) << "Commuter" << left << setw(15) << "Destination" << left << setw(15) << "Time" << endl;

		if (file) {
			for (auto& v : vs) { //Writing schedule onto txt file
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
				if (v.getVehicle().getMin() > 10) //IF ELSE to ensure the timings are neatly written
					file << left << setw(15) << v.getSchId() << left << setw(15) << v.getVehicle().getName() << left << setw(15) << v.getVehicle().getDestination() << v.getVehicle().getHour() << ":" << left << setw(15) << v.getVehicle().getMin() << left << setw(15) << v.getVehicle().getCapacity() << left << setw(15) << v.getCommuter().getName() << left << setw(15) << v.getCommuter().getDestination() << v.getCommuter().getHour() << ":" << left << setw(15) << v.getCommuter().getMin() << endl;
				else if (v.getVehicle().getMin() < 10)
					file << left << setw(15) << v.getSchId() << left << setw(15) << v.getVehicle().getName() << left << setw(15) << v.getVehicle().getDestination() << v.getVehicle().getHour() << ":" << "0" << left << setw(14) << v.getVehicle().getMin() << left << setw(15) << v.getVehicle().getCapacity() << left << setw(15) << v.getCommuter().getName() << left << setw(15) << v.getCommuter().getDestination() << v.getCommuter().getHour() << ":" << left << setw(15) << v.getCommuter().getMin() << endl;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
			cout << "\nSuccesfully saved file : " << "Schedule.txt" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
		else {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
			cout << "\nError Opening Schedule.txt File" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
	file.close();
}


//******************Display*****************************////
void displayAllVehicle(vector <Vehicle>& vv) { //
	system("CLS");
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Display All Vehicles Under Capacity:" << "\n-----------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	//Header
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << left << setw(15) << "Name" << left << setw(15) << "Destination" << left << setw(15) << "Time" << left << setw(15) << "Capacity" << endl;

	/* Display vehicle list */
	for (auto& v : vv) {
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN );
		v.displayVehicle();
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		cout << endl;
	}
}
void displayAllCommuter(vector <Commuter>& vc) { //
	system("CLS");
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Display All Remaining Commuters:" << "\n-----------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	//Header
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << left << setw(15) << "Name" << left << setw(15) << "Destination" << left << setw(15) << "Time" << endl;

	//Display commuters
	for (auto& v : vc) {
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		v.displayCommuter();
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		cout << endl;
	}
}
void displaySchedule(vector<Schedule>& vs) { //
	system("CLS");
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN );
	cout << "Display Schedule Plan:" << "\n---------------------------------------------------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	if (vs.size() == 0)
	{
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Schedule has not been made!\n" << endl;
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	//Header
	else {
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << left << setw(15) << "ID" << left << setw(15) << "Vehicle" << left << setw(15) << "Destination" << left << setw(15) << "Time" << left << setw(15) << "Capacity" << left << setw(15) << "Commuter";
		cout << left << setw(15) << "Destination" << left << setw(15) << "Time" << endl;

		//Display Schedules 
		for (auto& v : vs) {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
			v.displaySchedule();
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		}
	}
}



//*******************modify portion of program***********************//
//modifymenu

//modify menu for Vehicle //
int addVehicle(vector<Vehicle>& vv) { //
	system("CLS");
	string vn, dest, vt, capStr, uiStr;
	int hr, min, userInput, cap = 0, flag;
	size_t x = 0;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Add Vehicle: " << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	vn = checkVnameFormat(0);//check name to see if its in the correct format

	while (x < vv.size())
	{
		flag = 0;   //Flag is here so that program wont skip the FIRST obj in the vector due to the x++ at the end of the loop
		if (vn == vv.at(x).getName())
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Vehicle already exists!\n\n";
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
			vn = checkVnameFormat(0); //checking name format
			flag = 1;
			x = 0;
		}
		if (flag == 0)
			x++;
	}
	while (1) {
		cout << "\nEnter vehicle capacity in number, small(2) , large(4), super(6): ";
		cin >> capStr;
		if (check_number(capStr) == true)         //to check if input is a number
		{
			cap = stoi(capStr);                   //String to integer
			if (cap == 2 || cap == 4 || cap == 6) //If integer is any of these numbers, break from while loop
				break;
			else
			{
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << "Invalid input!" << endl;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			}
		}
		else if (check_number(capStr) == false) {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Please use number!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}

	cout << "\nAvailable Destination: " << "\nAljunied\n" << "Bedok\n" << "Bishan\n" << endl;
	dest = checkDest(1);                          //check for correct input of destination

	while (1) {
		cout << "\nEnter Vehicle time in hh:mm : ";
		cin >> vt;
		if (setTime(vt, hr, min) == true)        //checking for correct format for time
			break;
	}

	Vehicle temp;
	temp.setName(vn);
	temp.setCapacity(cap);
	temp.setDestination(dest);
	temp.setHour(hr);
	temp.setMin(min);
	vv.push_back(temp);

	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\nSuccesfully added Vehicle!" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "\n========================================================================\n" << left << setw(15) << "Vehicle ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << left << setw(15) << "Capacity" << endl;
	vv.back().displayLogistic();
	cout << left << setw(15) << vv.back().getCapacity() << "\n========================================================================" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	return userInput = changeMenuEdit();
}
int deleteVehicle(vector<Vehicle>& vv) { //
	displayAllVehicle(vv);
	string vn, uiStr; //vehicle name and userInput string to be used for checks
	int userInput;    //return value
	bool non_existant = true; //to check if the vehicle exists

	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Delete Vehicle: " << "\n-----------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	vn = checkVnameFormat(1);//check name to see if its in the correct format

	for (size_t x = 0; x < vv.size(); x++)
	{
		if (vv.at(x).getName() == vn) { //to check if vehicle user gave exists in the vector
			non_existant = false;
			system("CLS");
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "\nVehicle Found" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "\nVehicle Name: " << vn << " deleted from the current vehicle list!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			cout << "\n========================================================================\n" << left << setw(15) << "Vehicle ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << left << setw(15) << "Capacity" << endl;
			vv.at(x).displayLogistic();
			cout << left << setw(15) << vv.back().getCapacity() << "\n========================================================================" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

			vv.erase(vv.begin() + x); //delete vehicle
			break;
		}
	}

	if (non_existant) {
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "\nVehicle was not found. ";
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}

	return userInput = changeMenuEdit();
}
int editVehicle(vector<Vehicle>& vv) { //
	system("CLS");
	string dest, vn, nvn, vt, capStr, uiStr;
	int hour = 0, min = 0, cap = 0, c = 0, pos, userInput, flag;
	size_t x = 0;
	bool non_existant = true;

	cout << "Edit Vehicle: " << endl;

	while (1) {
		vn = checkVnameFormat(1);//check name to see if its in the correct format

		for (size_t x = 0; x < vv.size(); x++)
		{
			if (vv.at(x).getName() == vn) { //check if vehicle is in vector
				cout << "Vehicle found!" << endl;
				non_existant = false;
				pos = x;                    //getting position of vector to be used for editing
				break;
			}
		}
		if (non_existant)
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Vehicle not found! Try again" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
		else if (non_existant == false)
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "\n========================================================================\n" << left << setw(15) << "Vehicle ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << left << setw(15) << "Capacity" << endl;
			vv.at(pos).displayLogistic();
			cout << left << setw(15) << vv.at(pos).getCapacity() << "\n========================================================================" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);

			while (1) {
				c = 0;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << "Edit Vehicle Items: " << endl;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				while (1) {
					cout << "-----------------------------------------------------------------------" << "\n1. Vehicle Name" << "\n2. Capacity" << "\n3. Destination";
					cout << "\n4. Recharging Time" << "\n5. Back to previous menu" << "\n6. Back to Main menu" << endl;
					cout << "\n Please enter your option: ";
					cin >> uiStr;

					if (check_number(uiStr) == true) //check for valid user input
					{
						userInput = stoi(uiStr);
						if (userInput == 1 || userInput == 2 || userInput == 3 || userInput == 4 || userInput == 5 || userInput == 6)
							break;
						else {
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
							cout << "invalid input, try again!" << endl;
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						}
					}
					else if (check_number(uiStr) == false)
					{
						SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
						cout << "Please use number!" << endl;
						SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
					}
				}
				switch (userInput) {
				case 1:
					cout << "\nCurrrent vehicle name: " << vv.at(pos).getName();
					nvn = checkVnameFormat(3);//ask for user input and check name to see if its in the correct format
					x = 0;
					while (x < vv.size())
					{
						flag = 0;
						if (nvn == vv.at(x).getName())
						{
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
							cout << "Vehicle already exists!" << "\n\n Re-enter Vehicle name (eg. V01 : V36) : ";
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
							nvn = checkVnameFormat(3);//ask for user input and check name to see if its in the correct format
							flag = 1;
							x = 0;
						}
						if (flag == 0)
							x++;
					}
					cout << "\nVehicle Name: " << vv.at(pos).getName() << " is changed to " << nvn << " in vehicle list!" << endl;
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
					vv.at(pos).setName(nvn); //set the name at specified vector object
					break;
				case 2:
					cout << "\nCurrrent vehicle Capacity: " << vv[pos].getCapacity();
					while (1) {
						cout << "\nEnter New vehicle capacity in number, small(2) , large(4), super(6): ";
						cin >> capStr;
						if (check_number(capStr) == true) //checking for integer input
						{
							cap = stoi(capStr);
							if (cap == 2 || cap == 4 || cap == 6)
								break;
							else
							{
								SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
								cout << "Invalid input!" << endl;
								SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
							}
						}
						else if (check_number(capStr) == false)
						{
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED );
							cout << "Please use number!" << endl;
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						}
					}
					cout << "\nVehicle Capacity: " << vv.at(pos).getCapacity() << " is changed to " << cap << " in vehicle list!" << endl;
					vv.at(pos).setCapacity(cap); //set cap at given position
					break;
				case 3:
					cout << "\nCurrrent vehicle Destination: " << vv.at(pos).getDestination();
					cout << "\nAvailable Destination: " << "\nAljunied\n" << "Bedok\n" << "Bishan\n" << endl;
					dest = checkDest(1); //check if destination user gave is valid
					cout << "\nVehicle Destination: " << vv.at(pos).getDestination() << " is changed to " << dest << " in vehicle list!" << endl;
					vv.at(pos).setDestination(dest); //set destination at given position
					cin.ignore();
					break;
				case 4:
					if (vv.at(pos).getHour() > 10 && vv.at(pos).getMin() < 10) //If else statements are used to print 0s at the right place for time
						cout << "\nCurrent vehicle Time:" << right << setw(2) << vv.at(pos).getHour() << ":" << right << setw(2) << "0" << vv.at(pos).getMin() << left << setw(10) << setfill(' ') << ' ';
					else if (vv.at(pos).getHour() > 10 && vv.at(pos).getMin() > 10)
						cout << "\nCurrent vehicle Time:" << right << setw(2) << vv.at(pos).getHour() << ":" << right << setw(2) << vv.at(pos).getMin() << left << setw(10) << setfill(' ') << ' ';
					else if (vv.at(pos).getHour() < 10 && vv.at(pos).getMin() > 10)
						cout << "\nCurrent vehicle Time:" << right << setw(2) << "0" << vv.at(pos).getHour() << ":" << right << setw(2) << vv.at(pos).getMin() << left << setw(10) << setfill(' ') << ' ';
					else if (vv.at(pos).getHour() < 10 && vv.at(pos).getMin() < 10)
						cout << "\nCurrent vehicle Time:" << right << setw(2) << "0" << vv.at(pos).getHour() << ":" << right << setw(2) << "0" << vv.at(pos).getMin() << left << setw(10) << setfill(' ') << ' ';
					while (1) {
						cout << "\nEnter New Vehicle time in hh:mm : ";
						cin >> vt;
						if (setTime(vt, hour, min) == true) //check time format
							break;
					}

					vv.at(pos).setHour(hour); //set hour at given position
					vv.at(pos).setMin(min);   //set min at given position
					break;
				case 5:
					c = 1; //to be used for going back to prev menu (modify menu)
					break;
				case 6:
					c = 2; //to be used for going back to main menu
					break;

				default:
					cout << "invalid input" << endl;
				}
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << "\n========================================================================\n" << left << setw(15) << "Vehicle ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << left << setw(15) << "Capacity" << endl;
				vv.at(pos).displayLogistic();
				cout << left << setw(15) << vv.at(pos).getCapacity() << "\n========================================================================" << endl;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
				if (c == 1 || c == 2) //if either or break from nested while loop
					break;
			}

		}
		if (c == 1 || c == 2) //break again from outter while loop
			break;
	}
	return c; //to be used in tandem with changeMenuModify 
}
//modify menu for Commuter (*******Comments are the same as vehicle portion of modifying***********) //
int addCommuter(vector<Commuter>& vc) { //
	system("CLS");
	string cn, dest, ct, capStr, uiStr;
	int hr, min, userInput, cap = 0, flag;
	size_t x = 0;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Add Commuter: " << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	cn = checkCnameFormat(0);//check name function

	while (x < vc.size())
	{
		flag = 0;
		if (cn == vc.at(x).getName())
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Commuter already exists!\n\n";
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			cn = checkCnameFormat(0);
			flag = 1;
			x = 0;
		}
		if (flag == 0)
			x++;
	}
	cout << "\nAvailable Destination: " << "\nAljunied\n" << "Bedok\n" << "Bishan\n" << endl;
	dest = checkDest(0);

	while (1) {
		cout << "\nEnter Commuter time in hh:mm : ";
		cin >> ct;
		if (setTime(ct, hr, min) == true)
			break;
	}

	Commuter temp;
	temp.setName(cn);
	temp.setDestination(dest);
	temp.setHour(hr);
	temp.setMin(min);
	vc.push_back(temp);

	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\nSuccesfully added Vehicle!" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "\n========================================================================\n" << left << setw(15) << "Commuter ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << endl;
	vc.back().displayLogistic();
	cout << "\n========================================================================" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	return userInput = changeMenuEdit();
}
int deleteCommuter(vector<Commuter>& vc) { //
	displayAllCommuter(vc);
	string name, uiStr;
	int userInput;
	bool non_existant = true;

	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Delete Commuter: " << "\n-----------------------------------------------------------------------" << endl;
	SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);

	name = checkCnameFormat(1);//check name to see if its in the correct format);

	for (size_t x = 0; x < vc.size(); x++)
	{
		if (vc.at(x).getName() == name) {
			non_existant = false;
			system("CLS");
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "\Commuter Found" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);



			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "\Commuter Name: " << name << " deleted from the current commuter list!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
			cout << "\n========================================================================\n" << left << setw(15) << "Commuter ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << endl;
			vc.at(x).displayLogistic();
			cout << "\n========================================================================" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			vc.erase(vc.begin() + x); break;
		}
	}
	if (non_existant) {
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "\Commuter was not found. ";
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}

	return userInput = changeMenuEdit();
}
int editCommuter(vector<Commuter>& vc) { 
	system("CLS");
	string dest, cn, ncn, ct, capStr, uiStr;
	int hour = 0, min = 0, cap = 0, c = 0, pos, userInput, flag;
	size_t x = 0;
	bool non_existant = true;

	cout << "Edit Commuter: " << endl;

	while (1) {
		cn = checkCnameFormat(2);//check name to see if its in the correct format
		for (size_t x = 0; x < vc.size(); x++)
		{
			if (vc.at(x).getName() == cn) {
				cout << "Commuter found!" << endl;
				non_existant = false;
				pos = x;
				break;
			}
		}
		if (non_existant)
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Commuter not found! Try again" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
		else if (non_existant == false)
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "\n========================================================================\n" << left << setw(15) << "Vehicle ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << endl;
			vc.at(pos).displayLogistic();
			cout << "\n========================================================================" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
			while (1) {
				c = 0;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << "Edit Commuter Items: " << endl;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				while (1) {
					cout << "-----------------------------------------------------------------------" << "\n1. Commuter Name" << "\n2. Destination";
					cout << "\n3. Recharging Time" << "\n4. Back to previous menu" << "\n5. Back to Main menu" << endl;
					cout << "\n Please enter your option: ";
					cin >> uiStr;

					if (check_number(uiStr) == true)
					{
						userInput = stoi(uiStr);
						if (userInput == 1 || userInput == 2 || userInput == 3 || userInput == 4 || userInput == 5)
							break;
						else {
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
							cout << "invalid input, try again!" << endl;
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						}
					}
					else if (check_number(uiStr) == false)
					{
						SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
						cout << "Please use number!" << endl;
						SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
					}
				}
				switch (userInput) {
				case 1:
					cout << "\nCurrent Commuter ID: " << vc.at(pos).getName();
					ncn = checkCnameFormat(3);//check name to see if its in the correct format
					x = 0;
					while (x < vc.size())
					{
						flag = 0;
						if (ncn == vc.at(x).getName())
						{
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
							cout << "Commuter already exists!\n\n";
							SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
							ncn = checkCnameFormat(3);//check name to see if its in the correct format
							flag = 1;
							x = 0;
						}
						if (flag == 0)
							x++;
					}
					cout << "\nVehicle Name: " << vc.at(pos).getName() << " is changed to " << ncn << " in vehicle list!" << endl;
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
					vc.at(pos).setName(ncn);
					break;
				case 2:
					cout << "\nCurrent Commuter Destination: " << vc.at(pos).getDestination();
					cout << "\nAvailable Destination: " << "\nAljunied\n" << "Bedok\n" << "Bishan\n" << endl;
					dest = checkDest(0);
					cout << "\Commuter Destination: " << vc.at(pos).getDestination() << " is changed to " << dest << " in Commuter list!" << endl;
					vc.at(pos).setDestination(dest);
					cin.ignore();
					break;
				case 3:
					if (vc.at(pos).getHour() > 10 && vc.at(pos).getMin() < 10)
						cout << "\nCurrent Commuter Time:" << right << setw(2) << vc.at(pos).getHour() << ":" << right << setw(2) << "0" << vc.at(pos).getMin() << left << setw(10) << setfill(' ') << ' ';
					else if (vc.at(pos).getHour() > 10 && vc.at(pos).getMin() > 10)
						cout << "\nCurrent Commuter Time:" << right << setw(2) << vc.at(pos).getHour() << ":" << right << setw(2) << vc.at(pos).getMin() << left << setw(10) << setfill(' ') << ' ';
					else if (vc.at(pos).getHour() < 10 && vc.at(pos).getMin() > 10)
						cout << "\nCurrent Commuter Time:" << right << setw(2) << "0" << vc.at(pos).getHour() << ":" << right << setw(2) << vc.at(pos).getMin() << left << setw(10) << setfill(' ') << ' ';
					else if (vc.at(pos).getHour() < 10 && vc.at(pos).getMin() < 10)
						cout << "\nCurrent Commuter Time:" << right << setw(2) << "0" << vc.at(pos).getHour() << ":" << right << setw(2) << "0" << vc.at(pos).getMin() << left << setw(10) << setfill(' ') << ' ';

					while (1) {
						cout << "\nEnter New Commuter time in hh:mm : ";
						cin >> ct;
						if (setTime(ct, hour, min) == true)
							break;
					}
					vc.at(pos).setHour(hour);
					vc.at(pos).setMin(min);
					break;
				case 4:
					c = 1;
					break;
				case 5:
					c = 2;
					break;
				default:
				{
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
					cout << "invalid input" << endl;
					SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				}
				}
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << "\n========================================================================\n" << left << setw(15) << "Commuter ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << endl;
				vc.at(pos).displayLogistic();
				cout << "\n========================================================================" << endl;
				SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
				if (c == 1 || c == 2)
					break;
			}

		}
		if (c == 1 || c == 2)
			break;
	}
	return c;
}

//****************************Sorting functions************************************//
/**************************************************************************************
  User-Customizable Comparison Function for Vehicle List

  1. If destinationis different, sort in ascending destination (in alphabetical order)
  2. If destination is same, sort in descending capacity (Large to Small)

***************************************************************************************/
bool compVehDestDescendCap(Vehicle v1, Vehicle v2)
{
	if (v1.getDestination() != v2.getDestination())
		return (v1.getDestination() < v2.getDestination());
	return (v1.getCapacity() > v2.getCapacity());
}
/**************************************************************************************
  User-Customizable Comparison Function for Commuter List

  1. If destinationis different, sort in ascending destination (in alphabetical order)
  2. If destination is same, sort in ascending time (Small to Large)

***************************************************************************************/
bool compComDestTime(Commuter c1, Commuter c2) {
	if (c1.getDestination() != c2.getDestination())
		return (c1.getDestination() < c2.getDestination());
	return (c1.getTime() < c2.getTime());
}
/**************************************************************************************
  User-Customizable Comparison Function for Vehicle List

  1. If destinationis different, sort in ascending destination (in alphabetical order)
  2. If destination is same, sort in ascending time (Small to Large)

***************************************************************************************/
bool compVehDestAscTime(Vehicle v1, Vehicle v2) {
	if (v1.getDestination() != v2.getDestination())
		return (v1.getDestination() < v2.getDestination());
	return (v1.getTime() < v2.getTime());
}

//******************additional features*********************// 
void searchVehicle(vector <Vehicle>& vv) 
{
	//similar to edit portion
	system("CLS");
	string  vn;
	int pos;
	bool non_existant = true;
	cout << "Search Vehicle: " << endl;
	
	vn = checkVnameFormat(4);//check name to see if its in the correct format
	
	for (size_t x = 0; x < vv.size(); x++)
	{
		if (vv.at(x).getName() == vn) {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "\nVehicle found!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			non_existant = false;
			pos = x;
			break;
		}
	}
	if (non_existant) {
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "\nVehicle not found!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	else if (non_existant == false)
	{
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "\n========================================================================\n" << left << setw(15) << "Vehicle ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << left << setw(15) << "Capacity" << endl;
		vv.at(pos).displayLogistic();
		cout << left << setw(15) << vv.at(pos).getCapacity() << "\n========================================================================\n" << endl;
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
	}
}
void searchComm(vector <Commuter>& vc) 
{
	//similar to commuter edit portion
	system("CLS");
	string  vn;
	int pos;
	bool non_existant = true;
	cout << "Search Commuter: " << endl;
	vn = checkCnameFormat(4);
	
	for (size_t x = 0; x < vc.size(); x++)
	{
		if (vc.at(x).getName() == vn) {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout << "\Commuter found!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			non_existant = false;
			pos = x;
			break;
		}
	}
	if (non_existant) {
		    SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "\Commuter not found!" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	else if (non_existant == false)
	{
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "\n========================================================================\n" << left << setw(15) << "Commuter ID" << left << setw(15) << "Destination" << left << setw(15) << "Time" << endl;
		vc.at(pos).displayLogistic();
		cout << "\n========================================================================\n" << endl;
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}
}
void saveScheduleCSV(vector<Schedule>& vs) { 
	system("CLS");
	ofstream file;

	cout << "Save Schedule Plan:" << endl;
	cout << "-----------------------------------------------------------------------" << endl;


	if (vs.size() == 0) {
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "\nSchedule  has not been planned" << endl;
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	else {
		file.open("Schedule.CSV");
		file << "ID, Vehicle, Destination ,Time, Capacity, Commuter, Destination, Time" << endl;

		if (file) {
			for (auto& v : vs) { //Commas are placed between every variable as CSV files uses comma delimiters to seperate cells
				file << v.getSchId() << "," << v.getVehicle().getName() << "," << v.getVehicle().getDestination() << "," << v.getVehicle().getHour() << ":" << v.getVehicle().getMin() << "," << v.getVehicle().getCapacity() << "," << v.getCommuter().getName() << "," << v.getCommuter().getDestination() << "," << v.getCommuter().getHour() << ":" << v.getCommuter().getMin() << "," << endl;

			}
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE );
			cout << "\nSuccesfully saved file : " << "Schedule.csv" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);

		}
		else
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "\nError Opening Schedule.txt File" << endl;
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
	file.close();
}

//****************For Error checking and filtering of results (VALIDATION CHECK)********************// 
string checkVnameFormat(int i) //
{
	string vn;

	while (1) {
		//i is used to toggle between different cout for different cases of the program
		if (i == 0)
			cout << "\nVehicle name (eg. V01 : V36) : ";
		else if (i == 1)
			cout << "\nPlease enter the Vehicle name to be deleted (eg. V01 : V36) : ";
		else if (i == 2)
			cout << "\nPlease enter the Vehicle name to be edited (eg. V01 to V36) : ";
		else if (i == 3)
			cout << "\nEnter New vehicle name: ";
		else if (i == 4)
			cout << "\nSearch Vehicle (eg. V01 to V36) : ";
		cin >> vn;
		std::transform(vn.begin(), vn.end(), vn.begin(), ::toupper); //Transform string to upper case for better overall user Experience as its not case sensitive

		regex check{ R"(([V]{1}[0-9]{2}))" }; //regular expression format 
		smatch match;
		if (regex_match(vn, match, check)) {  //checking if given user input matches the format
			break;                            //if match break from while loop
		}
		else {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Invalid input, try again!" << "\n"; //if not try again
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
	return vn;
}
string checkCnameFormat(int i) //
{
	string vn;

	while (1) {
		//i is used to toggle between different cout for different cases of the program
		if (i == 0)
			cout << "\Commuter name (eg. C01 : C36) : ";
		else if (i == 1)
			cout << "\nPlease enter the Commuter name to be deleted (eg. C01 : C36) : ";
		else if (i == 2)
			cout << "\nPlease enter the Commuter name to be edited (eg. C01 to C36) : ";
		else if (i == 3)
			cout << "\nEnter New Commuter name: ";
		else if (i == 4)
			cout << "\nSearch Commuter (eg. C01 to C36) : ";

		cin >> vn;
		std::transform(vn.begin(), vn.end(), vn.begin(), ::toupper); //Transform string to upper case for better overall user Experience as its not case sensitive

		regex check{ R"(([C]{1}[0-9]{2}))" }; //format of string
		smatch match;
		if (regex_match(vn, match, check)) { //checking for format
			break;
		}
		else {
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Invalid input, try again!" << "\n";
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
	return vn;
}
string checkDest(int i) //
{
	string dest;
	while (1) {

		if (i == 1)
			cout << "\nEnter Vehicle destination in string : ";
		if (i == 0)
			cout << "\nEnter Commuter destination in string : ";
		cin >> dest;
		std::transform(dest.begin(), dest.end(), dest.begin(), ::toupper); //Transform string to upper case for better overall user Experience as its not case sensitive

		if (dest.compare("ALJUNIED") == 0 || dest.compare("BEDOK") == 0 || dest.compare("BISHAN") == 0) //checking if user input matches any of these strings
			break;
		else
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "invalid input!";
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		}
	}
	return dest;
}
bool setTime(string& enteredTime, int& hour, int& min) { 

	string row[3], word;
	size_t i = 0;

	for (int j = 0; enteredTime.find(":") != 2; cin >> enteredTime) { //checking for format of user input for time
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Invalid format!" << endl;
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "\nPlease enter in the format HH:MM  : ";

		i = 0;
	}

	stringstream time(enteredTime);	//repeat to split time at row[3] with ':' delimiter
	while (getline(time, word, ':')) {
		row[i] = word;
		if (check_number(word) == false) // checking if the input given by user is NUMBER only as time has no letters
		{
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Invalid format! Ensure there are no letters!";
			SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			return false;
		}
		i++;
	}
	hour = stoi(row[0]);
	min = stoi(row[1]);
	if ((hour >= 0 && hour < 24) && (min >= 0 && min < 60)) //to ensure that the time makes sense in a 24HR format
		return true;
	else
	{
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "invalid format! Ensure its 24hrs!";
		SetConsoleTextAttribute(hout, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		return false;
	}
}
bool check_number(string str) { 
	for (int i = 0; i < str.length(); i++)
		if (isdigit(str[i]) == false) //check if string is integer
			return false;
	return true;
}