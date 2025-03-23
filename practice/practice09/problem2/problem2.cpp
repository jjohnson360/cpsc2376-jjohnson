#include <iostream>

// Copilot intergration in Visual Studio was used in writing this program

struct Point
{
    float x;
    float y;
};

// Function to calculate the midpoint between two points (returns a Point object)
Point calcMidpoint(const Point* p1, const Point* p2)
{
    Point midpoint;
    midpoint.x = (p1->x + p2->x) / 2.0f;
    midpoint.y = (p1->y + p2->y) / 2.0f;
    return midpoint;
}

int main()
{
    Point a, b;

    // Get input for point a
    std::cout << "Enter x-coordinate for point a: ";
    std::cin >> a.x;
    std::cout << "Enter y-coordinate for point a: ";
    std::cin >> a.y;

    // Get input for point b
    std::cout << "Enter x-coordinate for point b: ";
    std::cin >> b.x;
    std::cout << "Enter y-coordinate for point b: ";
    std::cin >> b.y;

    // Call calcMidpoint using pointers to 'a' and 'b'
    Point mid = calcMidpoint(&a, &b);

    // Print the midpoint coordinates
    std::cout << "Midpoint is at (" << mid.x << ", " << mid.y << ")" << std::endl;

    return 0;
}