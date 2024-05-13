#pragma once
#include <vector>
#include "BaseExpression.h"

struct ExpressionFactory
{
    static IExpression create(const SyntaxTree & node);
    static std::vector<IExpression> createList(const SyntaxTree & list);
};