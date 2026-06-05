#pragma once
#include <iostream>
#include <numeric>
#include <ostream>

class Rational
{
private:
	long long numenator;
	long long denumenator;

	void reduceFraction();

	void normilizeFraction();

	static long long saveAdd(const long long first, const long long second);
	static long long saveSub(const long long first, const long long second);
	static long long saveDiv(const long long divisible, const long long divider);
	static long long saveMul(const long long first, const long long second);

	
public:
	Rational() : numenator(0), denumenator(1) {}

	Rational(long long numenator, long long denumenator) : numenator(numenator), denumenator(denumenator) {
		if (denumenator == 0) {
			throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
		}
		reduceFraction();
	}

	Rational(long long number) : numenator(number), denumenator(1) {}

	Rational& operator+=(const Rational& other);

	Rational& operator-=(const Rational& other);

	Rational operator-() const;

	Rational& operator*=(const Rational& other);

	Rational& operator/=(const Rational& other);

	Rational& operator=(const long long number);

	friend Rational operator+(Rational first, const Rational& second);
	friend Rational operator-(Rational first, const Rational& second);
	friend Rational operator*(Rational first, const Rational& second);
	friend Rational operator/(Rational first, const Rational& second);

	friend Rational operator+(Rational first, const long long integer);
	friend Rational operator-(Rational first, const long integer);
	friend Rational operator*(Rational first, const long long integer);
	friend Rational operator*(Rational first, const int integer);
	friend Rational operator/(Rational first, const long long integer);

	friend bool operator==(const Rational& first, const Rational& second);

	friend bool operator!=(const Rational& first, const Rational& second);

	friend bool operator==(const Rational& first, const long long integer);

	friend bool operator==(const Rational& first, const int integer);

	friend bool operator!=(const Rational& first, const long long integer);
	friend bool operator!=(const Rational& first, const int integer);

	friend bool operator>(const Rational& first, const Rational& second);

	friend bool operator<(const Rational& first, const Rational& second);

	friend bool operator>(const Rational& first, const long long number);
	friend bool operator>(const Rational& first, const int number);

	friend bool operator<(const Rational& first, const long long number);
	friend bool operator<(const Rational& first, const int number);

	friend std::ostream& operator<<(std::ostream& os, const Rational& r);

	bool isInteger() const;

	explicit operator long long() const;

	friend Rational abs(const Rational& r);

	long long getNumenator() const;

	long long getDenumenator() const;
};


