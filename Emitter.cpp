#include "Emitter.h"

void Emitter::trasmittion(std::queue<Lexem>& queueLexem, std::stack<MixedFraction>& polynomials) {

	Lexem curLexem;
	while (!queueLexem.empty()) {
		curLexem = queueLexem.front();
		queueLexem.pop();

		switch (curLexem.token)
		{
		case ID:
		{
			auto it = std::find_if(symbolicTable.cbegin(), symbolicTable.cend(),
				[&](const Symbol& s) {return s.name == curLexem.value; });
			if (it != symbolicTable.cend()) {
				polynomials.push({ it->value });
			}
			else {
				Monomial m(static_cast<const int>(symbolicTable.size()));
				Polynom p(m, 1);
				MixedFraction mx(p);
				symbolicTable.push_back({ curLexem.value, mx });
				polynomials.push({ mx });
			}
			break;
		}
		case NUM:
		{
			long long number = std::stoll(curLexem.value);
			polynomials.push(MixedFraction(Polynom(Rational(number))));
			break;
		}
		case PLUS:
			addition(polynomials);
			break;
		case MINUS:
			substraction(polynomials);
			break;
		case MUL:
			multiplication(polynomials);
			break;
		case DIV:
			division(polynomials);
			break;
		case POWER:
			power(polynomials);
			break;
		default:
			break;
		}
	}

	if (!polynomials.empty()) {
		polynomials.top().normilize();
	}
}

std::vector<Symbol> Emitter::getResult() const {
	if (symbolicTable.size() >= 1) {
		return std::vector<Symbol>(symbolicTable.cbegin() + 1, symbolicTable.cend());
	}
	return {};
}

void Emitter::launch() {
	try {
		while (!queuesLexem.empty()) {
			std::pair<std::string, std::queue<Lexem>> line;
			line = queuesLexem.front();
			queuesLexem.pop();

			std::stack<MixedFraction> polynomials;
			std::queue<Lexem> queueLexem = line.second;

			trasmittion(queueLexem, polynomials);

			MixedFraction result;
			if (!polynomials.empty()) {
				result = polynomials.top();
				polynomials.pop();
			}

			auto it = std::find_if(symbolicTable.begin(), symbolicTable.end(),
				[&](const Symbol& s) {return s.name == line.first; });
			if (it == symbolicTable.cend()) {
				symbolicTable.push_back({ line.first, result });
			}
			else {
				it->value = result;
			}

		}
	}
	catch (const std::overflow_error& e) {
		throw std::overflow_error(std::string("Overflow Error in Emitter: ") + e.what());
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(std::string("Runtime Error in Emitter: ") + e.what());
	}
	catch (...) {
		throw std::runtime_error("Undefined Error in Emitter: ");
	}

}

void Emitter::addition(std::stack<MixedFraction>& polynomials) {
	MixedFraction firstPolinom = polynomials.top();
	polynomials.pop();

	polynomials.top() += firstPolinom;
}

void Emitter::substraction(std::stack<MixedFraction>& polynomials) {
	MixedFraction firstPolinom = polynomials.top();
	polynomials.pop();

	polynomials.top() -= firstPolinom;
}

void Emitter::multiplication(std::stack<MixedFraction>& polynomials) {
	MixedFraction firstPolinom = polynomials.top();
	polynomials.pop();

	polynomials.top() *= firstPolinom;

}

void Emitter::division(std::stack<MixedFraction>& polynomials) {
	MixedFraction firstPolinom = polynomials.top();
	polynomials.pop();

	if (!firstPolinom.isZero()) {
		polynomials.top() /= firstPolinom;
	}
	else {
		throw std::runtime_error("zero division");
	}


}

void Emitter::power(std::stack<MixedFraction>& polynomials) {
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
