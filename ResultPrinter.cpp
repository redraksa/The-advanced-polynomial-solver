#include "ResultPrinter.h"

void ResultPrinter::printPolynom(const Polynom& polynom, std::ostringstream& oss) {
	for (auto it = polynom.getMembers().crbegin(); it != polynom.getMembers().crend(); ++it) {
		if (it != polynom.getMembers().crbegin()) {
			if (it->second > 0) {
				oss << " + ";
			}
			else {
				oss << " - ";
			}
		}
		else {
			if (it->second < 0) {
				oss << "-";
			}
		}
		if (abs(it->second) != 1 || it->first.isFreeMonom()) oss << abs(it->second);
		if (it->first != 0) {
			printMonomial(it->first, oss);
		}
	}
}

void ResultPrinter::printMonomial(const Monomial& monomial, std::ostringstream& oss) {
	if (!monomial.isFreeMonom()) {
		for (auto& [var, power] : monomial.getVars()) {
			oss << results[var - 1].name;
			if (power > 1) {
				oss << "^" << power;
			}
		}
	}
}

void ResultPrinter::printResult() {
	for (auto& [var, result] : results) {
		std::ostringstream ossVar;
		std::ostringstream ossWhole;
		std::ostringstream ossFraction;
		ossVar << var;

		if (result.isZero()) {
			printPolynom(result.getWholePart(), ossWhole);
			//std::cout << result.getWholePart();
		}
		else {
			bool isFraction = result.isFraction();
			if (!result.isZeroWhole()) {
				printPolynom(result.getWholePart(), ossWhole);
				//std::cout << result.getWholePart();
				if (isFraction) {
					ossWhole << " + ";
				}
			}
			if (isFraction) {
				if (result.getFractionPart().getNumerator().size() > 1) {
					ossFraction << "(";
					printPolynom(result.getFractionPart().getNumerator(), ossFraction);
					//std::cout << result.getFractionPart().getNumerator();
					ossFraction << ")";
				}
				else {
					printPolynom(result.getFractionPart().getNumerator(), ossFraction);
					//std::cout << result.getFractionPart().getNumerator();
				}
				ossFraction << " / ";
				if (result.getFractionPart().getDenumenator().size() != 1) {
					ossFraction << "(";
					printPolynom(result.getFractionPart().getDenumenator(), ossFraction);
					//std::cout << result.getFractionPart().getDenumenator();
					ossFraction << ")";
				}
				else {
					printPolynom(result.getFractionPart().getDenumenator(), ossFraction);
					//std::cout << result.getFractionPart().getDenumenator();
				}
			}
		}

		std::string strV = ossVar.str();
		std::string strW = ossWhole.str();
		std::string strF = ossFraction.str();

		if (!(strV == strW && strF.empty())) {
			if (strW.length() > 2 && !strF.empty()) {
				std::string_view svVW(strW);
				std::string_view svF(strF);

				auto subViewVW = svVW.substr(svVW.length() - 2);
				auto subViewF = svF.substr(0, 1);

				if (subViewVW == "+ " && subViewF == "-") {
					strW.erase(strW.length() - 2);
					strW += "- ";

					strF.erase(0, 1);
				}
			}

			std::cout << strV << " = " << strW << strF << "\n";
		}
	}
}
