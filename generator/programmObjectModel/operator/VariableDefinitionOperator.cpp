#include "VariableDefinitionOperator.h"

VariableDefinitionOperator::VariableDefinitionOperator(const SyntaxTree & node)
    : node(node),
    type(getTokenValue(node.children[0])),
    identifier(getTokenValue(node.children[1])),
    expression(node.children.size() > 2
        ? ExpressionFactory::create(node.children[3])
        : nullptr) {}

TypeCheckErrors VariableDefinitionOperator::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    TypeCheckErrors errors;
    errors.add(expression->initIdentifiersScope(_scope));
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
            && !(type == "flaot" && realType == "int"))
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