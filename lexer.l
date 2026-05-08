%{
#include <iostream>
#include <string>
#include <stdexcept>
#include "parser.tab.hpp"
extern FILE* yyin;
%}

%option noyywrap
%option nounput
%option noinput

%%
[ \t\r\n]		;
<<EOF>>		{
			return END;
		}
[1-9][0-9]*|0	{
			try {
				yylval.value = std::stoll(yytext);
				return NUM;
			}
			catch (const std::out_of_range& e) {
				throw std::out_of_range("Number is too large: " + std::string(yytext));
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Invalid number: " + std::string(yytext));
			}
		}
[a-zA-Z][a-zA-Z0-9_]*	{
				yylval.s_value = new std::string(yytext);
				return ID;
			} 
"+"			{return PLUS;}
"-" 			{return MINUS;}
"*"			{return MUL;}
"/"			{return DIV;}
"^"			{return POWER;}
"("			{return OPENBACKET;}
")"			{return CLOSEBACKET;}
";"			{return SEMICOLON;}
"="			{return ASSIGN;}

.			{
				throw std::runtime_error("Unknown character: " + std::string(yytext));
			}

%%
