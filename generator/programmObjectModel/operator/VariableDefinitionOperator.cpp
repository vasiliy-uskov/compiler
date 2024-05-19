#include "VariableDefinitionOperator.h"

VariableDefinitionOperator::VariableDefinitionOperator(const SyntaxTree & _node)
    : node(_node),
    type(getTokenValue(_node.children[0])),
    identifier(getTokenValue(_node.children[1])),
    expression(_node.children.size() > 2
        ? ExpressionFactory::create(_node.children[3])
        : nullptr) {}

TypeCheckErrors VariableDefinitionOperator::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    TypeCheckErrors errors;
    if (expression)
    {
        errors.add(expression->initIdentifiersScope(_scope));
    }
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
    if (expression != nullptr)
    {
        try
        {
            auto realType = expression->getType();
            if (realType != type
            && !(type == "float" && realType == "int"))
            {   
                errors.add(InvalidTypeException(expression->getNode(), realType, type));
            }
        }
        catch (TypeCheckErrors e)
        {
            errors.add(e);
        }
    }
    return errors;
}