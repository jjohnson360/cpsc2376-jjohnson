#include <iostream>
#include <iomanip>

// Function to convert temperature between Celsius and Fahrenheit
double convertTemperature(double temp, char scale = 'F') {
    if (scale == 'F') 
    {
        return (temp * 9.0 / 5.0) + 32.0; // Celsius to Fahrenheit
    }
    else 
    {
        return (temp - 32.0) * 5.0 / 9.0; // Fahrenheit to Celsius
    }
}

int main() 
{
    int choice;
    double temp;

    std::cout << "1. Convert Celsius to Fahrenheit\n";
    std::cout << "2. Convert Fahrenheit to Celsius\n";
    std::cout << "Choose an option: ";
    std::cin >> choice;

    std::cout << "Enter temperature: ";
    std::cin >> temp;

    if (choice == 1) 
    {
        std::cout << "Converted: " << std::fixed << std::setprecision(2) << convertTemperature(temp, 'F') << "°F\n";
    }
    else if (choice == 2) 
    {
        std::cout << "Converted: " << std::fixed << std::setprecision(2) << convertTemperature(temp, 'C') << "°C\n";
    }
    else 
    {
        std::cout << "Invalid option.\n";
    }

    return 0;
}
