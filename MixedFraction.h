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


	MixedFraction& operator+=(const MixedFraction& other) {
		whole += other.whole;

		remainder += other.remainder;

		Polynom temp = remainder.getMixedFraction(REMAINDER);
		whole += remainder.getMixedFraction(QUOTIENT);
		Fraction tempFraction(temp);
		tempFraction /= remainder.getDenumenator();
		remainder = tempFraction;

		return *this;
	}

	friend MixedFraction operator+(MixedFraction first, const MixedFraction& second) {
		first += second;
		return first;
	}

	MixedFraction& operator-=(const MixedFraction& other) {
		whole -= other.whole;

		remainder -= other.remainder;

		Polynom temp = remainder.getMixedFraction(REMAINDER);
		whole += remainder.getMixedFraction(QUOTIENT);
		Fraction tempFraction(temp);
		tempFraction /= remainder.getDenumenator();
		remainder = tempFraction;

		return *this;
	}

	friend MixedFraction operator-(MixedFraction first, const MixedFraction& second) {
		first -= second;
		return first;
	}

	MixedFraction& operator*=(const MixedFraction& other) {
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

	friend MixedFraction operator*(MixedFraction first, const MixedFraction& second) {
		first *= second;
		return first;
	}

	MixedFraction& operator/=(const MixedFraction& other) {
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
	

	friend MixedFraction operator/(MixedFraction first, const MixedFraction& second) {
		first /= second;
		return first;
	}

	void normilize() {
		remainder.normilizeFraction();
	}

	bool isZero() {
		return whole.isZeroPolynom() && remainder.isZeroFraction();
	}

	bool isInteger() {
		return remainder.isZeroFraction() && whole.isFreePolynom();
	}

	long long getFreeMember() {
		std::optional<Rational> oMember = whole.getFreeMember();
		if (oMember.has_value()) {
			return static_cast<long long>(oMember.value());
		}
		else {
			return 0;
		}
	}
	/*	

	



	


	}*/

	bool isZeroWhole() {
		return whole.isZeroPolynom();
	}
	
	bool isFraction() {
		return !remainder.isZeroFraction();
	}

	Polynom getWholePart() const {
		return whole;
	}

	Fraction getFractionPart() const {
		return remainder;
	}
};

