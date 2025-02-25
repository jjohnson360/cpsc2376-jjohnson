#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

int main()
{
    std::vector<int> numbers;
    std::string input;

    std::cout << "Enter a list of integers (enter 'end' to stop):" << std::endl;

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
        catch (const std::invalid_argument& e) {
            std::cout << "Invalid input. Please enter an integer or 'end'." << std::endl;
            continue;
        }
    }

    if (numbers.empty())
    {
        std::cout << "No numbers entered." << std::endl;
        return 0;
    }

    std::cout << "Squared elements:" << std::endl;
    std::for_each(numbers.begin(), numbers.end(), [](int n)
        {
            std::cout << n * n << " ";
        });
    std::cout << std::endl;

    int sumOfSquares = std::accumulate(numbers.begin(), numbers.end(), 0, [](int sum, int n)
        {
            return sum + n * n;
        });

    std::cout << "Sum of squared elements: " << sumOfSquares << std::endl;

    return 0;
}