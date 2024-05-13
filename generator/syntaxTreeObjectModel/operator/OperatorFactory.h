#pragma once
#include <vector>
#include "IOperator.h"

struct OperatorFactory
{
    static IOperator create(const SyntaxTree & node);
    static std::vector<IOperator> createList(const SyntaxTree & node);
    static IOperator createFunctionOperator(const std::string & functionReturnType, const SyntaxTree & node);
    static std::vector<IOperator> createFunctionOperatorsList(const std::string & functionReturnType, const SyntaxTree & node);
};
