#include <string>
#include <iostream>
#include "Parser.h"
#include "Emitter.h"
#include "ResultPrinter.h"
#include "Polynom.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    else {
        try {
            Parser parser{ std::string(argv[1]) };
            parser.translate();
            Emitter emitter(parser.getStackLexem());
            emitter.launch();
            ResultPrinter resultPrinter(emitter.getResult());
            resultPrinter.printResult();
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what();
        }
    } 

    return 0;
}

