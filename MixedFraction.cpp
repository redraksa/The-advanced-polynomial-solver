#include "MixedFraction.h"

MixedFraction& MixedFraction::operator+=(const MixedFraction& other) {
	whole += other.whole;

	remainder += other.remainder;

	Polynom temp = remainder.getMixedFraction(REMAINDER);
	whole += remainder.getMixedFraction(QUOTIENT);
	Fraction tempFraction(temp);
	tempFraction /= remainder.getDenumenator();
	remainder = tempFraction;

	return *this;
}

MixedFraction operator+(MixedFraction first, const MixedFraction& second) {
	first += second;
	return first;
}

MixedFraction& MixedFraction::operator-=(const MixedFraction& other) {
	whole -= other.whole;

	remainder -= other.remainder;

	Polynom temp = remainder.getMixedFraction(REMAINDER);
	whole += remainder.getMixedFraction(QUOTIENT);
	Fraction tempFraction(temp);
	tempFraction /= remainder.getDenumenator();
	remainder = tempFraction;

	return *this;
}

MixedFraction operator-(MixedFraction first, const MixedFraction& second) {
	first -= second;
	return first;
}

MixedFraction& MixedFraction::operator*=(const MixedFraction& other) {
	Polynom tempWhole = whole * other.whole;
	Fraction wholeFraction(whole), otherWholeFraction(other.whole);
	Fraction tempRemainder = wholeFraction * other.remainder + remainder * otherWholeFraction + remainder * other.remainder;

	Polynom temp = tempRemainder.getMixedFraction(REMAINDER);
	whole = tempWhole + tempRemainder.getMixedFraction(QUOTIENT);
	Fraction tempFraction(temp);
	tempFraction /= tempRemainder.getDenumenator();
	remainder = tempFraction;

	return *this;
}

MixedFraction operator*(MixedFraction first, const MixedFraction& second) {
	first *= second;
	return first;
}

MixedFraction& MixedFraction::operator/=(const MixedFraction& other) {
	Fraction divider(other.whole);
	divider += other.remainder;

	Fraction divisible(whole);
	divisible += remainder;

	divisible /= divider;

	Polynom temp = divisible.getMixedFraction(REMAINDER);
	whole = divisible.getMixedFraction(QUOTIENT);
	Fraction tempFraction(temp);
	tempFraction /= divisible.getDenumenator();
	remainder = tempFraction;

	return *this;
}


MixedFraction operator/(MixedFraction first, const MixedFraction& second) {
	first /= second;
	return first;
}

void MixedFraction::normilize() {
	remainder.normilizeFraction();
}

bool MixedFraction::isZero() const {
	return whole.isZeroPolynom() && remainder.isZeroFraction();
}

bool MixedFraction::isInteger() const {
	return remainder.isZeroFraction() && whole.isFreePolynom();
}

long long MixedFraction::getFreeMember() const {
	std::optional<Rational> oMember = whole.getFreeMember();
	if (oMember.has_value()) {
		return static_cast<long long>(oMember.value());
	}
	else {
		return 0;
	}
}

bool MixedFraction::isZeroWhole() const {
	return whole.isZeroPolynom();
}

bool MixedFraction::isFraction() const {
	return !remainder.isZeroFraction();
}

Polynom MixedFraction::getWholePart() const {
	return whole;
}

Fraction MixedFraction::getFractionPart() const {
	return remainder;
}