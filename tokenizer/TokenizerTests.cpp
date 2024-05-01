#include <gtest/gtest.h>
#include <iostream>
#include "Tokenizer.cpp"

void checkTokenizerParseCorrect(const std::string & inputString, std::list<Token> acceptableResult)
{
    acceptableResult.reverse();
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

TEST(TokenizerParseCorrectly, Tokens)
{
    checkTokenizerParseCorrect("main equal //asfddf 132. 14\n 13.1 41 4 1. 915{} (f1, b2, ddd3);\n", {
         Token(Token::TokenType::Keyword, "main"),
         Token(Token::TokenType::Identifier, "equal"),
         Token(Token::TokenType::FloatValue, "13.1"),
         Token(Token::TokenType::IntValue, "41"),
         Token(Token::TokenType::IntValue, "4"),
         Token(Token::TokenType::FloatValue, "1."),
         Token(Token::TokenType::IntValue, "915"),
         Token(Token::TokenType::Breaket, "{"),
         Token(Token::TokenType::Breaket, "}"),
         Token(Token::TokenType::Breaket, "("),
         Token(Token::TokenType::Identifier, "f1"),
         Token(Token::TokenType::Punctuation, ","),
         Token(Token::TokenType::Identifier, "b2"),
         Token(Token::TokenType::Punctuation, ","),
         Token(Token::TokenType::Identifier, "ddd3"),
         Token(Token::TokenType::Breaket, ")"),
         Token(Token::TokenType::Punctuation, ";"),
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
    input = std::stringstream("? ");
    EXPECT_THROW(Tokenizer::parse(input), UnexpectedSymbolExeption);
    input = std::stringstream("//adsfa ? . q");
    EXPECT_NO_THROW(Tokenizer::parse(input));
}