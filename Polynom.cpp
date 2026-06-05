#include "Polynom.h"

Polynom Polynom::dividePolynoms(Polynom divisible, const Polynom& divider, RETURNTYPEDIVIDEPOLYNOMS returnType) {
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

Polynom Polynom::getSPolynom(const Polynom& first, const Polynom& second) {
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

Monomial Polynom::getLM() const {
	if (!lc_monomials.empty()) {
		return lc_monomials.crbegin()->first;
	}
	else {
		return Monomial();
	}
}

const Polynom* Polynom::getLCM(const std::set<Polynom>& GrobnerBasis) {
	Polynom minPolynomAbs;
	const Polynom* minPolynom = nullptr;

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

		Polynom curCoeff(polynom.getLMC());
		Polynom curPolynomAbs(polynom);
		curPolynomAbs /= curCoeff;
		if (it == polynom.lc_monomials.cend() && (minPolynom == nullptr || curPolynomAbs < minPolynomAbs)) {
			minPolynomAbs = curPolynomAbs;
			minPolynom = &polynom;
		}
	}

	return minPolynom;
}

Polynom Polynom::getNewBasisElem(const std::set<Polynom>& dividers) const {
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

const Polynom* Polynom::getDivider(const Polynom& divisible, const std::set<Polynom>& dividers) const {
	for (auto& divider : dividers) {
		if (divisible.getLM().is_divisible_by(divider.getLM())) {
			return &divider;
		}
	}
	return nullptr;
}

void Polynom::getAllGrobnerBasis(std::set<Polynom>& GrobnerBasis) {
	if (GrobnerBasis.size() >= 2) {
		std::queue<std::pair<const Polynom*, const Polynom*>> queueTasks;
		for (auto it1 = GrobnerBasis.begin(); it1 != GrobnerBasis.end(); ++it1) {
			for (auto it2 = std::next(it1); it2 != GrobnerBasis.end(); ++it2) {
				queueTasks.push({ &(*it1), &(*it2) });
			}
		}
		while (!queueTasks.empty()) {
			std::pair<const Polynom*, const Polynom*> curTask = queueTasks.front();
			queueTasks.pop();

			bool res = reduceGrobnerBasisSimpleLM(*curTask.first, *curTask.second);

			if (res) {
				continue;
			}

			res = reduceGrobnerBasisCommonIdeal(*curTask.first, *curTask.second, GrobnerBasis);

			if (res) {
				continue;
			}

			Polynom sPolynom = getSPolynom(*curTask.first, *curTask.second);
			Polynom newElem = sPolynom.getNewBasisElem(GrobnerBasis);

			if (!newElem.isZero()) {
				auto [it_new, inserted] = GrobnerBasis.insert(newElem);
				if (inserted) {
					for (auto it = GrobnerBasis.begin(); it != GrobnerBasis.end(); ++it) {
						if (it_new != it) {
							queueTasks.push({ &(*it), &(*it_new) });
						}
					}
				}
			}
		}
	}
}

bool Polynom::reduceGrobnerBasisSimpleLM(const Polynom& firstPolynom, const Polynom& secondPolynom) {

	Monomial GCDMonom = Monomial::GCD(firstPolynom.getLM(), secondPolynom.getLM());

	if (GCDMonom.isFreeMonom()) {
		return true;
	}

	return false;
}

bool Polynom::reduceGrobnerBasisCommonIdeal(const Polynom& firstPolynom, const Polynom& secondPolynom, const std::set<Polynom>& GrobnerBasis) {

	Monomial currentLCM = Monomial::LCM(firstPolynom.getLM(), secondPolynom.getLM());


	for (const auto& poly : GrobnerBasis) {


		if (firstPolynom == poly || secondPolynom == poly) {
			continue;
		}

		if (currentLCM.is_divisible_by(poly.getLM())) {
			Monomial firstLCM = Monomial::LCM(firstPolynom.getLM(), poly.getLM()),
				secondLCM = Monomial::LCM(secondPolynom.getLM(), poly.getLM());

			if (currentLCM > firstLCM && currentLCM > secondLCM) {
				return true;
			}
		}
	}

	return false;
}

bool Polynom::isZero() const {
	return lc_monomials.empty();
}

bool Polynom::isFreePolynom() const {
	return lc_monomials.size() == 1 && lc_monomials.find(Monomial()) != lc_monomials.cend();
}

Monomial Polynom::getCommonMonom() const {
	if (lc_monomials.size() > 1) {
		auto it = lc_monomials.cbegin();
		Monomial commonMonom = it->first;
		++it;
		while (it != lc_monomials.cend()) {
			commonMonom = Monomial::GCD(commonMonom, it->first);

			if (commonMonom.isFreeMonom()) {
				break;
			}

			++it;

		}
		return commonMonom;
	}
	else {
		if (!lc_monomials.empty()) {
			return lc_monomials.begin()->first;
		}
		else {
			return Monomial();
		}
	}

}

const std::map<Monomial, Rational>& Polynom::getMembers() const {
	return lc_monomials;
}

std::optional<Rational> Polynom::getFreeMember() const {
	if (lc_monomials.find(Monomial()) != lc_monomials.cend()) {
		return lc_monomials.at(Monomial());
	}
	else {
		return std::nullopt;
	}
}

bool Polynom::isZeroPolynom() const {
	return lc_monomials.empty();
}

Rational Polynom::getLMC() const {
	if (!lc_monomials.empty()) {
		return lc_monomials.crbegin()->second;
	}
	else {
		return Rational();
	}
}

size_t Polynom::size() const {
	return lc_monomials.size();
}

Polynom& Polynom::operator+=(const Polynom& other) {
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

Polynom& Polynom::operator-=(const Polynom& other) {
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
Polynom& Polynom::operator*=(const Polynom& other) {
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

Polynom& Polynom::operator/=(const Polynom& other) {
	*this = dividePolynoms(*this, other, QUOTIENT);
	return *this;
}

Polynom& Polynom::operator%=(const Polynom& other) {
	*this = dividePolynoms(*this, other, REMAINDER);
	return *this;
}

bool Polynom::operator>(const Polynom& other) const {
	auto it1 = lc_monomials.crbegin();
	auto it2 = other.lc_monomials.crbegin();

	while (it1 != lc_monomials.crend() && it2 != other.lc_monomials.crend()) {
		if (it1->first > it2->first) {
			return true;
		}
		if (it1->first < it2->first) {
			return false;
		}
		if (it1->second > it2->second) {
			return true;
		}
		if (it1->second < it2->second) {
			return false;
		}

		++it1;
		++it2;
	}

	return it1 != lc_monomials.crend();
}

bool Polynom::operator<(const Polynom& other) const {
	return other > *this;
}

bool Polynom::operator==(const Polynom& other) const {
	return lc_monomials == other.lc_monomials;
}

bool Polynom::operator!=(const Polynom& other) const {
	return lc_monomials != other.lc_monomials;
}

std::optional<Polynom> Polynom::GCD(const Polynom& first, const Polynom& second) {
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

Polynom operator/(Polynom divisible, const Polynom& divider) {
	divisible /= divider;
	return divisible;
}

Polynom operator%(Polynom divisible, const Polynom& divider) {
	divisible %= divider;
	return divisible;
}

Polynom operator+(Polynom first, const Polynom& second) {
	return first += second;
}

Polynom operator-(Polynom first, const Polynom& second) {
	return first -= second;
}

Polynom operator*(Polynom first, const Polynom& second) {
	return first *= second;
}

std::ostream& operator<<(std::ostream& os, const Polynom& polynom) {
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