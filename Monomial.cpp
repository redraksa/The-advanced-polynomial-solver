#include "Monomial.h"

Monomial& Monomial::operator*=(const Monomial& other) {
	for (auto& [var, power] : other.vars) {
		vars[var] += power;
	}

	return *this;
}

std::map<int, long long> Monomial::getVars() const {
	return vars;
}

Monomial Monomial::LCM(const Monomial& first, const Monomial& second) {
	Monomial result = first;
	for (auto& [var, power] : second.vars) {
		result.vars[var] = std::max(result.vars[var], power);
	}
	return result;
}

Monomial Monomial::GCD(const Monomial& first, const Monomial& second) {
	return first * second / LCM(first, second);

}

Monomial operator*(Monomial first, const Monomial& second) {
	first *= second;
	return first;
}

Monomial& Monomial::operator/=(const Monomial& other) {
	for (auto& [var, power] : other.vars) {
		vars[var] -= power;
		if (vars[var] == 0) {
			vars.erase(var);
		}
	}

	return *this;
}

bool Monomial::isFreeMonom() const {
	return vars.empty();
}

bool Monomial::hasVar(const int var) const {
	return vars.find(var) != vars.cend();
}

Monomial operator/(Monomial first, const Monomial& second) {
	first /= second;
	return first;
}

bool Monomial::is_divisible_by(const Monomial& other) const {
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

bool Monomial::operator<(const Monomial& other) const {
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

bool Monomial::operator>(const Monomial& other) const {
	return other < *this;
}

bool Monomial::operator==(const Monomial& other) const {
	return vars == other.vars;
}

bool Monomial::operator!=(const Monomial& other) const {
	return vars != other.vars;
}

bool Monomial::operator>=(const Monomial& other) const {
	return !(*this < other);
}

bool Monomial::operator<=(const Monomial& other) const {
	return !(*this > other);
}

std::ostream& operator<<(std::ostream& os, const Monomial& monom) {
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