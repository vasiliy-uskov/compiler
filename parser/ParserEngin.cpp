#include <sstream>
#include "ParserEngin.h"

template <class T>
SyntaxTree<T>::SyntaxTree(T rule, const std::list<Token> & tokens)
        : rule(rule), tokens(tokens), children({}) {}

template <class T>
SyntaxTree<T>::SyntaxTree(T rule, const std::list<Token> & tokens, const std::vector<SyntaxTree<T>> & _children)
        : rule(rule), tokens(tokens), children(_children) {}

template <class T>
ParserEngin<T>::ParserEngin(const TokenIterator & start, const TokenIterator & end)
    : max(start), end(end) {}

template <class T>
ParsingResult<T> ParserEngin<T>::processSyntaxRuleVariant(T rule, const TokenIterator & startIt, const std::list<ParseFn<T>> & rules)
{
    std::vector<SyntaxTree<T>> children = {};
    TokenIterator currentIt = startIt;
    for (auto rule : rules)
    {
        auto [ruleResult, ruleEndProvider] = rule(*this, currentIt);
        if (!ruleResult.has_value())
        {
            return {std::nullopt, currentIt};
        }
        children.push_back(ruleResult.value());
        currentIt = ruleEndProvider;
    }
    return {
        SyntaxTree(rule, {startIt, currentIt}, children),
        currentIt
    };
}

template <class T>
ParsingResult<T> ParserEngin<T>::processSyntaxRule(T rule, const TokenIterator & startIt, const std::list<std::list<ParseFn<T>>> & ruleVariants)
{
    auto errorTokenIt = startIt; 
    for (auto ruleVariant : ruleVariants)
    {
        auto result = processSyntaxRuleVariant(rule, startIt, ruleVariant);
        if (result.first.has_value())
        {
            return result;
        }
        if (tokenItLess(errorTokenIt, result.second))
        {
            errorTokenIt = result.second;
        }
    }
    return {std::nullopt, errorTokenIt};
}

template <class T>
ParsingResult<T> ParserEngin<T>::parseToken(T rule, const TokenIterator & startIt, const std::string & expectedToken, std::function<bool(const Token&)> matcher)
{
    if (tokenItLess(max, startIt))
    {
        max = startIt;
        expectedTokenOnMaxPosition = expectedToken;
    }
    if (startIt != end && matcher(*startIt))
    {
        const auto nextIt = std::next(startIt, 1);
        if (tokenItLess(max, nextIt))
        {

        }
        return ParsingResult(
            SyntaxTree(rule, {startIt, nextIt}),
            nextIt
        );
    }
    return ParsingResult(std::nullopt, startIt);
}

template <class T>
bool ParserEngin<T>::tokenItLess(const TokenIterator & a, const TokenIterator & b)
{
    int aDistance = std::distance(a, end);
    int bDistance = std::distance(b, end);
    return aDistance >= bDistance;

}

template <class T>
ParseFn<T> ParserEngin<T>::makeTokenParserByValue(T rule, const std::string & tokenValue)
{
    return [rule, tokenValue](ParserEngin & engin, const TokenIterator & tokenIt) {
        return engin.parseToken(rule, tokenIt, tokenValue, [tokenValue](const Token & token) {
            return token.value == tokenValue;
        });
    };
}

template <class T>
ParseFn<T> ParserEngin<T>::makeTokenParserByType(T rule, TokenType tokenType)
{
    return [rule, tokenType](ParserEngin & engin, const TokenIterator & tokenIt) {
        return engin.parseToken(rule, tokenIt, fromTokenTypeToString(tokenType), [tokenType](const Token & token) {
            return token.type == tokenType;
        });
    };
}

template <class T>
UnexpectedTokenException ParserEngin<T>::getUnexpectedTokenException()
{
    return UnexpectedTokenException(expectedTokenOnMaxPosition, *max);
}

UnexpectedTokenException::UnexpectedTokenException(const std::string & expectedToken, const Token & realToken)
    : expectedToken(expectedToken), realToken(realToken) {}
    
const char* UnexpectedTokenException::what() const noexcept
{
    std::ostringstream message;
    message
        << "Unexpected token"
        << "(" << realToken.position.line << "," <<  realToken.position.end - 1 << "): "
        << realToken.value
        << " expect "
        << expectedToken;
    return std::string(message.str()).c_str();
}