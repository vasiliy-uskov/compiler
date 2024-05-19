#include "FunctionCallOperator.h"

FunctionCallOperator::FunctionCallOperator(const SyntaxTree & node)
    : expression(FunctionCall(node)) {}

TypeCheckErrors FunctionCallOperator::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope)
{
    return expression.initIdentifiersScope(scope);
}

TypeCheckErrors FunctionCallOperator::checkTypes() const
{
    return expression.checkTypes();
}