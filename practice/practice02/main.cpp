#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

//Reads the balance from the file
double readBalanceFromFile(const std::string& filename)
{
	std::ifstream inFile(filename);
	if (!inFile.is_open())
	{
		std::ofstream outFile(filename);
		if (!outFile.is_open())
		{
			std::cerr << "Error: Unable create account file." << std::endl;
			exit(1);
		}
		outFile << std::fixed << std::setprecision(2) << 100.00;
		outFile.close();
		return 100.00;
	}

	double balance;
	inFile >> balance;
	inFile.close();
	return balance;
}

// Writes the balance to the file
void writeBalanceToFile(const std::string& filename, double balance)
{
	std::ofstream outFile(filename);
	if (!outFile.is_open())
	{
		std::cerr << "Error: Unable to create account file. " << std::endl;
		exit(1);
	}
	outFile << std::fixed << std::setprecision(2) << balance;
	outFile.close();
}
// Displays the current balance
void checkBalance(double balance)
{
	std::cout << "Current balance is: $" << std::fixed << std::setprecision(2) << balance << std::endl;
}

// Deposits money into the account
double deposit(double balance)
{
	double amount;
	std::cout << "Enter the amount to deposit: ";
	std::cin >> amount;

	if (std::cin.fail() || amount <= 0)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cerr << "Error: Inavild amount. Must be a positive number." << std::endl;
		return balance;
	}

	balance += amount;
	std::cout << "Deposit successful.New balance is: $" << std::fixed << std::setprecision(2) << balance << std::endl;
	return balance;
}
// Withdraws money from the account
double withdraw(double balance)
{
	double amount;
	std::cout << " Enter amount to withdraw: ";
	std::cin >> amount;

	if (std::cin.fail() || amount <= 0)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cerr << "Error: Invalid amount. Must be a positive number." << std::endl;
		return balance;
	}

	if (amount > balance)
	{
		std::cerr << "Error: Insufficient funds. Your Balance is $" << std::fixed << std::setprecision(2) << balance << "." << std::endl;
		return balance;
	}

	balance -= amount;
	std::cout << "Withdrawal successful. New balance is: $" << std::fixed << std::setprecision(2) << balance << std::endl;
	return balance;
}


int main()
{
	const std::string filename = "account_balance.txt";
	double balance = readBalanceFromFile(filename);

	std::cout << "Welcome to Your Bank Account!" << std::endl;

	if (balance == 100.00)
	{
		std::cout << "Intializing account with $100.00..." << std::endl;
	}
	else
	{
		std::cout << "Your current balance is: $" << std::fixed << std::setprecision(2) << balance << std::endl;
	}

	while (true)
	{
		std::cout << "\nBank Account Menu:\n";
		std::cout << "1. Check Balance\n";
		std::cout << "2. Deposit\n";
		std::cout << "3. Withdraw\n";
		std::cout << "4. Exit\n";
		std::cout << "Enter your choice (1-4): ";

		int choice;
		std::cin >> choice;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cerr << "Error: Invalid choice. Choose options 1-4." << std::endl;
			continue;
		}

		if (choice == 1)
		{
			checkBalance(balance);
		}
		else if (choice == 2)
		{
			balance = deposit(balance);
			writeBalanceToFile(filename, balance);
		}
		else if (choice == 3)
		{
			balance = withdraw(balance);
			writeBalanceToFile(filename, balance);
		}
		else if (choice == 4)
		{
			std::cout << "Thank you for banking with us, Goodbye..." << std::endl;
			writeBalanceToFile(filename, balance);
			break;
		}
		else
		{
			std::cerr << "Error: Invalid choice. Please try again." << std::endl;
		}
	}

	return 0;
}