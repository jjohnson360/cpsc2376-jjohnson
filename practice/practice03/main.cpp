// practice/practice03/practice03.cpp
#include <iostream>
#include <vector>
#include <stdexcept>

class MathUtils {
public:
    static int sumRange(int start, int end) {
        int sum = 0;
        for (int i = start; i <= end; i++) { // Fixed: Include end value
            sum += i;
        }
        return sum;
    }

    static bool containsNegative(const std::vector<int>& numbers) {
        for (size_t i = 0; i < numbers.size(); i++) { // Fixed: Correct loop condition
            if (numbers[i] < 0) { // Fixed: Correct condition to check for negative
                return true;
            }
        }
        return false;
    }

    static int findMax(const std::vector<int>& numbers) {
        if (numbers.empty()) {
            throw std::invalid_argument("Empty vector"); // Fixed: Throw exception for empty vector
        }
        int maxVal = numbers[0];
        for (size_t i = 1; i < numbers.size(); i++) { // Fixed: Correct loop condition
            if (numbers[i] > maxVal) { // Fixed: Correct condition
                maxVal = numbers[i];
            }
        }
        return maxVal;
    }
};

int main() {
    std::cout << "Sum from 1 to 5: " << MathUtils::sumRange(1, 5) << std::endl;
    std::vector<int> values = { 3, -1, 5, 7 };
    std::cout << "Contains negative? " << MathUtils::containsNegative(values) << std::endl;
    std::cout << "Max value: " << MathUtils::findMax(values) << std::endl;
    return 0;
}