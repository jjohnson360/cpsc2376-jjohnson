#pragma once
#ifndef MIXEDFRACTION_H
#define MIXEDFRACTION_H

#include "Fraction.h"

class MixedFraction : public Fraction 
{
private:
    int whole;

public:
    // Parameterized constructor
    MixedFraction(int whole, int n, int d);
    // Constructor to convert Fraction to MixedFraction
    MixedFraction(const Fraction& fraction);

    // Friend function to overload << operator
    friend std::ostream& operator<<(std::ostream& os, const MixedFraction& mf);
};

#endif // MIXEDFRACTION_H
