#pragma once
#include <list>
#include <optional>
#include <functional>
#include <vector>
#include "../tokenizer/Token.h"

template <class T>
class SyntaxTree
{
public:
    const std::vector<SyntaxTree> children;
    const std::list<Token> tokens;
    const T rule;

    SyntaxTree(T, const std::list<Token> &);
    SyntaxTree(T, const std::list<Token> &, const std::vector<SyntaxTree<T>> &);
};

typedef std::list<Token>::iterator TokenIterator;

template <class T>
class ParsingResult : public std::pair<std::optional<SyntaxTree<T>>, TokenIterator> {};

template <class T> class ParserEngin;

template <class T>
class ParseFn : public std::function<ParsingResult<T>(ParserEngin<T>, const TokenIterator)> {};

class UnexpectedTokenException : public std::exception
{
private:
    const std::string expectedToken;
    const Token realToken;
public:
    UnexpectedTokenException(const std::string & expectedToken, const Token & realToken);
    const char* what() const noexcept override;
};

template <class T>
class ParserEngin
{
private:
    TokenIterator max;
    std::string expectedTokenOnMaxPosition;
    const TokenIterator end;

    bool tokenItLess(const TokenIterator & a, const TokenIterator & b);
    ParsingResult<T> processSyntaxRuleVariant(T rule, const TokenIterator & startProvider, const std::list<ParseFn<T>> & rules);
    ParsingResult<T> parseToken(T rule, const TokenIterator & provider, const std::string&, std::function<bool(const Token&)> matcher);

public:
    ParserEngin(const TokenIterator & start, const TokenIterator & end);

    UnexpectedTokenException getUnexpectedTokenException();
    ParsingResult<T> processSyntaxRule(T rule, const TokenIterator & provider, const std::list<std::list<ParseFn<T>>> & ruleVariants);    
    ParseFn<T> makeTokenParserByValue(T rule, const std::string & tokenValue);
    ParseFn<T> makeTokenParserByType(T rule, TokenType tokenType);
};