#include <iostream>
#include "Fraction.h"
#include "MixedFraction.h"

void displayMenu() 
{
    std::cout << "Current Fraction: ";
    // Function to display the current fraction will be implemented here
    std::cout << "\n\nOptions:\n";
    std::cout << "1. Enter a fraction manually\n";
    std::cout << "2. Add a fraction\n";
    std::cout << "3. Subtract a fraction\n";
    std::cout << "4. Multiply by a fraction\n";
    std::cout << "5. Divide by a fraction\n";
    std::cout << "6. Display as Mixed Fraction\n";
    std::cout << "7. Clear Fraction\n";
    std::cout << "8. Exit\n";
}

void enterFraction(Fraction& frac) 
{
    int num, den;
    std::cout << "Enter numerator: ";
    std::cin >> num;
    std::cout << "Enter denominator: ";
    std::cin >> den;
    frac = Fraction(num, den);
}

int main() 
{
    Fraction currentFraction; // Initialize to 0/1

    while (true) 
    {
        displayMenu();
        int choice;
        std::cin >> choice;

        try {
            if (choice == 1) 
            {
                enterFraction(currentFraction);
            }
            else if (choice == 2) 
            {
                Fraction other;
                enterFraction(other);
                currentFraction = currentFraction + other;
            }
            else if (choice == 3) 
            {
                Fraction other;
                enterFraction(other);
                currentFraction = currentFraction - other;
            }
            else if (choice == 4) 
            {
                Fraction other;
                enterFraction(other);
                currentFraction = currentFraction * other;
            }
            else if (choice == 5) 
            {
                Fraction other;
                enterFraction(other);
                currentFraction = currentFraction / other;
            }
            else if (choice == 6) 
            {
                MixedFraction mf(currentFraction);
                std::cout << "Mixed Fraction: " << mf << std::endl;
            }
            else if (choice == 7) 
            {
                currentFraction = Fraction();
            }
            else if (choice == 8) 
            {
                return 0;
            }
            else 
            {
                std::cout << "Invalid option. Please try again.\n";
            }
        }
        catch (const std::exception& e) 
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}
