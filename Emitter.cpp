#include "Emitter.h"

std::vector<Symbol> Emitter::getResult() const {
	if (symbolicTable.size() >= 1) {
		return std::vector<Symbol>(symbolicTable.cbegin() + 1, symbolicTable.cend());
	}
	return {};
}

void Emitter::normilizeAnswer() {
	if (!polynomials.empty()) {
		polynomials.top().normilize();
	}
}

void Emitter::addition() {
	MixedFraction firstPolinom = polynomials.top();
	polynomials.pop();

	polynomials.top() += firstPolinom;
}

void Emitter::substraction() {
	MixedFraction firstPolinom = polynomials.top();
	polynomials.pop();

	polynomials.top() -= firstPolinom;
}

void Emitter::multiplication() {
	MixedFraction firstPolinom = polynomials.top();
	polynomials.pop();

	polynomials.top() *= firstPolinom;

}

void Emitter::division() {
	MixedFraction firstPolinom = polynomials.top();
	polynomials.pop();

	if (!firstPolinom.isZero()) {
		polynomials.top() /= (firstPolinom);
	}
	else {
		throw std::runtime_error("zero division");
	}


}

void Emitter::power() {
	MixedFraction firstFraction = polynomials.top();
	polynomials.pop();

	if (firstFraction.isInteger()) {
		long long power = firstFraction.getFreeMember();
		if (power > 0) {
			MixedFraction multi = polynomials.top();
			for (long long i = 1; i < power; ++i) {
				polynomials.top() *= multi;
			}
		}
		else if (power < 0) {
			MixedFraction divider = polynomials.top();
			polynomials.pop();
			polynomials.push({ { 1 } });
			for (long long i = 0; i > power; --i) {
				polynomials.top() /= divider;
			}
		}
		else {
			polynomials.pop();
			polynomials.push({ {1} });
		}
	}
	else {
		throw std::runtime_error("Non-integer power");
	}
}

void Emitter::pushID(const char* name) {
	std::string strName(name);
	auto it = std::find_if(symbolicTable.cbegin(), symbolicTable.cend(),
		[&](const Symbol& s) {return s.name == strName; });
	if (it != symbolicTable.cend()) {
		polynomials.push({ it->value });
	}
	else {
		Monomial m(static_cast<const int>(symbolicTable.size()));
		Polynom p(m, 1);
		MixedFraction mx(p);
		symbolicTable.push_back({ strName, mx });
		polynomials.push({ mx });
	}
}

void Emitter::pushNumber(long long num) {
	polynomials.push(MixedFraction(Polynom(Rational(num))));
}

void Emitter::pushZero() {
	polynomials.push(MixedFraction(Polynom(Rational(0))));
}

void Emitter::assignID(const char* name) {
	normilizeAnswer();

	MixedFraction result;
	if (!polynomials.empty()) {
		result = polynomials.top();
		polynomials.pop();
	}

	auto it = std::find_if(symbolicTable.begin(), symbolicTable.end(),
		[&](const Symbol& s) {return s.name == std::string(name); });
	if (it != symbolicTable.cend()) {
		it->value = result;
	}
	else {
		Symbol symbol{ std::string(name), result };
		symbolicTable.push_back(symbol);
	}

}