#pragma once
#include <iostream>
#include <string>
#include "Init.h"
#include <fstream>

#include "parametres.h"

using namespace parameters;

class Lexer
{
private:
    Lexem lexem;
    char lookahead;
    std::ifstream file;

    void nextLexem();

public:
	Lexer() : lexem{}, lookahead('\0') {}

    const Lexem& getLexem();

    void openFile(const std::string& fileName);
};

