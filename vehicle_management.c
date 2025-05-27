#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "vehicle_management.h"

Vehicle vehicles[MAX_VEHICLES];
int vehicleCount = 0;

int authenticateUser() {
    char user[20], pass[20];
    printf("Username: "); scanf("%s", user);
    printf("Password: "); scanf("%s", pass);
    return strcmp(user, "admin") == 0 && strcmp(pass, "admin123") == 0;
}

int ci_compare(const char *a, const char *b) {
    while (*a && *b) if (tolower(*a++) != tolower(*b++)) return 0;
    return *a == *b;
}

void saveToFile() {
    FILE *f = fopen(FILENAME, "w");
    if (!f) { printf("Error saving file!\n"); return; }
    for (int i = 0; i < vehicleCount; i++)
        fprintf(f, "%s|%s|%s|%s|%d\n", vehicles[i].regNumber, vehicles[i].ownerName,
                vehicles[i].model, vehicles[i].type, vehicles[i].year);
    fclose(f);
}

void loadFromFile() {
    FILE *f = fopen(FILENAME, "r");
    if (!f) { printf("No data file found.\n"); return; }

    char line[256], *fields[5];
    while (fgets(line, sizeof(line), f) && vehicleCount < MAX_VEHICLES) {
        line[strcspn(line, "\n")] = 0;
        for (int i = 0; (fields[i] = strtok(i ? NULL : line, "|")) && i < 5; i++);
        if (fields[0] && fields[1] && fields[2] && fields[3] && fields[4]) {
            strcpy(vehicles[vehicleCount].regNumber, fields[0]);
            strcpy(vehicles[vehicleCount].ownerName, fields[1]);
            strcpy(vehicles[vehicleCount].model, fields[2]);
            strcpy(vehicles[vehicleCount].type, fields[3]);
            vehicles[vehicleCount++].year = atoi(fields[4]);
        }
    }
    fclose(f);
    printf("Loaded %d vehicles.\n", vehicleCount);
}

void addVehicle() {
    if (vehicleCount >= MAX_VEHICLES) { printf("Database full!\n"); return; }

    Vehicle v;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int current_year = tm.tm_year + 1900;

    printf("Enter reg number: "); scanf("%s", v.regNumber); getchar();
    printf("Enter owner name: "); fgets(v.ownerName, 50, stdin);
    v.ownerName[strcspn(v.ownerName, "\n")] = 0;
    printf("Enter model & type: "); scanf("%s %s", v.model, v.type);

    while (1) {
        printf("Enter year (1900-%d): ", current_year);
        scanf("%d", &v.year);
        if (v.year >= 1900 && v.year <= current_year) break;
        printf("Invalid year!\n");
    }

    vehicles[vehicleCount++] = v;
    saveToFile();
    printf("Vehicle added!\n");
}

void searchVehicles() {
    if (!vehicleCount) { printf("No vehicles!\n"); return; }

    char query[50];
    printf("Search (reg/name): ");
    getchar(); fgets(query, 50, stdin); query[strcspn(query, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < vehicleCount; i++) {
        if (strstr(vehicles[i].regNumber, query) || strstr(vehicles[i].ownerName, query)) {
            printf("\nVehicle %d:\nReg: %s\nOwner: %s\nModel: %s\nType: %s\nYear: %d\n",
                   i+1, vehicles[i].regNumber, vehicles[i].ownerName,
                   vehicles[i].model, vehicles[i].type, vehicles[i].year);
            found = 1;
        }
    }
    if (!found) printf("No matches found.\n");
}

void sortRecords() {
    if (!vehicleCount) { printf("No vehicles to sort!\n"); return; }

    printf("Sort by:\n1. Model\n2. Year\nChoice: ");
    int choice; scanf("%d", &choice);

    for (int i = 0; i < vehicleCount-1; i++) {
        for (int j = i+1; j < vehicleCount; j++) {
            int shouldSwap = 0;
            if (choice == 1) shouldSwap = strcmp(vehicles[i].model, vehicles[j].model) > 0;
            else if (choice == 2) shouldSwap = vehicles[i].year > vehicles[j].year;

            if (shouldSwap) {
                Vehicle temp = vehicles[i];
                vehicles[i] = vehicles[j];
                vehicles[j] = temp;
            }
        }
    }
    saveToFile();
    printf("Records sorted!\n");
}

void deleteVehicle() {
    if (!vehicleCount) { printf("No vehicles to delete!\n"); return; }

    char reg[20];
    printf("Enter reg number to delete: "); scanf("%s", reg);

    for (int i = 0; i < vehicleCount; i++) {
        if (strcmp(vehicles[i].regNumber, reg) == 0) {
            for (int j = i; j < vehicleCount-1; j++)
                vehicles[j] = vehicles[j+1];
            vehicleCount--;
            saveToFile();
            printf("Vehicle deleted!\n");
            return;
        }
    }
    printf("Vehicle not found!\n");
}

void generateReport() {
    if (!vehicleCount) { printf("No vehicles to report!\n"); return; }

    int car = 0, bike = 0, truck = 0, other = 0;
    for (int i = 0; i < vehicleCount; i++) {
        if (ci_compare(vehicles[i].type, "car")) car++;
        else if (ci_compare(vehicles[i].type, "bike")) bike++;
        else if (ci_compare(vehicles[i].type, "truck")) truck++;
        else other++;
    }

    FILE *f = fopen("vehicle_report.txt", "w");
    if (!f) { printf("Error creating report!\n"); return; }

    fprintf(f, "=== VEHICLE MANAGEMENT SYSTEM REPORT ===\n\n");
    fprintf(f, "Total Registered Vehicles: %d\n", vehicleCount);
    fprintf(f, "Cars: %d\nBikes: %d\nTrucks: %d\n", car, bike, truck);
    if (other) fprintf(f, "Other Types: %d\n", other);

    fprintf(f, "\nDETAILED LIST:\n");
    fprintf(f, "%-15s %-20s %-15s %-10s %s\n", "Reg Number", "Owner Name", "Model", "Type", "Year");
    fprintf(f, "------------------------------------------------------------\n");

    for (int i = 0; i < vehicleCount; i++) {
        fprintf(f, "%-15s %-20s %-15s %-10s %d\n",
               vehicles[i].regNumber, vehicles[i].ownerName,
               vehicles[i].model, vehicles[i].type, vehicles[i].year);
    }

    fclose(f);
    printf("Report generated in 'vehicle_report.txt'\n");
}
