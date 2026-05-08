#pragma once

#include <iostream>
#include "parametres.h"
#include <map>
#include "Monomial.h"
#include <set>
#include "Rational.h"
#include <queue>
#include <optional>

using namespace parameters;

class Polynom
{
private:
	std::map<Monomial, Rational> lc_monomials;
	
	Polynom dividePolynoms(Polynom divisible, const Polynom& divider, RETURNTYPEDIVIDEPOLYNOMS returnType) {
		if (divider.lc_monomials.empty()) {
			throw std::runtime_error("Division by zero");
		}

		Polynom quotient, remainder;
		auto lt_g_monom = divider.lc_monomials.rbegin()->first;
		auto lt_g_coeff = divider.lc_monomials.rbegin()->second;
		while (!divisible.lc_monomials.empty()) {
			auto it_f = divisible.lc_monomials.rbegin();
			if (it_f->first.is_divisible_by(lt_g_monom)) {
				Monomial m_q = it_f->first / lt_g_monom;
				Rational c_q = it_f->second / lt_g_coeff;

				quotient.lc_monomials[m_q] += c_q;
				if (quotient.lc_monomials[m_q] == 0) {
					quotient.lc_monomials.erase(m_q);
				}

				Polynom step(m_q, c_q);
				divisible -= (step * divider);
			}
			else {
				remainder.lc_monomials[it_f->first] = it_f->second;
				Polynom term(it_f->first, it_f->second);
				divisible -= term;
			}
		}

		return (returnType == REMAINDER) ? remainder : quotient;
	}

	static Polynom getSPolynom(const Polynom& first, const Polynom& second) {
		if (first.isZeroPolynom() || second.isZeroPolynom()) {
			return Polynom(Rational(0));
		}

		Monomial LMfirst = first.getLM();
		Monomial LMsecond = second.getLM();
		Polynom lcmPolynom(Monomial::LCM(LMfirst, LMsecond), 1);

		Polynom LTfirst(LMfirst, first.getLMC());
		Polynom LTsecond(LMsecond, second.getLMC());

		return first * (lcmPolynom / LTfirst) - second * (lcmPolynom / LTsecond);

	}

	Monomial getLM() const {
		if (!lc_monomials.empty()) {
			return lc_monomials.crbegin()->first;
		}
		else {
			return Monomial();
		}
	}

	static const Polynom* getLCM(const std::set<Polynom>& GrobnerBasis) {
		for (auto& polynom : GrobnerBasis) {
			if (polynom.isZeroPolynom()) {
				continue;
			}

			auto it = polynom.lc_monomials.cbegin();
			while (it != polynom.lc_monomials.cend()) {
				if (it->first.hasVar(0)) {
					break;
				}
				++it;
			}
			
			if (it == polynom.lc_monomials.cend()) {
				return &polynom;
			}
		}

		return nullptr;
	}

	Polynom getNewBasisElem(const std::set<Polynom>& dividers) const {
		if (!dividers.empty() && !lc_monomials.empty()) {
			Polynom tempPolynom = *this, remainder;
			while (!tempPolynom.isZero()) {
				const Polynom* Divider = getDivider(tempPolynom, dividers);
				if (Divider != nullptr) {
					Polynom tempQuotient(tempPolynom.getLM() / Divider->getLM(), tempPolynom.getLMC() / Divider->getLMC());
					tempPolynom -= tempQuotient * *Divider;
				}
				else {
					Polynom LT(tempPolynom.getLM(), tempPolynom.getLMC());
					remainder += LT;
					tempPolynom -= LT;
				}
			}
			return remainder;
		}
		else {
			return *this;
		}
	}

	const Polynom* getDivider(const Polynom& divisible, const std::set<Polynom>& dividers) const {
		for (auto& divider : dividers) {
			if (divisible.getLM().is_divisible_by(divider.getLM())) {
				return &divider;
			}
		}
		return nullptr;
	}

	static void getAllGrobnerBasis(std::set<Polynom>& GrobnerBasis) {
		if (GrobnerBasis.size() >= 2) {
			std::queue<std::pair<const Polynom*, const Polynom*>> queueTasks;
			for (auto it1 = GrobnerBasis.begin(); it1 != GrobnerBasis.end(); ++it1) {
				for (auto it2 = std::next(it1); it2 != GrobnerBasis.end(); ++it2) {
					queueTasks.push({&(*it1), &(*it2)});
				}
			}
			while (!queueTasks.empty()) {
				std::pair<const Polynom*, const Polynom*> curTask = queueTasks.front();
				queueTasks.pop();
				Polynom sPolynom = getSPolynom(*curTask.first, *curTask.second);
				Polynom newElem = sPolynom.getNewBasisElem(GrobnerBasis);

				if (!newElem.isZero()) {
					auto [it_new, inserted] = GrobnerBasis.insert(newElem);
					if (inserted) {
						for (auto it = GrobnerBasis.begin(); it != GrobnerBasis.end(); ++it) {
							if (it_new != it) {
								queueTasks.push({&(*it), &(*it_new)});
							}
						}
					}
				}
			}
		}
	}

