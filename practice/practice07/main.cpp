#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory>

// Base class: Emplyee
class Employee 
{
protected:
	std::string name;
	int id;
	
public:
	Employee(std::string name, int id) : name(name), id(id) {}

	// Pure virtual function for salary calculation
	virtual double calculateSalary() const = 0;

	// Function to display employee details
	virtual void display() const 
	{
		std::cout << "ID: " << id << ", Name: "<< name;
	}
	// Virtual destructor to ensure proper cleanup of derived classes
	virtual ~Employee() {}
};

// Derived class: SalariedEmployee
class SalariedEmployee : public Employee
{
private:
	double monthlySalary;

public:
