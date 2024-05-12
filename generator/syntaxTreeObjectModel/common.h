#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <list>
#include "identifiersScope/IdentifiersScope.h"
#include "TypeCheckErrors.cpp"
#include "../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.h"

struct INode
{
    virtual TypeCheckErrors checkTypes() const;
    virtual TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope);
};

std::string getTokenValue(const SyntaxTree & node)
{
    return node.tokens[0].value;
}

template <typename T>
std::vector<T> buildList(const SyntaxTree & listNode, const std::function<T(const SyntaxTree&)> & creator)
{
    std::vector<T> argumentsList;
    for (const auto & argument : listNode.children)
    {
        argumentsList.push_back(creator(argument));
    }
    return argumentsList;
}