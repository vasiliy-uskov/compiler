#pragma once
#include <list>
#include <optional>
#include <functional>
#include <sstream>
#include "../tokenizer/Token.h"
#include "SyntaxTree.h"

class ParserEngin;
typedef std::list<Token>::iterator TokenIterator;
typedef std::pair<std::optional<SyntaxTree>, TokenIterator> ParsingResult;
typedef std::function<ParsingResult(ParserEngin&, const TokenIterator&)> ParseFn;

class UnexpectedTokenException : public std::exception
{
private:
    const std::string expectedToken;
    const Token realToken;
public:
    UnexpectedTokenException(const std::string & expectedToken, const Token & realToken);
    const char* what() const noexcept override;
};

class ParserEngin
{
private:
    TokenIterator max;
    std::string expectedTokenOnMaxPosition;
    const TokenIterator end;

    bool tokenItLess(const TokenIterator & a, const TokenIterator & b);
    ParsingResult processSyntaxRuleVariant(SyntaxRule rule, const TokenIterator & startProvider, const std::list<ParseFn> & rules);
    ParsingResult parseToken(SyntaxRule rule, const TokenIterator & provider, const std::string&, std::function<bool(const Token&)> matcher);

public:
    ParserEngin(const TokenIterator & start, const TokenIterator & end);

    UnexpectedTokenException getUnexpectedTokenException();
    ParsingResult processSyntaxRule(SyntaxRule rule, const TokenIterator & provider, const std::list<std::list<ParseFn>> & ruleVariants);    
    ParseFn makeTokenParserByValue(SyntaxRule rule, const std::string & tokenValue);
    ParseFn makeTokenParserByValue(SyntaxRule rule, const std::list<std::string> & values);
    ParseFn makeTokenParserByType(SyntaxRule rule, TokenType tokenType);
};