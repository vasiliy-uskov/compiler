#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.h"

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