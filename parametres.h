#pragma once
#include <string>
#include <map>
#include <vector>
namespace parameters {
	enum COMPARES {
		GREATER, SMALLER, EQUAL
	};

	enum RETURNTYPEDIVIDEPOLYNOMS {
		REMAINDER, QUOTIENT
	};

	inline std::string CHAR_DIVIDERS = { ' ', '\t' };
	inline std::string CHAR_END = { EOF };

	enum TokenType
	{
		PLUS, MINUS, MUL, DIV, POWER, OPENBACKET, CLOSEBACKET, ASSIGN, SEMICOLON, ENDLINE, ENDENUM, NUM, ID, END, NOTYPE, ERRORTOKEN
	};

	struct Lexem {
		TokenType token;
		std::string value;
		Lexem() {
			token = NOTYPE;
			value = "";
		}
	};



	//typedef std::map<std::string, long long> Monomial;

	//typedef std::map<long long, Rational> Polynom;

	//typedef std::map<std::map<std::string, long long>, long long> Polynom;

	inline std::vector<std::string> VARS = { "", "x", "y", "z" };

}