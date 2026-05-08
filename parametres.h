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

	inline std::string CHAR_DIVIDERS = { ' ', '\t' , '\r', '\n'};
	inline std::string CHAR_END = { EOF };

	enum TokenType
	{
		PLUS, MINUS, MUL, DIV, POWER, OPENBACKET, CLOSEBACKET, ASSIGN, SEMICOLON, ENDENUM, NUM, ID, END, NOTYPE, ERRORTOKEN
	};

	struct Lexem {
		TokenType token;
		std::string value;
		Lexem() {
			token = NOTYPE;
			value = "";
		}
	};

	inline std::vector<std::string> VARS = { "", "x", "y", "z" };

}