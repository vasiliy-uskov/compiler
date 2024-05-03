#include <gtest/gtest.h>
#include <iostream>
#include <list>
#include "Tokenizer.h"

void checkTokenizerParseCorrect(const std::string & inputString, std::list<Token> acceptableResult)
{
    std::stringstream input = std::stringstream(inputString.c_str());
    std::list<Token> realResult = Tokenizer::parse(input);
    EXPECT_EQ(realResult.size(), acceptableResult.size());
    for (auto realResultCurrent = realResult.begin(), acceptableResultCurrent = acceptableResult.begin();
        realResultCurrent != realResult.end(); 
        realResultCurrent++, acceptableResultCurrent++)
    {
        EXPECT_EQ((*realResultCurrent).type, (*acceptableResultCurrent).type);
        EXPECT_STREQ((*realResultCurrent).value.c_str(), (*acceptableResultCurrent).value.c_str());
    }
}

TEST(TokenizerParseCorrectly, RandomTokens)
{
    checkTokenizerParseCorrect("main equal //asfddf 132. 14\n 13.1 41 4 1. 915{} (f1, b2, ddd3);\n", {
         Token(TokenType::Keyword, "main", {0, 5}),
         Token(TokenType::Identifier, "equal", {0, 11}),
         Token(TokenType::FloatValue, "13.1", {1, 6}),
         Token(TokenType::IntValue, "41", {1, 9}),
         Token(TokenType::IntValue, "4", {1, 11}),
         Token(TokenType::FloatValue, "1.", {1, 14}),
         Token(TokenType::IntValue, "915", {1, 18}),
         Token(TokenType::Breaket, "{", {1, 19}),
         Token(TokenType::Breaket, "}", {1, 20}),
         Token(TokenType::Breaket, "(", {1, 22}),
         Token(TokenType::Identifier, "f1", {1, 24}),
         Token(TokenType::Punctuation, ",", {1, 25}),
         Token(TokenType::Identifier, "b2", {1, 28}),
         Token(TokenType::Punctuation, ",", {1, 29}),
         Token(TokenType::Identifier, "ddd3", {1, 34}),
         Token(TokenType::Breaket, ")", {1, 35}),
         Token(TokenType::Punctuation, ";", {1, 36})
    });
}

TEST(TokenizerParseCorrectly, ProgramTokens)
{
    checkTokenizerParseCorrect(
    std::string("main {\n")
    + "  int a = 4;\n"
    + "  int b = 3;\n"
    + "  b = (b + a) * 4;\n"
    + "}\n", {
         Token(TokenType::Keyword, "main", {0, 5}),
         Token(TokenType::Breaket, "{", {0, 7}),
         Token(TokenType::Keyword, "int", {1, 6}),
         Token(TokenType::Identifier, "a", {1, 8}),
         Token(TokenType::Operator, "=", {1, 10}),
         Token(TokenType::IntValue, "4", {1, 12}),
         Token(TokenType::Punctuation, ";", {1, 13}),
         Token(TokenType::Keyword, "int", {2, 6}),
         Token(TokenType::Identifier, "b", {2, 8}),
         Token(TokenType::Operator, "=", {2, 10}),
         Token(TokenType::IntValue, "3", {2, 12}),
         Token(TokenType::Punctuation, ";", {2, 13}),
         Token(TokenType::Identifier, "b", {3, 4}),
         Token(TokenType::Operator, "=", {3, 6}),
         Token(TokenType::Breaket, "(", {3, 8}),
         Token(TokenType::Identifier, "b", {3, 9}),
         Token(TokenType::Operator, "+", {3, 11}),
         Token(TokenType::Identifier, "a", {3, 13}),
         Token(TokenType::Breaket, ")", {3, 14}),
         Token(TokenType::Operator, "*", {3, 16}),
         Token(TokenType::IntValue, "4", {3, 18}),
         Token(TokenType::Punctuation, ";", {3, 19}),
         Token(TokenType::Breaket, "}", {4, 2})
    });
}

TEST(TokenizerParseExeption, UnknownOperator)
{
    auto input = std::stringstream("g&a ");
    EXPECT_THROW(Tokenizer::parse(input), UnknownOperatorExeption);
    input = std::stringstream("a|a ");
    EXPECT_THROW(Tokenizer::parse(input), UnknownOperatorExeption);
    input = std::stringstream("b|||b ");
    EXPECT_THROW(Tokenizer::parse(input), UnknownOperatorExeption);
    input = std::stringstream("** ");
    EXPECT_THROW(Tokenizer::parse(input), UnknownOperatorExeption);
    input = std::stringstream("++ ");
    EXPECT_THROW(Tokenizer::parse(input), UnknownOperatorExeption);
    input = std::stringstream("--a ");
    EXPECT_THROW(Tokenizer::parse(input), UnknownOperatorExeption);
}

TEST(TokenizerParseExeption, UnexpectedSymbol)
{
    auto input = std::stringstream("1ab");
    EXPECT_THROW(Tokenizer::parse(input), UnexpectedSymbolExeption);
    input = std::stringstream("1.4.");
    EXPECT_THROW(Tokenizer::parse(input), UnexpectedSymbolExeption);
    input = std::stringstream(" . ");
    EXPECT_THROW(Tokenizer::parse(input), UnexpectedSymbolExeption);
    input = std::stringstream(". ");
    EXPECT_THROW(Tokenizer::parse(input), UnexpectedSymbolExeption);
    input = std::stringstream("? ");
    EXPECT_THROW(Tokenizer::parse(input), UnexpectedSymbolExeption);
    input = std::stringstream("Что");
    EXPECT_THROW(Tokenizer::parse(input), UnexpectedSymbolExeption);
    input = std::stringstream("//adsfa ? . q");
    EXPECT_NO_THROW(Tokenizer::parse(input));
}