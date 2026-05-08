#pragma once
#include <iostream>
#include <numeric>
#include <ostream>

class Rational
{
private:
	long long numenator;
	long long denumenator;

	void reduceFraction() {
		long long gcd = std::gcd(numenator, denumenator);
		if (gcd != 0) {
			numenator /= gcd;
			denumenator /= gcd;
		}

		if ((numenator > 0 && denumenator < 0) || (numenator < 0 && denumenator < 0)) {
			numenator = saveMul(numenator, -1);
			denumenator = saveMul(denumenator, -1);
		}
	}

	
	static long long saveAdd(const long long first, const long long second) {
		long long result;
		
		if (__builtin_add_overflow(first, second, &result)) {
			throw std::overflow_error("Addition overflow: " + std::to_string(first) + "+" + std::to_string(second));
		}

		return result;
	}

	static long long saveSub(const long long first, const long long second) {
		long long result;
		
		if (__builtin_sub_overflow(first, second, &result)) {
			throw std::overflow_error("Substraction overflow: " + std::to_string(first) + "-" + std::to_string(second));
		}

		return result;
	}

	
public:
	Rational() : numenator(0), denumenator(1) {}

	Rational(long long numenator, long long denumenator) : numenator(numenator), denumenator(denumenator) {
		if (denumenator == 0) {
			throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
		}
		reduceFraction();
	}

	Rational(long long number) : numenator(number), denumenator(1) {}
	
	static long long saveMul(const long long first, const long long second) {
		long long result;
		
		if (__builtin_mul_overflow(first, second, &result)) {
			throw std::overflow_error("Multiplication overflow: " + std::to_string(first) + "*" + std::to_string(second));
		}

		return result;
	}

	static long long savePow(long long base, long long exp) {
		if (exp < 0) throw std::domain_error("Only natural exponents are supported");
		if (exp == 0) return 1;
		if (base == 0) return 0;
		if (base == 1) return 1;
		if (base == -1) return (exp % 2 == 0) ? 1 : -1;

		long long orig_base = base;
		long long orig_exp = exp;
		long long res = 1;

		try {
			while (exp > 0) {
				if (exp % 2 == 1) {
					res = saveMul(res, base);
				}
				if (exp > 1) {
					base = saveMul(base, base);
				}
				exp /= 2;
			}
		}
		catch (const std::overflow_error&) {
			throw std::overflow_error("Exponentiation overflow: " + std::to_string(orig_base) + "^" + std::to_string(orig_exp));
		}
		return res;
	}
	
	Rational& operator+=(const Rational& other) {

		long long commonDenumenator = saveMul(denumenator, other.denumenator);
		long long newFirstNumenator = saveMul(numenator, other.denumenator);
		long long newSecondNumenator = saveMul(other.numenator, denumenator);
		long long newNumenator = saveAdd(newFirstNumenator, newSecondNumenator);
		
		numenator = newNumenator;
		denumenator = commonDenumenator;

		if (denumenator == 0) {
			throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
		}

		reduceFraction();

		return *this;
	}

	Rational& operator-=(const Rational& other) {
		long long commonDenumenator = saveMul(denumenator, other.denumenator);
		long long newFirstNumenator = saveMul(numenator, other.denumenator);
		long long newSecondNumenator = saveMul(other.numenator, denumenator);
		long long newNumenator = saveSub(newFirstNumenator, newSecondNumenator);
				
		numenator = newNumenator;
		denumenator = commonDenumenator;

		if (denumenator == 0) {
			throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
		}

		reduceFraction();

		return *this;
	}

	Rational operator-() const {
		Rational temp = *this;
		temp *= -1;
		return temp;
	}

	Rational& operator*=(const Rational& other) {
		numenator = saveMul(numenator, other.numenator);
		denumenator = saveMul(denumenator, other.denumenator);

		if (denumenator == 0) {
			throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
		}

		reduceFraction();

		return *this;
	}

	Rational& operator/=(const Rational& other) {		
		numenator = saveMul(numenator, other.denumenator);
		denumenator = saveMul(denumenator, other.numenator);

		reduceFraction();

		if (denumenator == 0) {
			throw std::runtime_error("Denumenator of coefficient can't be equal to 0");
		}

		return *this;
	}

	Rational& operator=(const long long number) {
		numenator = number;
		denumenator = 1;
		return *this;
	}

	friend Rational operator+(Rational first, const Rational& second) {
		return first += second;
	}
	friend Rational operator-(Rational first, const Rational& second) {
		return first -= second;
	}
	friend Rational operator*(Rational first, const Rational& second) {
		return first *= second;
	}
	friend Rational operator/(Rational first, const Rational& second) {
		return first /= second;
	}

	friend Rational operator+(Rational first, const long long integer) {
		Rational second(integer);
		return first += second;
	}
	friend Rational operator-(Rational first, const long integer) {
		Rational second(integer);
		return first -= second;
	}
	friend Rational operator*(Rational first, const long long integer) {
		Rational second(integer);
		return first *= second;
	}
	friend Rational operator*(Rational first, const int integer) {
		return first * (long long)integer;
	}
	friend Rational operator/(Rational first, const long long integer) {
		Rational second(integer);
		return first /= second;
	}

	friend bool operator==(const Rational& first, const Rational& second) {
		long long firstMul = saveMul(first.numenator, second.denumenator);
		long long secondMul = saveMul(second.numenator, first.denumenator);
		
		return firstMul == secondMul;
	}

	friend bool operator!=(const Rational& first, const Rational& second) {
		return !(first == second);
	}

	friend bool operator==(const Rational& first, const long long integer) {
		Rational other(integer);
		return first == other;
	}

	friend bool operator==(const Rational& first, const int integer) {
		return first == (long long) integer;
	}

	friend bool operator!=(const Rational& first, const long long integer) {
		return !(first == integer);
	}
	friend bool operator!=(const Rational& first, const int integer) {
		return first != (long long) integer;
	}
	
	friend bool operator>(const Rational& first, const Rational& second) {
		long long firstMul = saveMul(first.numenator, second.denumenator);
		long long secondMul = saveMul(second.numenator, first.denumenator);
		
		return firstMul > secondMul;		
	}

	friend bool operator<(const Rational& first, const Rational& second) {
		long long firstMul = saveMul(first.numenator, second.denumenator);
		long long secondMul = saveMul(second.numenator, first.denumenator);
		
		return firstMul < secondMul;		

	}

	friend bool operator>(const Rational& first, const long long number) {
		Rational second(number);
		return first > second;
	}
	friend bool operator>(const Rational& first, const int number) {
		return first > (long long) number;
	}

	friend bool operator<(const Rational& first, const long long number) {
		Rational second(number);
		return first < second;
	}
	friend bool operator<(const Rational& first, const int number) {
		return first < (long long) number;
	}

	friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
		os << r.numenator;
		if (r.denumenator != 1) {
			os << "/" << r.denumenator;
		}

		return os;
	}

	bool isInteger() const {
		return denumenator == 1;
	}

	explicit operator long long() const {
		return numenator / denumenator;
	}

	friend Rational abs(const Rational& r) {
		return Rational(std::abs(r.numenator), std::abs(r.denumenator));
	}

	long long getNumenator() const {
		return numenator;
	}

	long long getDenumenator() const {
		return denumenator;
	}
};


