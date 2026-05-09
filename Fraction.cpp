#include "Fraction.h"

void Fraction::reduceFraction() {
	try {
		Polynom commonMonomNumerator(numerator.getCommonMonom(), 1);
		Polynom commonMonomDenumerator(denumerator.getCommonMonom(), 1);

		Polynom temp = commonMonomNumerator + commonMonomDenumerator;
		Polynom smallDivider(temp.getCommonMonom(), 1);

		if (!smallDivider.getFreeMember()) {
			numerator /= smallDivider;
			denumerator /= smallDivider;
		}

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

bool Fraction::isZeroFraction() const {
	return numerator.isZeroPolynom();
}

Fraction& Fraction::operator+=(const Fraction& other) {
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

Fraction operator+(Fraction first, const Fraction& second) {
	first += second;
	return first;
}

Fraction& Fraction::operator-=(const Fraction& other) {
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

Fraction operator-(Fraction first, const Fraction& second) {
	first -= second;
	return first;
}

Fraction& Fraction::operator*=(const Fraction& other) {
	numerator *= other.numerator;
	denumerator *= other.denumerator;

	if (isFraction()) {
		reduceFraction();
	}

	return *this;
}

Fraction operator*(Fraction first, const Fraction& second) {
	first *= second;
	return first;
}

Fraction& Fraction::operator/=(const Fraction& other) {
	numerator *= other.denumerator;
	denumerator *= other.numerator;

	if (isFraction()) {
		reduceFraction();
	}

	return *this;
}

Fraction operator/(Fraction first, const Fraction& second) {
	first /= second;
	return first;
}

Polynom Fraction::getMixedFraction(RETURNTYPEDIVIDEPOLYNOMS returnType) {
	if (returnType == QUOTIENT) {
		return numerator / denumerator;
	}
	else {
		return numerator % denumerator;
	}
}

bool Fraction::isRational() const {
	return numerator.isFreePolynom() && !isFraction();
}

void Fraction::normilizeFraction() {
	Polynom p(denumerator.getLMC());
	numerator /= p;
	denumerator /= p;
}

bool Fraction::isFraction() const {
	return !denumerator.isFreePolynom();
}

Polynom Fraction::getNumerator() const {
	return numerator;
}

Polynom Fraction::getDenumenator() const {
	return denumerator;
}

std::ostream& operator<<(std::ostream& os, const Fraction& fraction) {
	os << fraction.numerator;
	os << " / ";
	os << fraction.denumerator;

	return os;
}