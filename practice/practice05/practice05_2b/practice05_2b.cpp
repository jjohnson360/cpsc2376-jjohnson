#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main() {
    std::vector<int> numbers = { 15, 8, 22, 3, 17, 9, 25, 42, 11, 36,
                                7, 19, 28, 5, 31, 14, 20, 4, 29, 12 };

    if (numbers.empty())
    {
        std::cout << "Vector is empty." << std::endl;
        return 0;
    }

    // Find the largest element
    auto maxIt = std::max_element(numbers.begin(), numbers.end());
    std::cout << "Largest element: " << *maxIt << std::endl;

    // Find the smallest element
    auto minIt = std::min_element(numbers.begin(), numbers.end());
    std::cout << "Smallest element: " << *minIt << std::endl;

    return 0;
}