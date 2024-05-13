#include "VariableDefinitionOperator.h"

VariableDefinitionOperator::VariableDefinitionOperator(const SyntaxTree & node)
    : node(node),
    type(getTokenValue(node.children[0])),
    identifier(getTokenValue(node.children[1])),
    expression(node.children.size() > 2
        ? std::optional(ExpressionFactory::create(node.children[3]))
        : std::nullopt) {}

TypeCheckErrors VariableDefinitionOperator::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    TypeCheckErrors errors;
    try
    {
        scope->addVariable(identifier, type);
    }
    catch (IdentifiersScope::DublicateException e)
    {
        errors.add(DublicateDeclorationException(node));
    }
    return errors;
}

TypeCheckErrors VariableDefinitionOperator::checkTypes() const
{
    TypeCheckErrors errors;
    if (expression.has_value())
    {
        try
        {
            auto realType = expression.value().getType();
            if (realType != type)
            {   
                errors.add(InvalidTypeException(expression.value().getNode(), realType, type));
            }
        }
        catch (TypeCheckErrors e)
        {
            errors.add(e);
        }
    }
    return errors;
}