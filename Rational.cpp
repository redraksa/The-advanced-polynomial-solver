#include "Rational.h"

void Rational::reduceFraction() {
	long long gcd = std::gcd(numenator, denumenator);
	if (gcd != 0) {
		numenator /= gcd;
		denumenator /= gcd;
	}

	normilizeFraction();
}

void Rational::normilizeFraction() {
	if ((numenator > 0 && denumenator < 0) || (numenator < 0 && denumenator < 0)) {
		numenator = saveMul(numenator, -1);
		denumenator = saveMul(denumenator, -1);
	}
}

long long Rational::saveAdd(const long long first, const long long second) {
	long long result;

	if (__builtin_add_overflow(first, second, &result)) {
		throw std::overflow_error("Addition overflow: " + std::to_string(first) + "+" + std::to_string(second));
	}

	return result;
}

long long Rational::saveSub(const long long first, const long long second) {
	long long result;

	if (__builtin_sub_overflow(first, second, &result)) {
		throw std::overflow_error("Substraction overflow: " + std::to_string(first) + "-" + std::to_string(second));
	}

	return result;
}

long long Rational::saveDiv(const long long divisible, const long long divider) {
	if (divider == 0) {
		throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
	}
	return divisible / divider;
}

long long Rational::saveMul(const long long first, const long long second) {
	long long result;

	if (__builtin_mul_overflow(first, second, &result)) {
		throw std::overflow_error("Multiplication overflow: " + std::to_string(first) + "*" + std::to_string(second));
	}

	return result;
}

Rational& Rational::operator+=(const Rational& other) {
	long long GCDDenumenator = std::gcd(denumenator, other.denumenator);
	long long LCMDenumeantor = saveMul(saveDiv(denumenator, GCDDenumenator), other.denumenator);
	long long newFirstNumenator = saveMul(numenator, saveDiv(other.denumenator, GCDDenumenator));
	long long newSecondNumenator = saveMul(other.numenator, saveDiv(denumenator, GCDDenumenator));
	long long newNumenator = saveAdd(newFirstNumenator, newSecondNumenator);

	numenator = newNumenator;
	denumenator = LCMDenumeantor;

	if (denumenator == 0) {
		throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
	}

	reduceFraction();

	return *this;
}

Rational& Rational::operator-=(const Rational& other) {
	long long GCDDenumenator = std::gcd(denumenator, other.denumenator);
	long long LCMDenumeantor = saveMul(saveDiv(denumenator, GCDDenumenator), other.denumenator);
	long long newFirstNumenator = saveMul(numenator, saveDiv(other.denumenator, GCDDenumenator));
	long long newSecondNumenator = saveMul(other.numenator, saveDiv(denumenator, GCDDenumenator));
	long long newNumenator = saveSub(newFirstNumenator, newSecondNumenator);

	numenator = newNumenator;
	denumenator = LCMDenumeantor;

	if (denumenator == 0) {
		throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
	}

	reduceFraction();

	return *this;
}

Rational Rational::operator-() const {
	Rational temp = *this;
	temp *= -1;
	return temp;
}

Rational& Rational::operator*=(const Rational& other) {
	long long firstGCD = std::gcd(numenator, other.denumenator);
	long long secondGCD = std::gcd(denumenator, other.numenator);
	numenator = saveDiv(numenator, firstGCD);
	denumenator = saveDiv(denumenator, secondGCD);
	long long otherDenumenator = saveDiv(other.denumenator, firstGCD);
	long long otherNumenator = saveDiv(other.numenator, secondGCD);
	numenator = saveMul(numenator, otherNumenator);
	denumenator = saveMul(denumenator, otherDenumenator);

	if (denumenator == 0) {
		throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
	}

	normilizeFraction();

	return *this;
}

Rational& Rational::operator/=(const Rational& other) {
	long long firstGCD = std::gcd(numenator, other.numenator);
	long long secondGCD = std::gcd(denumenator, other.denumenator);
	numenator = saveDiv(numenator, firstGCD);
	denumenator = saveDiv(denumenator, secondGCD);
	long long otherDenumenator = saveDiv(other.denumenator, secondGCD);
	long long otherNumenator = saveDiv(other.numenator, firstGCD);
	numenator = saveMul(numenator, otherDenumenator);
	denumenator = saveMul(denumenator, otherNumenator);

	normilizeFraction();

	if (denumenator == 0) {
		throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
	}

	return *this;
}

Rational& Rational::operator=(const long long number) {
	numenator = number;
	denumenator = 1;
	return *this;
}

Rational operator+(Rational first, const Rational& second) {
	return first += second;
}
Rational operator-(Rational first, const Rational& second) {
	return first -= second;
}
Rational operator*(Rational first, const Rational& second) {
	return first *= second;
}
Rational operator/(Rational first, const Rational& second) {
	return first /= second;
}

Rational operator+(Rational first, const long long integer) {
	Rational second(integer);
	return first += second;
}
Rational operator-(Rational first, const long integer) {
	Rational second(integer);
	return first -= second;
}
Rational operator*(Rational first, const long long integer) {
	Rational second(integer);
	return first *= second;
}
Rational operator*(Rational first, const int integer) {
	return first * (long long)integer;
}
Rational operator/(Rational first, const long long integer) {
	Rational second(integer);
	return first /= second;
}

bool operator==(const Rational& first, const Rational& second) {
	if (first.numenator == second.numenator && first.denumenator == second.denumenator) {
		return true;
	}

	long long firstMul = Rational::saveMul(first.numenator, second.denumenator);
	long long secondMul = Rational::saveMul(second.numenator, first.denumenator);

	return firstMul == secondMul;
}

bool operator!=(const Rational& first, const Rational& second) {
	return !(first == second);
}

bool operator==(const Rational& first, const long long integer) {
	Rational other(integer);
	return first == other;
}

bool operator==(const Rational& first, const int integer) {
	return first == (long long)integer;
}

bool operator!=(const Rational& first, const long long integer) {
	return !(first == integer);
}
bool operator!=(const Rational& first, const int integer) {
	return first != (long long)integer;
}

bool operator>(const Rational& first, const Rational& second) {
	long long firstMul = Rational::saveMul(first.numenator, second.denumenator);
	long long secondMul = Rational::saveMul(second.numenator, first.denumenator);

	return firstMul > secondMul;
}

bool operator<(const Rational& first, const Rational& second) {
	long long firstMul = Rational::saveMul(first.numenator, second.denumenator);
	long long secondMul = Rational::saveMul(second.numenator, first.denumenator);

	return firstMul < secondMul;

}

bool operator>(const Rational& first, const long long number) {
	Rational second(number);
	return first > second;
}
bool operator>(const Rational& first, const int number) {
	return first > (long long)number;
}

bool operator<(const Rational& first, const long long number) {
	Rational second(number);
	return first < second;
}
bool operator<(const Rational& first, const int number) {
	return first < (long long)number;
}

std::ostream& operator<<(std::ostream& os, const Rational& r) {
	os << r.numenator;
	if (r.denumenator != 1) {
		os << "/" << r.denumenator;
	}

	return os;
}

bool Rational::isInteger() const {
	return denumenator == 1;
}

Rational::operator long long() const {
	return numenator / denumenator;
}

Rational abs(const Rational& r) {
	return Rational(std::abs(r.numenator), std::abs(r.denumenator));
}

long long Rational::getNumenator() const {
	return numenator;
}

long long Rational::getDenumenator() const {
	return denumenator;
}