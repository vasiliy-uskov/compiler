#include "Parser.h"

SyntaxTree Parser::parse(std::list<Token> tokens)
{
    ParserEngin engin = {tokens.begin(), tokens.end()};
    auto [result, endTokent] = parseProgramm(engin, tokens.begin());
    if (result.has_value())
    {
        return result.value();
    }
    throw engin.getUnexpectedTokenException();
}