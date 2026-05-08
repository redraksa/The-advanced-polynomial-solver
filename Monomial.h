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
	
	Monomial& operator*=(const Monomial& other) {
		for (auto& [var, power] : other.vars) {
			vars[var] += power;
		}

		return *this;
	}

	std::map<int, long long> getVars() const {
		return vars;
	}

	static Monomial LCM(const Monomial& first, const Monomial& second) {
		Monomial result = first;
		for (auto& [var, power] : second.vars) {
			result.vars[var] = std::max(result.vars[var], power);
		}
		return result;
	}

	friend Monomial operator*(Monomial first, const Monomial& second) {
		first *= second;
		return first;
	}

	Monomial& operator/=(const Monomial& other) {
		for (auto& [var, power] : other.vars) {
			vars[var] -= power;
			if (vars[var] == 0) {
				vars.erase(var);
			}
		}

		return *this;
	}

	bool isFreeMonom() const {
		return vars.empty();
	}

	bool hasVar(const int var) const {
		return vars.find(var) != vars.cend();
	}

	friend Monomial operator/(Monomial first, const Monomial& second) {
		first /= second;
		return first;
	}

	bool is_divisible_by(const Monomial& other) const {
		for (auto& [var, power] : other.vars) {
			auto it = vars.find(var);
			if (it == vars.cend()) {
				return false;
			}
			if (it->second < power) {
				return false;
			}
		}

		return true;
	}

	bool operator<(const Monomial& other) const {
		auto it1 = vars.cbegin();
		auto it2 = other.vars.cbegin();

		while (it1 != vars.cend() && it2 != other.vars.cend()) {
			if (it1->first != it2->first) {
				return it1->first > it2->first;
			}
			if (it1->second != it2->second) {
				return it1->second < it2->second;
			}
			++it1;
			++it2;
		}

		return it1 == vars.cend() && it2 != other.vars.cend();
	}

	bool operator>(const Monomial& other) const {
		return other < *this;
	}

	bool operator==(const Monomial& other) const {
		return vars == other.vars;
	}

	bool operator!=(const Monomial& other) const {
		return vars != other.vars;
	}

	bool operator>=(const Monomial& other) const {
		return !(*this < other);
	}

	bool operator<=(const Monomial& other) const {
		return !(*this > other);
	}

	friend std::ostream& operator<<(std::ostream& os, const Monomial& monom) {
		if (!monom.vars.empty()) {
			for (auto& [var, power] : monom.vars) {
				os << VARS[var];
				if (power > 1) {
					os << "^" << power;
				}
			}
		}
		return os;
	}
};

