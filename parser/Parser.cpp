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

std::vector<SyntaxTree> collapseList(const std::vector<SyntaxTree> & children)
{  
    std::vector<SyntaxTree> newChildren;
    std::vector<SyntaxTree> currentNodeChildren = children;

    newChildren.push_back(currentNodeChildren[0]);
    while (currentNodeChildren.size() == 2)
    {
        newChildren.push_back(currentNodeChildren[0]);
        currentNodeChildren = currentNodeChildren[1].children;
    }
    if (&currentNodeChildren != &children)
    {
        newChildren.push_back(currentNodeChildren[0]);
    }

    return newChildren;
}

bool isRuleAnyOf(SyntaxRule rule, const std::list<SyntaxRule> & rulesToFilter)
{
    return std::any_of(rulesToFilter.begin(), rulesToFilter.end(), [rule](const SyntaxRule & item) {
        return item == rule;
    });
}

std::vector<SyntaxTree> filterSyntaxRules(const std::vector<SyntaxTree> & children, const std::list<SyntaxRule> & rulesToFilter)
{
    std::vector<SyntaxTree> newChildren;
    for (auto child : children)
    {
        if (!isRuleAnyOf(child.rule, rulesToFilter))
        {
            newChildren.push_back(child);
        }
    };
    return newChildren;
}

std::vector<SyntaxTree> filterEmptyOperators(const std::vector<SyntaxTree> & children)
{
    std::vector<SyntaxTree> newChildren;
    for (auto child : children)
    {
        if (child.children.size())
        {
            newChildren.push_back(child);
        }
    };
    return newChildren;
}

SyntaxTree Parser::deleteExtraNodes(SyntaxTree & node)
{
    std::vector<SyntaxTree> newChildren = filterSyntaxRules(node.children, {SyntaxRule::Breaket, SyntaxRule::Punctuation});
    if (isRuleAnyOf(node.rule, {
        SyntaxRule::FunctionsList,
        SyntaxRule::DefinitionArgumentsList,
        SyntaxRule::CallArgumentsList,
        SyntaxRule::FunctionOperatorsList,
        SyntaxRule::OperatorsList
    }))
    {
        newChildren = collapseList(newChildren);
    }
    if (node.rule == SyntaxRule::OperatorsList)
    {
        newChildren = filterEmptyOperators(newChildren);
    }
    std::transform(
        newChildren.begin(),
        newChildren.end(),
        newChildren.begin(),
        Parser::deleteExtraNodes
    );
    return SyntaxTree(node.rule, node.tokens, newChildren);
}