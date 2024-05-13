#include "BaseExpression.h"

bool BaseExpression::isOperandArithmetic(const std::string & operandType)
{
    return operandType == "int" || operandType == "float";
}

BaseExpression::BaseExpression(const SyntaxTree & node) : node(node) {}

TypeCheckErrors BaseExpression::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    return {};
}
SyntaxTree BaseExpression::getNode() const
{
    return node;
}