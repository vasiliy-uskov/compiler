#include "Parser.h"

SyntaxTree deleteExtraNodes(const SyntaxTree & node)
{
    return SyntaxTree(node.rule, node.tokens, JSLikeVectorAdapter(node.children)
        .reduce([](const SyntaxTree & child) {
            return child.rule != SyntaxRule::Breaket
                && child.rule != SyntaxRule::Punctuation;
        })
        .map(deleteExtraNodes)
        .flatIf(isListRule(node.rule), [](const SyntaxTree & child) {
            return isListRule(child.rule) || isOperatorRule(child.rule)
                ? std::optional<std::vector<SyntaxTree>>(child.children)
                : std::nullopt;
        })
        .flatIf(node.rule == SyntaxRule::Expression, [](const SyntaxTree & child) {
            return child.children.size() == 1 && child.rule == SyntaxRule::Expression
                ? std::optional<std::vector<SyntaxTree>>(child.children)
                : std::nullopt;
        })
        .value());
}


SyntaxTree Parser::parse(std::list<Token> tokens)
{
    ParserEngin engin = {tokens.begin(), tokens.end()};
    auto [result, endTokent] = parseProgramm(engin, tokens.begin());
    if (result.has_value())
    {
        return deleteExtraNodes(result.value());
    }
    throw engin.getUnexpectedTokenException();
}