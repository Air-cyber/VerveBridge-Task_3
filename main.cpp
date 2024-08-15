#include <iostream>
#include <list>
#include <ctime>
#include <string>
#include <map>

// Class to represent a single parking space
class ParkingSpace {
private:
    int id;
    bool isOccupied;
    std::string vehicleNumber;
    time_t entryTime;

public:
    ParkingSpace(int id) : id(id), isOccupied(false) {}

    void occupySpace(const std::string& vehicleNumber) {
        this->vehicleNumber = vehicleNumber;
        entryTime = std::time(nullptr);
        isOccupied = true;
    }

    void freeSpace() {
        isOccupied = false;
        vehicleNumber = "";
    }

    double calculateFee(double ratePerHour) const {
        time_t currentTime = std::time(nullptr);
        double hours = difftime(currentTime, entryTime) / 3600.0;
        return hours * ratePerHour;
    }

    bool getStatus() const {
        return isOccupied;
    }

    int getId() const {
        return id;
    }

    void displayDetails() const {
        std::cout << "Space ID: " << id << " | Status: " 
                  << (isOccupied ? "Occupied" : "Available") << std::endl;
    }
};

// Class to represent the parking lot
class ParkingLot {
private:
    std::list<ParkingSpace> parkingSpaces;
    int totalSpaces;
    int availableSpaces;
    std::map<int, double> ratePerHour; // Rate per hour for different space types

public:
    ParkingLot(int numSpaces) : totalSpaces(numSpaces), availableSpaces(numSpaces) {
        for (int i = 1; i <= numSpaces; ++i) {
            parkingSpaces.push_back(ParkingSpace(i));
            ratePerHour[i] = 10.0; // Default rate: 10 units/hour
        }
    }

    void displayAvailableSpaces() const {
        for (const auto& space : parkingSpaces) {
            if (!space.getStatus()) {
                space.displayDetails();
            }
        }
    }

    void reserveSpace(const std::string& vehicleNumber) {
        for (auto& space : parkingSpaces) {
            if (!space.getStatus()) {
                space.occupySpace(vehicleNumber);
                availableSpaces--;
                std::cout << "Space " << space.getId() << " reserved for vehicle: " << vehicleNumber << std::endl;
                return;
            }
        }
        std::cout << "No available spaces!" << std::endl;
    }

    void releaseSpace(int spaceId) {
        for (auto& space : parkingSpaces) {
            if (space.getId() == spaceId && space.getStatus()) {
                double fee = space.calculateFee(ratePerHour[spaceId]);
                space.freeSpace();
                availableSpaces++;
                std::cout << "Space " << spaceId << " is now available. Parking fee: " << fee << " units." << std::endl;
                return;
            }
        }
        std::cout << "Invalid space ID or space is not occupied!" << std::endl;
    }

    void adminOptions() {
        int adminChoice;
        std::cout << "Admin Panel:\n1. View Parking Statistics\n2. Change Rate Per Hour\n3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> adminChoice;

        switch (adminChoice) {
        case 1:
            std::cout << "Total Spaces: " << totalSpaces << "\nAvailable Spaces: " << availableSpaces << std::endl;
            break;
        case 2: {
            int spaceId;
            double newRate;
            std::cout << "Enter space ID to change rate: ";
            std::cin >> spaceId;
            std::cout << "Enter new rate: ";
            std::cin >> newRate;
            if (ratePerHour.find(spaceId) != ratePerHour.end()) {
                ratePerHour[spaceId] = newRate;
                std::cout << "Rate updated successfully.\n";
            } else {
                std::cout << "Invalid space ID!\n";
            }
            break;
        }
        case 3:
            std::cout << "Exiting Admin Panel...\n";
            break;
        default:
            std::cout << "Invalid choice!\n";
        }
    }
};

int main() {
    ParkingLot lot(10);  // Initialize parking lot with 10 spaces

    int choice;
    do {
        std::cout << "\n1. View Available Spaces\n2. Reserve a Space\n3. Release a Space\n4. Admin Panel\n5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            lot.displayAvailableSpaces();
            break;
        case 2: {
            std::string vehicleNumber;
            std::cout << "Enter vehicle number: ";
            std::cin >> vehicleNumber;
            lot.reserveSpace(vehicleNumber);
            break;
        }
        case 3: {
            int spaceId;
            std::cout << "Enter space ID to release: ";
            std::cin >> spaceId;
            lot.releaseSpace(spaceId);
            break;
        }
        case 4:
            lot.adminOptions();
            break;
        case 5:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
