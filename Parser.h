#pragma once
#include <iostream>
#include "Lexer.h"
#include <queue>
#include <memory>
#include "parametres.h"

using namespace parameters;

class Parser {
private:
    Lexem nextLexem;
 
    Lexer lexer;
    std::queue<std::pair<std::string, std::queue<Lexem>>> queueCommand;
	std::string fileName;

    void match(TokenType token);

    void unar(std::queue<Lexem>& stackLexem);

    void factor(std::queue<Lexem>& stackLexem);

    void lines();

    void line();

    void expr(std::queue<Lexem>& stackLexem);

    void term(std::queue<Lexem>& stackLexem);

    void power(std::queue<Lexem>& stackLexem);

    void pushToken(const std::string& token);

    void pushToken(std::queue<Lexem>& stackLexem, const Lexem& lexem);

public:
    Parser(const std::string& fileName) : nextLexem{}, lexer(), fileName(fileName) {}

    void translate();

    std::queue<std::pair<std::string, std::queue<Lexem>>> getStackLexem() const;
};

