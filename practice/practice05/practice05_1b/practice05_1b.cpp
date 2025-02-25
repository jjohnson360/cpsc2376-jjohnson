#include <iostream>
#include <vector>
#include <iterator>
#include <string>

int main() 
{
    std::vector<int> numbers;
    std::string input;

    std::cout << "Enter a list of numbers (type 'end' to stop):" << std::endl;

    while (true) 
    {
        std::cin >> input;
        if (input == "end") 
        {
            break;
        }
        try 
        {
            numbers.push_back(std::stoi(input));
        }
        catch (const std::invalid_argument& e) 
        {
            std::cout << "Invalid input. Please enter a number or 'end'." << std::endl;
            continue;
        }
    }

    if (numbers.empty()) 
    {
        std::cout << "No numbers entered." << std::endl;
        return 0;
    }

    std::cout << "Numbers in reverse order:" << std::endl;

    for (auto it = numbers.rbegin(); it != numbers.rend(); ++it) 
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}