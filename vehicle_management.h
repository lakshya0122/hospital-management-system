#ifndef VEHICLE_MANAGEMENT_H
#define VEHICLE_MANAGEMENT_H

#define MAX_VEHICLES 1000
#define FILENAME "vehicle.txt"

typedef struct {
    char regNumber[20], ownerName[50], model[30], type[20];
    int year;
} Vehicle;

extern Vehicle vehicles[MAX_VEHICLES];
extern int vehicleCount;

// Function declarations
int authenticateUser();
int ci_compare(const char *a, const char *b);
void saveToFile();
void loadFromFile();
void addVehicle();
void searchVehicles();
void sortRecords();
void deleteVehicle();
void generateReport();

#endif
