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
	
	static Polynom dividePolynoms(Polynom divisible, const Polynom& divider, RETURNTYPEDIVIDEPOLYNOMS returnType);

	static Polynom getSPolynom(const Polynom& first, const Polynom& second);

	Monomial getLM() const;

	static const Polynom* getLCM(const std::set<Polynom>& GrobnerBasis);

	Polynom getNewBasisElem(const std::set<Polynom>& dividers) const;

	const Polynom* getDivider(const Polynom& divisible, const std::set<Polynom>& dividers) const;

	static void getAllGrobnerBasis(std::set<Polynom>& GrobnerBasis);

	bool isZero() const;
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

	bool isFreePolynom() const;

	Monomial getCommonMonom() const;

	const std::map<Monomial, Rational>& getMembers() const;

	std::optional<Rational> getFreeMember() const;

	bool isZeroPolynom() const;

	Rational getLMC() const;

	size_t size() const;

	Polynom& operator+=(const Polynom& other);

	Polynom& operator-=(const Polynom& other);
	Polynom& operator*=(const Polynom& other);

	Polynom& operator/=(const Polynom& other);

	Polynom& operator%=(const Polynom& other);

	bool operator>(const Polynom& other) const;

	bool operator<(const Polynom& other) const;

	bool operator==(const Polynom& other) const;

	bool operator!=(const Polynom& other) const;

	static std::optional<Polynom> GCD(const Polynom& first, const Polynom& second);

	friend Polynom operator/(Polynom divisible, const Polynom& divider);

	friend Polynom operator%(Polynom divisible, const Polynom& divider);

	friend Polynom operator+(Polynom first, const Polynom& second);

	friend Polynom operator-(Polynom first, const Polynom& second);

	friend Polynom operator*(Polynom first, const Polynom& second);

	friend std::ostream& operator<<(std::ostream& os, const Polynom& polynom);
};

