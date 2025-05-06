#include <iostream>
#include <memory>
#include <iomanip>
#include <sstream>

// Created with the assitance of Copilot integration in Visual Studio

// Forward declaration of the Purchase interface
class Purchase 
{
public:
    virtual double getCost() const = 0;
    virtual std::string getDescription() const = 0;
    virtual ~Purchase() = default;
};

// Implementation of the SimplePurchase class
class SimplePurchase : public Purchase 
{
private:
    double cost;
    std::string description;
public:
    SimplePurchase(double initialCost, const std::string& initialDescription) : cost(initialCost), description(initialDescription) {}

    double getCost() const override 
    {
        return cost;
    }
    std::string getDescription() const override 
    {
        return description;
    }
};

// Base class for decorators
class PurchaseDecorator : public Purchase 
{
protected:
    std::shared_ptr<Purchase> purchase;
public:
    PurchaseDecorator(std::shared_ptr<Purchase> decoratedPurchase) : purchase(std::move(decoratedPurchase)) {}
    virtual double getCost() const override = 0;
    virtual std::string getDescription() const override = 0;
};

// Implementation of the TaxDecorator class
class TaxDecorator : public PurchaseDecorator 
{
private:
    double taxRate;
public:
    TaxDecorator(std::shared_ptr<Purchase> decoratedPurchase, double rate)
        : PurchaseDecorator(std::move(decoratedPurchase)), taxRate(rate) {
    }

    double getCost() const override {
        return purchase->getCost() * (1 + taxRate);
    }

    std::string getDescription() const override {
        std::ostringstream oss;
        oss << purchase->getDescription() << " + Tax (" << std::fixed << std::setprecision(2) << taxRate * 100 << "%)";
        return oss.str();
    }
};

// Implementation of the ShippingFeeDecorator class
class ShippingFeeDecorator : public PurchaseDecorator 
{
private:
    double shippingFee;
public:
    ShippingFeeDecorator(std::shared_ptr<Purchase> decoratedPurchase, double fee)
        : PurchaseDecorator(std::move(decoratedPurchase)), shippingFee(fee) {
    }

    double getCost() const override 
    {
        return purchase->getCost() + shippingFee;
    }

    std::string getDescription() const override {
        std::ostringstream oss;
        oss << purchase->getDescription() << " + Shipping (" << std::fixed << std::setprecision(2) << shippingFee << ")";
        return oss.str();
    }
};

// Implementation of the DiscountDecorator class
class DiscountDecorator : public PurchaseDecorator 
{
private:
    double discountRate;
public:
    DiscountDecorator(std::shared_ptr<Purchase> decoratedPurchase, double rate)
        : PurchaseDecorator(std::move(decoratedPurchase)), discountRate(rate) {
    }

    double getCost() const override 
    {
        return purchase->getCost() * (1 - discountRate);
    }
    std::string getDescription() const override 
    {
        std::ostringstream oss;
        oss << purchase->getDescription() << " - Discount (" << std::fixed << std::setprecision(2) << discountRate * 100 << "%)";
        return oss.str();
    }
};

// Function to get double input from the user with validation
double getDoubleInput(const std::string& prompt) 
{
    double value;
    while (true) 
    {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
        }
        else 
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    return value;
}

// Function to get integer input from the user with validation
int getIntegerInput(const std::string& prompt) 
{
    int value;
    while (true) 
    {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter an integer.\n";
        }
        else 
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    return value;
}

int main() 
{
    // Initial purchase
    double baseCost = getDoubleInput("Enter the base purchase amount: ");
    std::string baseDescription = "Base Purchase";
    std::shared_ptr<Purchase> purchase = std::make_shared<SimplePurchase>(baseCost, baseDescription);

    // Decorator options
    int addTax = getIntegerInput("Add tax (1 for yes, 0 for no): ");
    if (addTax == 1) 
    {
        double taxRate = getDoubleInput("Enter tax rate (e.g., 0.10 for 10%): ");
        purchase = std::make_shared<TaxDecorator>(purchase, taxRate);
    }

    int addShipping = getIntegerInput("Add shipping fee (1 for yes, 0 for no): ");
    if (addShipping == 1) 
    {
        double shippingFee = getDoubleInput("Enter shipping fee: ");
        purchase = std::make_shared<ShippingFeeDecorator>(purchase, shippingFee);
    }

    int addDiscount = getIntegerInput("Add discount (1 for yes, 0 for no): ");
    if (addDiscount == 1) 
    {
        double discountRate = getDoubleInput("Enter discount rate (e.g., 0.10 for 10%): ");
        purchase = std::make_shared<DiscountDecorator>(purchase, discountRate);
    }

    // Print the final cost and description
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Final Cost: $" << purchase->getCost() << std::endl;
    std::cout << "Description: " << purchase->getDescription() << std::endl;

    return 0;
}

