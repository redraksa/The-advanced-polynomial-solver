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

    void match(TokenType token) {
        if (nextLexem.token == token) {
            nextLexem = lexer.getLexem();
        }
        else {
            throw std::runtime_error(std::string("Expected type of token \'") +
                Init::getValue(token) + std::string("\', but there are symbol \'") + nextLexem.value + std::string("\'"));
        }
    }

    void unar(std::queue<Lexem>& stackLexem) {
        if (nextLexem.token == PLUS) {
            match(PLUS);
            power(stackLexem);

        }
        else if (nextLexem.token == MINUS) {
            Lexem curLexem1 = nextLexem;
            match(MINUS);
            pushToken("0");
            Lexem curLexem2;
            curLexem2.token = NUM;
            curLexem2.value = "0";
            pushToken(stackLexem, curLexem2);
            power(stackLexem);
            pushToken(Init::getValue(MINUS));
            pushToken(stackLexem, curLexem1);
        }
        else
        {
            power(stackLexem);
        }
    }

    void factor(std::queue<Lexem>& stackLexem) {
        if (nextLexem.token == NUM) {
            pushToken(nextLexem.value);
            pushToken(stackLexem, nextLexem);
            match(NUM);
        }
        else if (nextLexem.token == ID) {
            pushToken(nextLexem.value);
            
            pushToken(stackLexem, nextLexem);

            match(ID);

            if (nextLexem.token == ID) {
                throw std::runtime_error(std::string("Impicit multiplication isn't supported for X X"));
            }
        }
        else if (nextLexem.token == OPENBACKET) {
            match(OPENBACKET);
            expr(stackLexem);
            match(CLOSEBACKET);
        }
        else {
            throw std::runtime_error(std::string("Expected number, but there are \'") +
                nextLexem.value + std::string("\'"));
        }

    }

    void lines() {
        while (nextLexem.token != END) {
            line();
            pushToken(nextLexem.value);
            match(SEMICOLON);
            std::cout << "\n";
        }
    }

    void line() {
        if (nextLexem.token == ID) {
            std::string var = nextLexem.value;
            pushToken(nextLexem.value);
            match(ID);
            pushToken(nextLexem.value);
            match(ASSIGN);
            std::queue<Lexem> stackLexem;
            expr(stackLexem);
            queueCommand.push({ var, stackLexem });
        }
    }

    void expr(std::queue<Lexem>& stackLexem) {
        term(stackLexem);
        while (nextLexem.token == PLUS || nextLexem.token == MINUS) {
            if (nextLexem.token == PLUS) {
                Lexem curLexem = nextLexem;
                match(PLUS);
                term(stackLexem);
                pushToken(Init::getValue(PLUS));
                pushToken(stackLexem, curLexem);
            }
            else if (nextLexem.token == MINUS) {
                Lexem curLexem = nextLexem;
                match(MINUS);
                term(stackLexem);
                pushToken(Init::getValue(MINUS));
                pushToken(stackLexem, curLexem);
            }
        }
    }

    void term(std::queue<Lexem>& stackLexem) {
        unar(stackLexem);
        while (nextLexem.token == MUL || nextLexem.token == DIV || nextLexem.token == OPENBACKET || nextLexem.token == ID) {
            if (nextLexem.token == MUL) {
                Lexem curLexem = nextLexem;
                match(MUL);
                unar(stackLexem);
                pushToken(Init::getValue(MUL));
                pushToken(stackLexem, curLexem);
            }
            else if (nextLexem.token == DIV) {
                Lexem curLexem = nextLexem;
                match(DIV);
                unar(stackLexem);
                pushToken(Init::getValue(DIV));
                pushToken(stackLexem, curLexem);
            }
            else if (nextLexem.token == OPENBACKET || nextLexem.token == ID) {
                unar(stackLexem);
                pushToken(Init::getValue(MUL));
                Lexem curLexem;
                curLexem.token = MUL;
                curLexem.value = "*";
                pushToken(stackLexem, curLexem);
            }
        }
        
    }

    void power(std::queue<Lexem>& stackLexem) {
        factor(stackLexem);
        if (nextLexem.token == POWER) {
            Lexem curLexem = nextLexem;
            match(POWER);
            power(stackLexem);
            pushToken(Init::getValue(POWER));
            pushToken(stackLexem, curLexem);
        }
    }

    void pushToken(const std::string& token) {
        std::cout << "\'" << token << "\' ";
    }

    void pushToken(std::queue<Lexem>& stackLexem, const Lexem& lexem) {
        stackLexem.push(lexem);
    }

public:
    Parser(const std::string& fileName) : nextLexem{}, lexer(), fileName(fileName) {}

    void translate() {
        try {
			lexer.openFile(fileName);
            nextLexem = lexer.getLexem();
            lines();
            if (nextLexem.token != ERRORTOKEN) {
                if (nextLexem.token == END) {
                    std::cout << "\nSUCCESS\n";
                }
                else {
                    std::cout << "\nFailure";
                }
            }
            else {
                std::cout << "\nInvalid Token: " << nextLexem.value;
            }

        }
        catch (const std::exception& e) {
            throw std::runtime_error(std::string("Parser error: ") + e.what());
        }
    }

    std::queue<std::pair<std::string, std::queue<Lexem>>> getStackLexem() const {
        return queueCommand;
    }
};

