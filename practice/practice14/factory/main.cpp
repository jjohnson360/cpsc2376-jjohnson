#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>

// Created with the assitance of Copilot integration in Visual Studio

// Define M_PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846 // More precise definition
#endif

// Forward declaration of the Shape interface
class Shape {
public:
    virtual double area() const = 0;
    virtual std::string getName() const = 0;
    virtual ~Shape() = default;
};

// Implementation of the Rectangle class
class Rectangle : public Shape {
private:
    double width;
    double height;
    std::string name;
public:
    Rectangle(double w, double h) : width(w), height(h), name("Rectangle") {}

    double area() const override {
        return width * height;
    }
    std::string getName() const override {
        return name;
    }
};

// Implementation of the Circle class
class Circle : public Shape {
private:
    double radius;
    std::string name;
public:
    Circle(double r) : radius(r), name("Circle") {}

    double area() const override {
        return M_PI * radius * radius;
    }
    std::string getName() const override {
        return name;
    }
};

// Implementation of the Triangle class
class Triangle : public Shape {
private:
    double base;
    double height;
    std::string name;
public:
    Triangle(double b, double h) : base(b), height(h), name("Triangle") {}

    double area() const override {
        return 0.5 * base * height;
    }
    std::string getName() const override {
        return name;
    }
};

// Implementation of the Square class
class Square : public Shape {
private:
    double side;
    std::string name;
public:
    Square(double s) : side(s), name("Square") {}

    double area() const override {
        return side * side;
    }
    std::string getName() const override {
        return name;
    }
};

// Implementation of the Ellipse class
class Ellipse : public Shape {
private:
    double majorAxis;
    double minorAxis;
    std::string name;
public:
    Ellipse(double a, double b) : majorAxis(a), minorAxis(b), name("Ellipse") {}

    double area() const override {
        return M_PI * majorAxis * minorAxis;
    }
    std::string getName() const override {
        return name;
    }
};

// Shape Factory class
class ShapeFactory {
public:
    static std::unique_ptr<Shape> createShape(const std::string& type, const std::vector<double>& dimensions) {
        if (type == "rectangle") {
            if (dimensions.size() != 2) {
                throw std::runtime_error("Rectangle requires 2 dimensions (width, height)");
            }
            return std::make_unique<Rectangle>(dimensions[0], dimensions[1]);
        }
        else if (type == "circle") {
            if (dimensions.size() != 1) {
                throw std::runtime_error("Circle requires 1 dimension (radius)");
            }
            return std::make_unique<Circle>(dimensions[0]);
        }
        else if (type == "triangle") {
            if (dimensions.size() != 2) {
                throw std::runtime_error("Triangle requires 2 dimensions (base, height)");
            }
            return std::make_unique<Triangle>(dimensions[0], dimensions[1]);
        }
        else if (type == "square") {
            if (dimensions.size() != 1) {
                throw std::runtime_error("Square requires 1 dimension (side)");
            }
            return std::make_unique<Square>(dimensions[0]);
        }
        else if (type == "ellipse") {
            if (dimensions.size() != 2) {
                throw std::runtime_error("Ellipse requires 2 dimensions (major axis, minor axis)");
            }
            return std::make_unique<Ellipse>(dimensions[0], dimensions[1]);
        }
        else {
            throw std::runtime_error("Invalid shape type: " + type);
        }
    }
};

int main() {
    std::string filename = "shapes.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return 1;
    }

    std::string line;
    double totalArea = 0.0;
    std::vector<std::unique_ptr<Shape>> shapes;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        std::getline(ss, type, ',');

        std::vector<double> dimensions;
        std::string dimensionStr;
        while (std::getline(ss, dimensionStr, ',')) {
            try {
                dimensions.push_back(std::stod(dimensionStr));
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid dimension value: " << dimensionStr << " in file " << filename << std::endl;
                file.close();
                return 1;
            }
        }

        try {
            std::unique_ptr<Shape> shape = ShapeFactory::createShape(type, dimensions);
            totalArea += shape->area();
            shapes.push_back(std::move(shape));
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error creating shape: " << e.what() << " in file " << filename << std::endl;
            file.close();
            return 1;
        }
    }
    file.close();

    std::cout << "Shapes processed from file " << filename << ":\n";
    for (const auto& shape : shapes) {
        std::cout << "  " << shape->getName() << ": Area = " << shape->area() << std::endl;
    }
    std::cout << "Total Area: " << totalArea << std::endl;

    return 0;
}

