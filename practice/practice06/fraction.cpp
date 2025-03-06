#include "Fraction.h"

// Arithmetic operators overloading
Fraction operator+(const Fraction& lhs, const Fraction& rhs) 
{
    int newNum = lhs.num * rhs.den + rhs.num * lhs.den;
    int newDen = lhs.den * rhs.den;
    return Fraction(newNum, newDen);
}

Fraction operator-(const Fraction& lhs, const Fraction& rhs) 
{
    int newNum = lhs.num * rhs.den - rhs.num * lhs.den;
    int newDen = lhs.den * rhs.den;
    return Fraction(newNum, newDen);
}

Fraction operator*(const Fraction& lhs, const Fraction& rhs) 
{
    int newNum = lhs.num * rhs.num;
    int newDen = lhs.den * rhs.den;
    return Fraction(newNum, newDen);
}

Fraction operator/(const Fraction& lhs, const Fraction& rhs) 
{
    if (rhs.num == 0) throw std::invalid_argument("Division by zero");
    int newNum = lhs.num * rhs.den;
    int newDen = lhs.den * rhs.num;
    return Fraction(newNum, newDen);
}

// Stream overloading
std::ostream& operator<<(std::ostream& os, const Fraction& frac) 
{
    os << frac.num << "/" << frac.den;
    return os;
}
