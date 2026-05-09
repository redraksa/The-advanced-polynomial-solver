#include "Lexer.h"

void Lexer::nextLexem() {
    file.get(lookahead);

    while (!file.eof() && CHAR_DIVIDERS.find(lookahead) != std::string::npos) {
        file.get(lookahead);
    }

    if (file.eof()) {
        lexem.value = "\0";
        lexem.token = END;
        return;
    }

    if (std::isdigit(lookahead)) {
        lexem.token = NUM;
        long long num = lookahead - '0';
        while (std::isdigit(file.peek())) {
            file.get(lookahead);
            num = num * 10ll + (lookahead - '0');
        }
        lexem.value = std::to_string(num);
        return;
    }

    if (std::isalpha(lookahead)) {
        lexem.token = ID;
        std::string var(1, lookahead);
        while (std::isalpha(file.peek()) || std::isdigit(file.peek()) || file.peek() == '_') {
            file.get(lookahead);
            var += lookahead;
        }
        lexem.value = var;
        return;
    }

    std::string curLexem = std::string(1, lookahead);

    for (int i = 0; i < TokenType::ENDENUM; ++i) {
        std::string expectedString = Init::getValue(static_cast<TokenType>(i));
        if (expectedString == curLexem) {
            lexem.value = curLexem;
            lexem.token = static_cast<TokenType>(i);
            return;
        }
    }

    lexem.token = ERRORTOKEN;
}

const Lexem& Lexer::getLexem() {
    nextLexem();
    return lexem;
}

void Lexer::openFile(const std::string& fileName) {
    file.open(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + fileName);
    }
}