#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// Structure to store items
struct Item
{
	std::string name;
	int quantity;
	double price;
};

// Function Prototypes
void printInventory(const::std::vector<Item>& inventory);
void addItem(std::vector<Item>& inventory);
void updateQuantity(std::vector<Item>& inventory);
void totalItems(const std::vector<Item>& inventory);
void updatePrice(std::vector<Item>& inventory);
void totalInventoryValue(const std::vector<Item>& inventory);
Item* findItem(std::vector<Item>& inventory, const std::string& itmeNmae);

int main()
{
	std::vector<Item> inventory;
	int choice;

	do 
	{
		// Inventory Menu
		std::cout << "\nInventory Menu:\n";
		std::cout << "1. Add New Item\n";
		std::cout << "2. Show Inventory List\n";
		std::cout << "3. Update Item Quantity\n";
		std::cout << "4. Update Item Price\n";
		std::cout << "5. Total Inventory Items\n";
		std::cout << "6. Total Value of Inventory\n";
		std::cout << "7. Exit\n";
		std::cout << "Enter Your choice: ";

		std::cin >> choice;
		std::cin.ignore();

		if (choice == 1) 
		{
			addItem(inventory);
		}
		else if (choice == 2)
		{
			printInventory(inventory);
		}
		else if (choice == 3)
		{
			updateQuantity(inventory);
		}
		else if (choice == 4)
		{
			updatePrice(inventory);
		}
		else if (choice == 5)
		{
			totalItems(inventory);
		}
		else if (choice == 6)
		{
			totalInventoryValue(inventory);
		}
		else if (choice == 7)
		{
			std::cout << "Exiting the program...\n";
		}
		else
		{
			std::cout << "Invalid choice. Please try again.\n";
		}

	} while (choice != 7);

	return 0;
}

// Function that prints the inventory
void printInventory(const std::vector<Item>& inventory)
{
	std::cout << "\nInventory List:\n";
	std::cout << std::setw(20) << std::left << "Item";
	std::cout << std::setw(10) << "Quantity";
	std::cout << std::setw(10) << "Price\n";
	std::cout << std::string(40, '-') << "\n";
	for (const auto& item : inventory) 
	{
		std::cout << std::setw(20) << std::left << item.name;
		std::cout << std::setw(10) << item.quantity;
		std::cout << "$" << std::fixed << std::setprecision(2) << item.price << '\n';
	}
}

// Function that adds an item to the inventory
void addItem(std::vector<Item>& inventory) 
{
	Item newItem;
	std::cout << "Enter Item Name: ";
	std::getline(std::cin, newItem.name);
	std::cout << "Enter Item Quantity: ";
	while (!(std::cin >> newItem.quantity) || newItem.quantity < 0)
	{
		std::cout << "Invalid quantity. Please try again: ";
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}
	std::cout << "Enter Item Price: ";
	while (!(std::cin >> newItem.price) || newItem.price < 0)
	{
		std::cout << "Invalid price. Please try again: ";
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}
	std::cin.ignore();
	inventory.push_back(newItem);
	std::cout << "Item added to inventory.\n";
}

// Function to find an item in the inventory
Item* findItem(std::vector<Item>& inventory, const std::string& itemName)
{
	for (auto& item : inventory)
	{
		if (item.name == itemName)
		{
			return &item;
		}
	}
	return nullptr;
}

// Function to update the quantity of an item
void updateQuantity(std::vector<Item>& inventory)
{
	std::string itemName;
	int newQuantity;
	std::cout << "Enter the name of the item: ";
	std::getline(std::cin, itemName);
	Item* item = findItem(inventory, itemName);
	if (item)
	{
		std::cout << "Enter the new quantity: ";
		while (!(std::cin >> newQuantity) || newQuantity < 0)
		{
			std::cout << "Invalid quantity. Please try again: ";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		item->quantity = newQuantity;
		std::cout << "Quantity updated.\n";
	}
	else
	{
		std::cout << "Item not found in inventory.\n";
	}
} 

// Function to update the price of an item
void updatePrice(std::vector<Item>& inventory)
{
	std::string itemName;
	double newPrice;
	std::cout << "Enter the name of the item: ";
	std::getline(std::cin, itemName);
	Item* item = findItem(inventory, itemName);
	if (item)
	{
		std::cout << "Enter the new price: ";
		while (!(std::cin >> newPrice) || newPrice < 0)
		{
			std::cout << "Invalid price. Please try again: ";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		item->price = newPrice;
		std::cout << "Price updated.\n";
	}
	else
	{
		std::cout << "Item not found in inventory.\n";
	}
}
// Function to calculate the total number of items in the inventory
void totalItems(const std::vector<Item>& inventory)
{
	int totalItems = 0;
	for (const auto& item : inventory)
	{
		totalItems += item.quantity;
	}
	std::cout << "Total number of items in inventory: " << totalItems << '\n';
}
// Function to calculate the total value of the inventory
void totalInventoryValue(const std::vector<Item>& inventory)
{
	double totalValue = 0.0;
	for (const auto& item : inventory)
	{
		totalValue += item.quantity * item.price;
	}
	std::cout << "Total value of inventory: $" << std::fixed << std::setprecision(2) << totalValue << '\n';
}