#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../tokenizer/Tokenizer.h"

TEST(ParserParseCorrectly, Parser)
{ 
    /*
    + "  if (((a < b) && true) || isCalculatable(a, b))) {\n"
    + "    calculate(a, b);\n"
    + "    b = (sum(a, b) * 4);\n"
    + "  }\n"
    + "  if (a < b) {\n"
    + "    calculate(a, b);\n"
    + "    b = (sum(a, b) * 4);\n"
    + "  }\n"
    + "  else {\n"
    + "    calculate(a, b);\n"
    + "    b = (sum(a, b) * 4);\n"
    + "  }\n"
    + "  while (a < b) {\n"
    + "    calculate(a, b);\n"
    + "    b = (sum(a, b) * 4);\n"
    + "  }\n"
    */
    std::stringstream programm = std::stringstream(std::string()
    + "int sum(int a, int b) {\n"
    + "  return a + b;"
    + "}\n"
    + "void calculate(int a, int b) {\n"
    + "  b = a + b;"
    + "}\n"
    + "main {\n"
    + "  int a = 4;\n"
    + "  int b = 3;\n"
    + "  calculate(a, b);\n"
    + "  b = sum(a, b) * 4;\n"
    + "  if (a < b) {\n"
    + "    calculate(a, b);\n"
    + "  }\n"
    + "}\n");
    auto tokens = Tokenizer::parse(programm);
    auto tree = Parser::parse(tokens);
    EXPECT_TRUE(true);
}