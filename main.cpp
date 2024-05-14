#include <iostream>
#include <fstream>
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"
#include "generator/Generator.h"

int main(int argc, char * argv[])
{
    if (argc != 1)
    {
        std::cout << "Expected program name";
        return 1;
    }
    auto file = std::fstream(argv[0], std::ios::in);
    auto tokens = Tokenizer::parse(file);
    auto syntaxTree = Parser::parse(tokens);
    Generator::generate(syntaxTree);
    return 0;
}