	bool isZero() const {
		return lc_monomials.empty();
	}
public:
	Polynom() = default;

	Polynom(Monomial monomial, Rational coeff) {
		if (coeff != 0) {
			lc_monomials[monomial] = coeff;
		}
	}

	Polynom(Rational coeff) {
		if (coeff != 0) {
			lc_monomials[Monomial()] = coeff;
		}
	}

	bool isFreePolynom() const {
		return lc_monomials.size() == 1 && lc_monomials.find(Monomial()) != lc_monomials.cend();
	}

	const std::map<Monomial, Rational>& getMembers() const {
		return lc_monomials;
	}	

	std::optional<Rational> getFreeMember() const {
		if (lc_monomials.find(Monomial()) != lc_monomials.cend()) {
			return lc_monomials.at(Monomial());
		}
		else {
			return std::nullopt;
		}
	}

	bool isZeroPolynom() const {
		return lc_monomials.empty();
	}

	Rational getLMC() const {
		if (!lc_monomials.empty()) {
			return lc_monomials.crbegin()->second;
		}
		else {
			return Rational();
		}
	}

	size_t size() const {
		return lc_monomials.size();
	}

	Polynom& operator+=(const Polynom& other) {
		for (const auto& [member, coeff] : other.lc_monomials) {
			if (lc_monomials.find(member) != lc_monomials.cend()) {
				lc_monomials[member] += coeff;
				if (lc_monomials[member] == 0) {
					lc_monomials.erase(member);
				}
			}
			else {
				lc_monomials[member] = coeff;
			}
		}

		return *this;
	}

	Polynom& operator-=(const Polynom& other) {
		for (const auto& [member, coeff] : other.lc_monomials) {
			if (lc_monomials.find(member) != lc_monomials.cend()) {
				lc_monomials[member] -= coeff;
				if (lc_monomials[member] == 0) {
					lc_monomials.erase(member);
				}
			}
			else {
				lc_monomials[member] = -coeff;
			}
		}

		return *this;
	}
	Polynom& operator*=(const Polynom& other) {
		std::map<Monomial, Rational> result;

		for (const auto& [member1, coeff1] : other.lc_monomials) {
			for (auto& [member2, coeff2] : lc_monomials) {
				Monomial resultMember = member1 * member2;
				Rational resultCoeff = coeff1 * coeff2;
				result[resultMember] += resultCoeff;
				if (result[resultMember] == 0) {
					result.erase(resultMember);
				}
			}
		}

		lc_monomials = std::move(result);

		return *this;
	}

	Polynom& operator/=(const Polynom& other) {
		*this = dividePolynoms(*this, other, QUOTIENT);
		return *this;
	} 

	Polynom& operator%=(const Polynom& other) {
		*this = dividePolynoms(*this, other, REMAINDER);
		return *this;
	}

	bool operator>(const Polynom& other) const {
		return lc_monomials > other.lc_monomials;
	}

	bool operator<(const Polynom& other) const {
		return lc_monomials < other.lc_monomials;
	}

	bool operator==(const Polynom& other) const {
		return lc_monomials == other.lc_monomials;
	}

	bool operator!=(const Polynom& other) const {
		return lc_monomials != other.lc_monomials;
	}

	static std::optional<Polynom> GCD(const Polynom& first, const Polynom& second) {
		Polynom a(first);
		Polynom b(second);

		std::set<Polynom> GrobnerBasis;
		Polynom tPolynom(0, 1);
		Polynom one(Monomial(), 1);
		GrobnerBasis.insert(a * tPolynom);
		GrobnerBasis.insert(b * (one - tPolynom));

		getAllGrobnerBasis(GrobnerBasis);

		const Polynom* lcm = getLCM(GrobnerBasis);
		if (lcm != nullptr) {
			Polynom result = a * b / *lcm;
			return result / result.getLMC();
		}
		else {
			return std::nullopt;
		}

	}

	friend Polynom operator/(Polynom divisible, const Polynom& divider) {
		divisible /= divider;
		return divisible;
	}

	friend Polynom operator%(Polynom divisible, const Polynom& divider) {
		divisible %= divider;
		return divisible;
	}

	friend Polynom operator+(Polynom first, const Polynom& second) {
		return first += second;
	}

	friend Polynom operator-(Polynom first, const Polynom& second) {
		return first -= second;
	}

	friend Polynom operator*(Polynom first, const Polynom& second) {
		return first *= second;
	}

	friend std::ostream& operator<<(std::ostream& os, const Polynom& polynom) {
		if (!polynom.lc_monomials.empty()) {
			auto it = polynom.lc_monomials.crbegin();
			if (it->second != 1 || it->first.isFreeMonom()) {
				os << it->second;
			}
			os << it->first;
			++it;
			while (it != polynom.lc_monomials.crend()) {
				os << " + ";
				if (it->second != 1 || it->first.isFreeMonom()) {
					os << it->second;
				}
				os << it->first;
				++it;
			}
		}
		else {
			os << "0";
		}
		return os;
	}
};

