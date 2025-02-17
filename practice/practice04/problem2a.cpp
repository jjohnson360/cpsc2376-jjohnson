#include <iostream>
#include <iomanip>
#include <string>

// Template function to perform basic arithmetic operations
template<typename T>
T calculate(T num1, T num2, char op) 
{
    switch (op) 
    {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    case '/': return num2 != 0 ? num1 / num2 : 0;
    default: return 0;
    }
}

int main() {
    std::string input1, input2;
    char op;

    std::cout << "Enter first number: ";
    std::cin >> input1;
    std::cout << "Enter second number: ";
    std::cin >> input2;
    std::cout << "Enter operation (+, -, *, /): ";
    std::cin >> op;

    if (input1.find('.') != std::string::npos || input2.find('.') != std::string::npos) 
    {
        double num1 = std::stod(input1);
        double num2 = std::stod(input2);
        std::cout << "Result: " << std::fixed << std::setprecision(2) << calculate(num1, num2, op) << "\n";
    }
    else 
    {
        int num1 = std::stoi(input1);
        int num2 = std::stoi(input2);
        std::cout << "Result: " << calculate(num1, num2, op) << "\n";
    }

    return 0;
}
