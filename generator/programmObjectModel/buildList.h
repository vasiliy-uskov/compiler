#pragma once
#include <vector>
#include <functional>
#include "../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.h"

template <typename T>
std::vector<T> buildList(const SyntaxTree & listNode, const std::function<T(const SyntaxTree&)> & creator)
{
    std::vector<T> list;
    for (size_t i = 0; i < listNode.children.size(); ++i)
    {
        list.push_back(creator(listNode.children[i]));
    }
    return list;
}