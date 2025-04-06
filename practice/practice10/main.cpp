#include <iostream>
#include <vector>
#include <memory> 
#include <string>
#include <fstream>  
#include <sstream>  
#include <cmath>    
#include <iomanip>  
#include <stdexcept> 


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Base Shape Class
class Shape 
{
public:
    // Virtual destructor is crucial for polymorphism with dynamic allocation
    virtual ~Shape() = default;

    // Pure virtual function makes Shape an abstract base class
    virtual double getArea() const = 0;
};

// Derived Rectangle Class
class Rectangle : public Shape 
{
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {
        if (w < 0 || h < 0) 
        {
            throw std::invalid_argument("Rectangle dimensions cannot be negative.");
        }
    }
    // Override the virtual function from Shape
    double getArea() const override 
    {
        return width * height;
    }
};
// Derived Circle Class
class Circle : public Shape 
{
private:
    double radius;

public:
    Circle(double r) : radius(r) 
    {
        if (r < 0) 
        {
            throw std::invalid_argument("Circle radius cannot be negative.");
        }
    }

    // Override the virtual function from Shape
    double getArea() const override 
    {
        return M_PI * radius * radius;
    }
};

// Function to print areas
void printAllAreas(const std::vector<std::unique_ptr<Shape>>& shapes) 
{
    std::cout << std::fixed << std::setprecision(4);
    for (const auto& shape_ptr : shapes) 
    {
        if (shape_ptr)
        { 
            std::cout << "Area: " << shape_ptr->getArea() << std::endl;
        }
    }
}

int main() 
{
    std::vector<std::unique_ptr<Shape>> shapes_vector;
    const std::string filename = "shapes.txt";
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) 
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(inputFile, line)) 
    {
        lineNumber++;
        std::stringstream ss(line);
        std::string shapeType;

        ss >> shapeType;

        try {
            if (shapeType == "rectangle") 
            {
                double width, height;
                if (!(ss >> width >> height)) 
                {
                    std::cerr << "Warning: Invalid format for rectangle on line " << lineNumber << ". Skipping." << std::endl;
                    continue;
                }

                shapes_vector.push_back(std::make_unique<Rectangle>(width, height));
            }
            else if (shapeType == "circle") 
            {
                double radius;
                if (!(ss >> radius)) 
                {
                    std::cerr << "Warning: Invalid format for circle on line " << lineNumber << ". Skipping." << std::endl;
                    continue;
                }

                shapes_vector.push_back(std::make_unique<Circle>(radius));

            }
            else if (!shapeType.empty()) 
            {
                std::cerr << "Warning: Unknown shape type '" << shapeType << "' on line " << lineNumber << ". Skipping." << std::endl;
            }
        }
        catch (const std::invalid_argument& e) 
        {
            std::cerr << "Warning: Invalid dimension on line " << lineNumber << " (" << e.what() << "). Skipping." << std::endl;
        }
        std::string remaining;
        if (ss >> remaining) 
        {
            std::cerr << "Warning: Extra data found on line " << lineNumber << ". Processing shape anyway." << std::endl;
        }

    }

    inputFile.close();

    printAllAreas(shapes_vector);


    return 0;
}