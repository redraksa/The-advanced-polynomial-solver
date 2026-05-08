#include <memory>
#include <iostream>
#include <stdexcept>
#include "Emitter.h"
#include "ResultPrinter.h"
#include "parser.tab.hpp"

Emitter* globalEmitter = nullptr;

extern int yyparse();
extern FILE* yyin;

int main(int argc, char* argv[])
{
    Emitter emitter;
    globalEmitter = &emitter;
    
    //std::cout << "Input expression: \n";
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    //FILE* inputFile = fopen(argv[1], "r");
    std::unique_ptr<FILE, int(*)(FILE*)> filePtr(fopen(argv[1], "r"), fclose);

    if (!filePtr) {
        std::cerr << "Error: Could not open file: " << argv[1];
    }

    yyin = filePtr.get();

    try
    {
        if (yyparse() == 0) {
            //emitter.normilizeAnswer();
            //std::cout << "SUCCESS\n";
            //std::cout << "Result: ";

            ResultPrinter resultPrinter(emitter.getResult());
	        resultPrinter.printResult();
	
        }
        else {
            std::cout << "Failure. Syntax error";
        }

    }
    catch (const std::out_of_range& e) {
	    std::cerr << "Value error: " << e.what() << "\n";
	    return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Runtime error: " << e.what() << "\n";
	    return 1;
    }
    catch (...) {
	    std::cerr << "Uknown error!\n";
	    return 1;
    }
    return 0;
}

