#pragma once
#include <iostream>
#include <map>
#include <numeric>
#include "Polynom.h"
#include "parametres.h"

using namespace parameters;

class Fraction {
private:
	Polynom numerator;
	Polynom denumerator;

	void reduceFraction();

public:
	Fraction(Polynom numenatorPolynom) : numerator(std::move(numenatorPolynom)), denumerator(Polynom(Rational(1))) {}

	bool isZeroFraction() const;

	Fraction& operator+=(const Fraction& other);

	friend Fraction operator+(Fraction first, const Fraction& second);

	Fraction& operator-=(const Fraction& other);

	friend Fraction operator-(Fraction first, const Fraction& second);
	
	Fraction& operator*=(const Fraction& other);

	friend Fraction operator*(Fraction first, const Fraction& second);

	Fraction& operator/=(const Fraction& other);

	friend Fraction operator/(Fraction first, const Fraction& second);

	Polynom getMixedFraction(RETURNTYPEDIVIDEPOLYNOMS returnType);

	bool isRational() const;

	void normilizeFraction();

	bool isFraction() const;

	Polynom getNumerator() const;

	Polynom getDenumenator() const;

	friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction);
};

