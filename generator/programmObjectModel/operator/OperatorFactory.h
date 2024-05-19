#pragma once
#include <vector>
#include "IOperator.h"

struct OperatorFactory
{
    static IOperator::OperatorPtr create(const SyntaxTree & node);
    static std::vector<IOperator::OperatorPtr> createList(const SyntaxTree & node);
    static IOperator::OperatorPtr createFunctionOperator(const std::string & functionReturnType, const SyntaxTree & node);
    static std::vector<IOperator::OperatorPtr> createFunctionOperatorsList(const std::string & functionReturnType, const SyntaxTree & node);
};
