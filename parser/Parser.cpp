#pragma once
#include "Parser.h"

SyntaxTree<SyntaxRule> Parser::parse(std::list<Token> tokens)
{
    ParserEngin<SyntaxRule> engin = {tokens.begin(), tokens.end()};
    auto [result, endTokent] = parseProgramm(engin, tokens.begin());
    if (result.has_value())
    {
        return result.value();
    }
    throw engin.getUnexpectedTokenException();
}