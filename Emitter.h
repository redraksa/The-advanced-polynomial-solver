#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include "Lexer.h"
#include <map>
#include <functional>
#include "Fraction.h"
#include "MixedFraction.h"
#include "parametres.h"

using namespace parameters;

struct Symbol {
	std::string name;
	MixedFraction value;
};

class Emitter
{
public:
	Emitter(std::queue<std::pair<std::string, std::queue<Lexem>>> queuesLexem) : queuesLexem(std::move(queuesLexem)) {
		Polynom tpolynom(Monomial(0), Rational(1));
		symbolicTable.push_back({ "t", MixedFraction(tpolynom)});
	}

	void trasmittion(std::queue<Lexem>& queueLexem, std::stack<MixedFraction>& polynomials);

	std::vector<Symbol> getResult() const;

	void launch();

private:
	std::queue<std::pair<std::string, std::queue<Lexem>>> queuesLexem;
	std::vector<Symbol> symbolicTable;

	void addition(std::stack<MixedFraction>& polynomials);

	void substraction(std::stack<MixedFraction>& polynomials);

	void multiplication(std::stack<MixedFraction>& polynomials);

	void division(std::stack<MixedFraction>& polynomials);

	void power(std::stack<MixedFraction>& polynomials);
};

