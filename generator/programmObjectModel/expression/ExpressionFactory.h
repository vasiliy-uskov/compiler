#pragma once
#include <vector>
#include "BaseExpression.h"

struct ExpressionFactory
{
    static IExpression::ExpressionPtr create(const SyntaxTree & node);
    static std::vector<IExpression::ExpressionPtr> createList(const SyntaxTree & list);
};