#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <map>
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
	Emitter() {
		Polynom tpolynom(Monomial(0), Rational(1));
		symbolicTable.push_back({ "_t", MixedFraction(tpolynom) });
	}

	std::vector<Symbol> getResult() const;

	void normilizeAnswer();

	void addition();

	void substraction();

	void multiplication();

	void division();

	void power();

	void pushID(const char* name);

	void pushNumber(long long num);

	void pushZero();

	void assignID(const char* name);

private:
	
	std::stack<MixedFraction> polynomials;
	std::vector<Symbol> symbolicTable;

	
};

