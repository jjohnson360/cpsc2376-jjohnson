#include <iostream>
#include <string>

// Copilot intergration in Visual Studio was used in writing this program

int main()
{
    float redPotion = 0.0f;   // Amount of red potion in mL
    float bluePotion = 0.0f;  // Amount of blue potion in mL
    float* flask = nullptr;   // Pointer to the selected potion

    std::string potionChoice;

    while (true)
    {
        std::cout << "Which potion to add liquid to ('Red' or 'Blue' or 'Done'): ";
        std::getline(std::cin, potionChoice);

        if (potionChoice == "Done")
        {
            break;
        }

        if (potionChoice == "Red")
        {
            flask = &redPotion;
        }
        else if (potionChoice == "Blue")
        {
            flask = &bluePotion;
        }
        else
        {
            std::cout << "Invalid potion choice." << std::endl;
            continue; // Go back to the beginning of the loop
        }

        float amountToAdd;
        std::cout << "How many milliliters to add: ";
        std::cin >> amountToAdd;
        std::cin.ignore(); // Clear the newline character from the buffer

        if (flask)
        {
            *flask += amountToAdd;
        }

        std::cout << "Red Potion: " << redPotion << " mL" << std::endl;
        std::cout << "Blue Potion: " << bluePotion << " mL" << std::endl;
    }

    return 0;
}