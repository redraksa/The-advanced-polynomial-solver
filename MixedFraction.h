#pragma once
#include "Fraction.h"
#include "parametres.h"

using namespace parameters;

class MixedFraction
{
private:
	Polynom whole;
	Fraction remainder;

public:
	MixedFraction(Polynom whole) : whole(std::move(whole)), remainder(Polynom(Rational(0))) {}
	
	MixedFraction() : whole(Polynom(Rational(0))), remainder(Polynom(Rational(0))) {}

	MixedFraction& operator+=(const MixedFraction& other);

	friend MixedFraction operator+(MixedFraction first, const MixedFraction& second);

	MixedFraction& operator-=(const MixedFraction& other);

	friend MixedFraction operator-(MixedFraction first, const MixedFraction& second);

	MixedFraction& operator*=(const MixedFraction& other);

	friend MixedFraction operator*(MixedFraction first, const MixedFraction& second);

	MixedFraction& operator/=(const MixedFraction& other);
	

	friend MixedFraction operator/(MixedFraction first, const MixedFraction& second);

	void normilize();

	bool isZero() const;

	bool isInteger() const;

	long long getFreeMember() const;

	bool isZeroWhole() const;
	
	bool isFraction() const;

	Polynom getWholePart() const;

	Fraction getFractionPart() const;
};

