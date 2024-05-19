#include "Variable.h"

Variable::Variable(const SyntaxTree & _node)
    : BaseExpression(_node),
    identifier(getTokenValue(_node)) {}

std::string Variable::getType() const
{
    try
    {
        return scope->getVariableType(identifier);
    }
    catch(IdentifiersScope::UnknownIdentifierException e)
    {
        throw TypeCheckErrors({UnknownVariableException(node)});
    }
}

TypeCheckErrors Variable::checkTypes() const
{
    try
    {
        getType();
    }
    catch(TypeCheckErrors e)
    {
        return e;
    }
}