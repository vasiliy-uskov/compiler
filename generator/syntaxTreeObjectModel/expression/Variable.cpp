#include "Variable.h"

Variable::Variable(const SyntaxTree & node)
    : BaseExpression(node),
    identifier(getTokenValue(node)) {}

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