#include "MixedFraction.h"

// Parameterized constructor
MixedFraction::MixedFraction(int whole, int n, int d)
    : Fraction(n, d), whole(whole) {
}

// Constructor to convert Fraction to MixedFraction
MixedFraction::MixedFraction(const Fraction& fraction)
    : Fraction(fraction) 
{
    whole = getNumerator() / getDenominator();
    setNumerator(getNumerator() % getDenominator());
}

// Overload << operator
std::ostream& operator<<(std::ostream& os, const MixedFraction& mf) 
{
    if (mf.getDenominator() == 1) 
    {
        os << mf.whole;
    }
    else if (mf.whole == 0) 
    {
        os << mf.getNumerator() << "/" << mf.getDenominator();
    }
    else 
    {
        os << mf.whole << " " << mf.getNumerator() << "/" << mf.getDenominator();
    }
    return os;
}
