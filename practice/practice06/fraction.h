#ifndef FRACTION_H
#define FRACTION_H

#include <stdexcept>
#include <iostream>

class Fraction 
{
private:
    int num; // numerator
    int den; // denominator

public:
    // Default constructor
    Fraction() : num(0), den(1) {}

    // Parameterized constructor
    Fraction(int n, int d) : num(n), den(d) 
    {
        if (den == 0) throw std::invalid_argument("Denominator cannot be zero");
        simplify();
    }

    // Getters
    int getNumerator() const { return num; }
    int getDenominator() const { return den; }

    // Setters
    void setNumerator(int n) 
    {
        num = n;
        simplify();
    }

    void setDenominator(int d) 
    {
        if (d == 0) throw std::invalid_argument("Denominator cannot be zero");
        den = d;
        simplify();
    }

    // Simplify the fraction using Euclidean Algorithm
    void simplify() 
    {
        if (den == 0) throw std::invalid_argument("Denominator cannot be zero");

        int a = num, b = den;
        while (b != 0) 
        {
            int temp = b;
            b = a % b;
            a = temp;
        }
        int gcd = a;

        num /= gcd;
        den /= gcd;

        // Ensure denominator is positive
        if (den < 0) 
        {
            num = -num;
            den = -den;
        }
    }

    // Friend functions for arithmetic operators
    friend Fraction operator+(const Fraction& lhs, const Fraction& rhs);
    friend Fraction operator-(const Fraction& lhs, const Fraction& rhs);
    friend Fraction operator*(const Fraction& lhs, const Fraction& rhs);
    friend Fraction operator/(const Fraction& lhs, const Fraction& rhs);

    // Stream overloading
    friend std::ostream& operator<<(std::ostream& os, const Fraction& frac);
};

#endif
