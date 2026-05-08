#pragma once
#include <iostream>
#include <map>
#include <numeric>
//#include "Rational.h"
#include "Polynom.h"
#include "parametres.h"

using namespace parameters;

class Fraction {
private:
	Polynom numerator;
	Polynom denumerator;

	void reduceFraction() {
		try {
			auto divider = Polynom::GCD(numerator, denumerator);
			if (divider.has_value()) {
				numerator /= divider.value();
				denumerator /= divider.value();
			}
		}
		catch (const std::overflow_error&) {
			return;
		}
	}

public:
	Fraction(Polynom numenatorPolynom) : numerator(std::move(numenatorPolynom)), denumerator(Polynom(Rational(1))) {}

	/*	void setNumenator(Polynom polynom) {
		numerator = std::move(polynom);
	}

	void setDenumenator(Polynom polynom) {
		denumerator = std::move(polynom);
	}*/

	bool isZeroFraction() const {
		return numerator.isZeroPolynom();
	}

	Fraction& operator+=(const Fraction& other) {
		Polynom otherNumerator = other.numerator;
		if (denumerator != other.denumerator) {
			otherNumerator = other.numerator * denumerator;
			numerator = numerator * other.denumerator;
			denumerator = other.denumerator * denumerator;
		}

		numerator = numerator + otherNumerator;

		if (isFraction()) {
			reduceFraction();
		}

		return *this;
	}

	friend Fraction operator+(Fraction first, const Fraction& second) {
		first += second;
		return first;
	}

	Fraction& operator-=(const Fraction& other) {
		Polynom otherNumerator = other.numerator;
		if (denumerator != other.denumerator) {
			otherNumerator = other.numerator * denumerator;
			numerator = numerator * other.denumerator;
			denumerator = other.denumerator * denumerator;
		}

		numerator = numerator - otherNumerator;

		if (isFraction()) {
			reduceFraction();
		}

		return *this;
	}

	friend Fraction operator-(Fraction first, const Fraction& second) {
		first -= second;
		return first;
	}
	
	Fraction& operator*=(const Fraction& other) {
		numerator *= other.numerator;
		denumerator *= other.denumerator;

		if (isFraction()) {
			reduceFraction();
		}

		return *this;
	}

	friend Fraction operator*(Fraction first, const Fraction& second) {
		first *= second;
		return first;
	}

	Fraction& operator/=(const Fraction& other) {
		numerator *= other.denumerator;
		denumerator *= other.numerator;

		if (isFraction()) {
			reduceFraction();
		}

		return *this;
	}

	friend Fraction operator/(Fraction first, const Fraction& second) {
		first /= second;
		return first;
	}

	Polynom getMixedFraction(RETURNTYPEDIVIDEPOLYNOMS returnType) {
		if (returnType == QUOTIENT) {
			return numerator / denumerator;
		}
		else {
			return numerator % denumerator;
		}
	}

	bool isRational() {
		return numerator.isFreePolynom() && !isFraction();
	}

	void normilizeFraction() {
		Polynom p(denumerator.getLMC());
		numerator /= p;
		denumerator /= p;
	}

	/*	Rational getFreeMember() {
		if (numerator.find(0) != numerator.cend()) {
			return numerator[0];
		}
		else {
			return Rational(0, 1);
		}
	}*/


	bool isFraction() {
		return !denumerator.isFreePolynom();
	}

	/*	void eraseFraction() {
		numerator[0] = 0;
		denumerator[0] = 1;
	}*/


	Polynom getNumerator() const {
		return numerator;
	}

	Polynom getDenumenator() const {
		return denumerator;
	}
};

