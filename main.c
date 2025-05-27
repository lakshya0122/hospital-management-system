#include <stdio.h>
#include "vehicle_management.h"

int main() {
    loadFromFile();

    if (!authenticateUser()) {
        printf("Access denied!\n");
        return 1;
    }

    while (1) {
        printf("\n1. Add Vehicle\n2. Search\n3. Sort\n4. Delete\n5. Report\n6. Exit\nChoice: ");
        int choice; scanf("%d", &choice); getchar();

        switch (choice) {
            case 1: addVehicle(); break;
            case 2: searchVehicles(); break;
            case 3: sortRecords(); break;
            case 4: deleteVehicle(); break;
            case 5: generateReport(); break;
            case 6: saveToFile(); return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
