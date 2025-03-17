#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory>

// Copilot intergration in Visual Studio was used in writing this program

// Base class: Employee
class Employee
{
protected:
    std::string name;
    int id;

public:
    Employee(std::string name, int id) : name(name), id(id) {}

    // Pure virtual function for salary calculation
    virtual double calculateSalary() const = 0;

    // Virtual function to display employee information
    virtual void displayInfo() const
    {
        std::cout << "ID: " << id << ", Name: " << name;
    }

    // Virtual destructor to ensure proper cleanup
    virtual ~Employee() {}
};

// Derived class: SalariedEmployee
class SalariedEmployee : public Employee
{
private:
    double monthlySalary;

public:
    SalariedEmployee(std::string name, int id, double monthlySalary)
        : Employee(name, id), monthlySalary(monthlySalary) {
    }

    double calculateSalary() const override
    {
        return monthlySalary;
    }

    void displayInfo() const override
    {
        Employee::displayInfo();
        std::cout << ", Type: Salaried, Monthly Salary: $" << std::fixed << std::setprecision(2) << monthlySalary << std::endl;
    }
};

// Derived class: HourlyEmployee
class HourlyEmployee : public Employee
{
private:
    double hourlyRate;
    int hoursWorked;

public:
    HourlyEmployee(std::string name, int id, double hourlyRate, int hoursWorked)
        : Employee(name, id), hourlyRate(hourlyRate), hoursWorked(hoursWorked) {
    }

    double calculateSalary() const override
    {
        return hourlyRate * hoursWorked;
    }

    void displayInfo() const override
    {
        Employee::displayInfo();
        std::cout << ", Type: Hourly, Hours Worked: " << hoursWorked
            << ", Hourly Rate: $" << std::fixed << std::setprecision(2) << hourlyRate
            << ", Salary: $" << calculateSalary() << std::endl;
    }
};

// Derived class: CommissionEmployee
class CommissionEmployee : public Employee
{
private:
    double baseSalary;
    double salesAmount;
    double commissionRate;

public:
    CommissionEmployee(std::string name, int id, double baseSalary, double salesAmount, double commissionRate)
        : Employee(name, id), baseSalary(baseSalary), salesAmount(salesAmount), commissionRate(commissionRate) {
    }

    double calculateSalary() const override
    {
        return baseSalary + (salesAmount * commissionRate);
    }

    void displayInfo() const override
    {
        Employee::displayInfo();
        std::cout << ", Type: Commission, Base Salary: $" << std::fixed << std::setprecision(2) << baseSalary
            << ", Sales: $" << salesAmount
            << ", Commission Rate: " << std::setprecision(0) << (commissionRate * 100) << "%"
            << ", Salary: $" << std::fixed << std::setprecision(2) << calculateSalary() << std::endl;
    }
};

// Function to read employees from file and store them in a vector
std::vector<Employee*> readEmployeesFromFile(const std::string& filename)
{
    std::vector<Employee*> employees;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return employees;
    }

    std::string type, name;
    int id;
    double salaryDetails1, salaryDetails2, salaryDetails3;

    while (file >> type >> id >> name)
    {
        if (type == "Salaried")
        {
            file >> salaryDetails1;
            employees.push_back(new SalariedEmployee(name, id, salaryDetails1));
        }
        else if (type == "Hourly")
        {
            file >> salaryDetails1 >> salaryDetails2;
            employees.push_back(new HourlyEmployee(name, id, salaryDetails1, salaryDetails2));
        }
        else if (type == "Commission")
        {
            file >> salaryDetails1 >> salaryDetails2 >> salaryDetails3;
            employees.push_back(new CommissionEmployee(name, id, salaryDetails1, salaryDetails2, salaryDetails3));
        }
        else
        {
            std::cerr << "Error: Unknown employee type '" << type << "' in file." << std::endl;
        }
    }

    file.close();
    return employees;
}

// Function to display all employees and their salaries
void displayEmployees(const std::vector<Employee*>& employees)
{
    for (const auto& employee : employees)
    {
        employee->displayInfo();
    }
}

// Function to clean up dynamically allocated memory
void cleanupEmployees(std::vector<Employee*>& employees)
{
    for (auto& employee : employees)
    {
        delete employee;
    }
    employees.clear();
}

int main()
{
    // Read employees from file
    std::vector<Employee*> employees = readEmployeesFromFile("employees.txt");

    // Display employee information and salaries
    displayEmployees(employees);

    // Clean up dynamically allocated memory
    cleanupEmployees(employees);

    return 0;
}