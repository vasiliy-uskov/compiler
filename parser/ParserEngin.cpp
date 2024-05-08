#include "ParserEngin.h"

ParserEngin::ParserEngin(const TokenIterator & start, const TokenIterator & end)
    : max(start), end(end) {}

ParsingResult ParserEngin::processSyntaxRuleVariant(SyntaxRule rule, const TokenIterator & startIt, const std::list<ParseFn> & rules)
{
    std::vector<SyntaxTree> children = {};
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

ParsingResult ParserEngin::processSyntaxRule(SyntaxRule rule, const TokenIterator & startIt, const std::list<std::list<ParseFn>> & ruleVariants)
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

ParsingResult ParserEngin::parseToken(SyntaxRule rule, const TokenIterator & startIt, const std::string & expectedToken, std::function<bool(const Token&)> matcher)
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

bool ParserEngin::tokenItLess(const TokenIterator & a, const TokenIterator & b)
{
    int aDistance = std::distance(a, end);
    int bDistance = std::distance(b, end);
    return aDistance >= bDistance;

}

ParseFn ParserEngin::makeTokenParserByValue(SyntaxRule rule, const std::string & tokenValue)
{
    return [rule, tokenValue](ParserEngin & engin, const TokenIterator & tokenIt) {
        return engin.parseToken(rule, tokenIt, tokenValue, [tokenValue](const Token & token) {
            return token.value == tokenValue;
        });
    };
}

ParseFn ParserEngin::makeTokenParserByType(SyntaxRule rule, TokenType tokenType)
{
    return [rule, tokenType](ParserEngin & engin, const TokenIterator & tokenIt) {
        return engin.parseToken(rule, tokenIt, fromTokenTypeToString(tokenType), [tokenType](const Token & token) {
            return token.type == tokenType;
        });
    };
}

UnexpectedTokenException ParserEngin::getUnexpectedTokenException()
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