#include <iostream>
#include <list>
#include <string>

// Function to display menu options
void displayMenu() {
    std::cout << "1. Add a task\n";
    std::cout << "2. Remove a task\n";
    std::cout << "3. Show tasks\n";
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

        switch (choice) {
        case 1:
            std::cin.ignore(); // to ignore the newline character left in the buffer
            std::cout << "Enter task: ";
            std::getline(std::cin, task);
            tasks.push_back(task);
            break;
        case 2:
            std::cin.ignore(); // to ignore the newline character left in the buffer
            std::cout << "Enter task to remove: ";
            std::getline(std::cin, task);
            tasks.remove(task);
            break;
        case 3:
            std::cout << "Tasks:\n";
            for (const auto& t : tasks) {
                std::cout << "- " << t << "\n";
            }
            break;
        case 4:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid option.\n";
            break;
        }
    } while (choice != 4);

    return 0;
}
