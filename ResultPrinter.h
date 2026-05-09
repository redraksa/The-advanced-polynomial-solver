#pragma once
#include <iostream>
#include <sstream>
#include <string_view>
#include "Emitter.h"

class ResultPrinter
{
private:
	std::vector<Symbol> results;

	void printPolynom(const Polynom& polynom, std::ostringstream& oss);
	void printMonomial(const Monomial& monomial, std::ostringstream& oss);

public:
	ResultPrinter(std::vector<Symbol> results) : results(std::move(results)) {}

	void printResult();
};

