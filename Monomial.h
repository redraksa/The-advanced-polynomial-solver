#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "parametres.h"

using namespace parameters;

class Monomial
{
private:
	std::map<int, long long> vars;

public:
	Monomial(const int var) {
		vars[var] = 1;
	}

	Monomial() = default;
	
	Monomial& operator*=(const Monomial& other);

	std::map<int, long long> getVars() const;

	static Monomial LCM(const Monomial& first, const Monomial& second);

	static Monomial GCD(const Monomial& first, const Monomial& second);

	friend Monomial operator*(Monomial first, const Monomial& second);

	Monomial& operator/=(const Monomial& other);

	bool isFreeMonom() const;

	bool hasVar(const int var) const;

	friend Monomial operator/(Monomial first, const Monomial& second);

	bool is_divisible_by(const Monomial& other) const;

	bool operator<(const Monomial& other) const;

	bool operator>(const Monomial& other) const;

	bool operator==(const Monomial& other) const;

	bool operator!=(const Monomial& other) const;

	bool operator>=(const Monomial& other) const;

	bool operator<=(const Monomial& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Monomial& monom);
};

