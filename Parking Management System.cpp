 #include <iostream>   // for input and output (cin, cout)
#include <fstream>    // for file handling (save and load data)
#include <iomanip>    // for formatting output table (setw, left)
#include <ctime>      // for getting current time (entry, exit)
#include <string>     // for handling strings like regNo, ownerName
using namespace std;

const int MAX_SLOTS = 10;   // total slots in parking
const int CHARGE_PER_HOUR = 50;  // parking charge rate

// Structure to represent each parking slot
struct Slot {
    int slotNo;      // slot number
    bool isFree;     // true if free, false if occupied
};

// Vehicle class to store all details of a parked vehicle
class Vehicle {
public:
    string regNo;        // registration number
    string type;         // vehicle type (Car/Bike)
    string ownerName;    // owner's name
    int slotNo;          // assigned slot number
    time_t entryTime;    // time when parked

    Vehicle() {          // default constructor
        regNo = "";

        type = "";
        ownerName = "";
        slotNo = -1;
        entryTime = 0;
    }
};

// global arrays
Slot slots[MAX_SLOTS];       // parking slots array
Vehicle vehicles[MAX_SLOTS]; // vehicles array
int vehicleCount = 0;        // total parked vehicles

// function to initialize slots
void initSlots() {
    for (int i = 0; i < MAX_SLOTS; i++) {
        slots[i].slotNo = i + 1;
        slots[i].isFree = true;  // initially all free
    }
}

// function to save vehicle data into file
void saveData() {
    ofstream file("parking.txt");
    for (int i = 0; i < vehicleCount; i++) {
        file << vehicles[i].regNo << " "
             << vehicles[i].type << " "
             << vehicles[i].ownerName << " "
             << vehicles[i].slotNo << " "
             << vehicles[i].entryTime << endl;
    }
    file.close();
}

// function to load vehicle data from file
void loadData() {
    ifstream file("parking.txt");
    vehicleCount = 0;
    while (file >> vehicles[vehicleCount].regNo
                >> vehicles[vehicleCount].type
                >> vehicles[vehicleCount].ownerName
                >> vehicles[vehicleCount].slotNo
                >> vehicles[vehicleCount].entryTime)
    {
        slots[vehicles[vehicleCount].slotNo - 1].isFree = false;
        vehicleCount++;
    }
    file.close();
}

// function to park a vehicle
void parkVehicle() {
    if (vehicleCount >= MAX_SLOTS) {
        cout << "No free slots available!" << endl;
        return;
    }

    string regNo, type, owner;
    cout << "Enter Registration Number: ";
    cin >> regNo;
    cout << "Enter Vehicle Type (Car/Bike): ";
    cin >> type;
    cout << "Enter Owner Name: ";
    cin >> owner;

    // find first free slot
    int slotIndex = -1;
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (slots[i].isFree) {
            slotIndex = i;
            break;
        }
    }

    if (slotIndex == -1) {
        cout << "No free slots available!" << endl;
        return;
    }

    // assign values to vehicle object
    Vehicle v;
    v.regNo = regNo;
    v.type = type;
    v.ownerName = owner;
    v.slotNo = slotIndex + 1;
    v.entryTime = time(0);

    // update arrays
    vehicles[vehicleCount] = v;
    slots[slotIndex].isFree = false;
    vehicleCount++;

    cout << "Vehicle Parked in Slot " << v.slotNo << "!" << endl;

    saveData();
}

// function to remove a vehicle
void removeVehicle() {
    string regNo;
    cout << "Enter Registration Number to remove: ";
    cin >> regNo;

    int index = -1;
    for (int i = 0; i < vehicleCount; i++) {
        if (vehicles[i].regNo == regNo) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Vehicle not found!" << endl;
        return;
    }

    // calculate duration
    time_t exitTime = time(0);
    double hours = difftime(exitTime, vehicles[index].entryTime) / 3600.0;
    if (hours < 1) hours = 1; // minimum 1 hour
    double charge = hours * CHARGE_PER_HOUR;

    cout << "Vehicle Removed:\n";
    cout << "Reg: " << vehicles[index].regNo
         << ", Type: " << vehicles[index].type
         << ", Owner: " << vehicles[index].ownerName << endl;
    cout << "Total Hours: " << (int)hours << endl;
    cout << "Parking Charge: " << charge << " BDT\n";

    // free the slot
    slots[vehicles[index].slotNo - 1].isFree = true;

    // shift vehicles array left
    for (int i = index; i < vehicleCount - 1; i++) {
        vehicles[i] = vehicles[i + 1];
    }
    vehicleCount--;

    saveData();
}

// function to show parking status
void showStatus() {
    cout << left << setw(12) << "RegNo"
         << setw(10) << "Type"
         << setw(15) << "Owner"
         << "Entry Time\n";
    cout << "-----------------------------------------------------------\n";
    for (int i = 0; i < vehicleCount; i++) {
        cout << left << setw(12) << vehicles[i].regNo
             << setw(10) << vehicles[i].type
             << setw(15) << vehicles[i].ownerName
             << ctime(&vehicles[i].entryTime);
    }
    cout << "-----------------------------------------------------------\n";
    cout << "Free Slots: " << (MAX_SLOTS - vehicleCount) << endl;
}

// main function
int main() {
    initSlots();   // set all slots free initially
    loadData();    // load any old data from file

    int choice;
    while (true) {
        cout << "\n====== Parking Management System ======\n";
        cout << "1. Park Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. Show Parking Status\n";
        cout << "4. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) parkVehicle();
        else if (choice == 2) removeVehicle();
        else if (choice == 3) showStatus();
        else if (choice == 4) {
            cout << "Exiting... Data Saved.\n";
            break;
        } else {
            cout << "Invalid option!\n";
        }
    }
    return 0;
}
