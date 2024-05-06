#include <gtest/gtest.h>
#include <string>
#include "Parser.h"
#include <fstream>
#include "../tokenizer/Tokenizer.h"

void printLevelSpacies(std::ostream & os, size_t level)
{
    os << "|";
    for (size_t i = 0; i < level; ++i)
    {
        os << " ";
    }
}

void printTree(std::ostream & os, const SyntaxTree & tree, size_t level = 0)
{
    printLevelSpacies(os, level);
    os << tree.rule << ": " << "\"";
    for (auto token : tree.tokens)
    {
        os << token.value << " ";
    }
    os << "\"" << std::endl;
    for (auto child : tree.children)
    {
        printTree(os, child, level + 1);
    }
}

TEST(ParserParseCorrectly, Parser)
{ 
    std::ifstream input;
    std::ofstream output;
    input.open("../tests/input1.txt", std::ios::in);
    output.open("../tests/result1.txt", std::ios::out);
    auto tokens = Tokenizer::parse(input);
    auto result = Parser::parse(tokens);
    
    printTree(output, result);
    input.close();
    output.close();
}