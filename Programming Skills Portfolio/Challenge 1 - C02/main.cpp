#include <iostream>
#include <string>
#include <iomanip>

double getDistance() {
    double distance = 0.0;
    while (true) {
        std::cout << "Enter journey distance in miles: ";
        if (std::cin >> distance && distance >= 0) {
            return distance;
        }
        std::cout << "Invalid input. Please enter a positive number.\n";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
}

int getFuelType() {
    int choice = 0;
    while (true) {
        std::cout << "Select Fuel Type:\n";
        std::cout << "1. Diesel\n";
        std::cout << "2. Petrol\n";
        std::cout << "Enter choice (1 or 2): ";
        if (std::cin >> choice && (choice == 1 || choice == 2)) {
            return choice;
        }
        std::cout << "Invalid choice. Please enter 1 or 2.\n";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
}

double calculateEmissions(double distance, int fuelType) {
    const double litresPerHundredMiles = 9.66;
    const double dieselEmissionFactor = 2.68;
    const double petrolEmissionFactor = 2.31;

    double litresUsed = (distance / 100.0) * litresPerHundredMiles;
    double factor = (fuelType == 1) ? dieselEmissionFactor : petrolEmissionFactor;

    return litresUsed * factor;
}

int main() {
    std::cout << "=== Journey CO2 Emissions Calculator ===\n";

    double distance = getDistance();
    int fuelType = getFuelType();

    double totalEmissions = calculateEmissions(distance, fuelType);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nJourney Summary:\n";
    std::cout << "Distance: " << distance << " miles\n";
    std::cout << "Fuel: " << (fuelType == 1 ? "Diesel" : "Petrol") << "\n";
    std::cout << "Estimated CO2 Emissions: " << totalEmissions << " kg CO2\n";

    return 0;
}