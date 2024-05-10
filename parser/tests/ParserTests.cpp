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

bool compareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

  if (f1.fail() || f2.fail()) {
    return false;
  }

  if (f1.tellg() != f2.tellg()) {
    return false;
  }

  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(
    std::istreambuf_iterator<char>(f1.rdbuf()),
    std::istreambuf_iterator<char>(),
    std::istreambuf_iterator<char>(f2.rdbuf())
    );
}

TEST(ParserParseCorrectly, Parser)
{ 
    std::fstream input;
    std::fstream realOutput;

    input.open("../tests/input.txt", std::ios::in);
    realOutput.open("../tests/result.txt", std::ios::out);

    auto tokens = Tokenizer::parse(input);
    auto result = Parser::parse(tokens);

    printTree(realOutput, result);
    input.close();
    realOutput.close();

    EXPECT_TRUE(compareFiles("../tests/result.txt", "../tests/output.txt"));
}

TEST(ParserParseInvalidInput, Parser)
{ 
    std::ifstream input;
    std::list<std::string> inputFiles = {
        "../tests/InvalidInput1.txt",
        "../tests/InvalidInput2.txt",
        "../tests/InvalidInput3.txt"
    };
    for (auto inputFile : inputFiles)
    {
        input.open(inputFile, std::ios::in);
        auto tokens = Tokenizer::parse(input);
        EXPECT_THROW(Parser::parse(tokens), UnexpectedTokenException);    
        input.close();
    }
}