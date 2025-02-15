#include <iostream>
#include <list>
#include <string>

// Function to display menu options
void displayMenu() {
    std::cout << "1. Add text\n";
    std::cout << "2. Remove text\n";
    std::cout << "3. Show content\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    std::list<std::string> tasks;
    int choice;
    std::string task;

    do {
        displayMenu();
        std::cin >> choice;

        if (choice == 1) {
            std::cin.ignore(); // to ignore the newline character left in the buffer
            std::cout << "Enter text: ";
            std::getline(std::cin, task);
            tasks.push_back(task);
        }
        else if (choice == 2) {
            std::cin.ignore(); // to ignore the newline character left in the buffer
            std::cout << "Enter tect to remove: ";
            std::getline(std::cin, task);
            tasks.remove(task);
        }
        else if (choice == 3) {
            std::cout << "Show content: ";
            for (const auto& t : tasks) {
                std::cout << t << " ";
            }
            std::cout << "\n";
        }
        else if (choice == 4) {
            std::cout << "Exiting...\n";
        }
        else {
            std::cout << "Invalid option.\n";
        }
    } while (choice != 4);

    return 0;
}