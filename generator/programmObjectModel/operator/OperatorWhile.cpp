#include "OperatorWhile.h"

OperatorWhile::OperatorWhile(const SyntaxTree & _node)
    : node(_node),
    expression(ExpressionFactory::create(_node.children[1])),
    operatorsList(OperatorFactory::createList(_node.children[2])) {}

OperatorWhile::OperatorWhile(const std::string & functionReturnType, const SyntaxTree & _node)
    : node(_node),
    expression(ExpressionFactory::create(_node.children[1])),
    operatorsList(OperatorFactory::createFunctionOperatorsList(functionReturnType, _node.children[2])) {}
        
TypeCheckErrors OperatorWhile::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & parentScope)
{
    scope = std::make_shared<IdentifiersScope>(parentScope);
    TypeCheckErrors errors;

    errors.add(expression->initIdentifiersScope(scope));
    for (IOperator::OperatorPtr & oper : operatorsList)
    {
        errors.add(oper->initIdentifiersScope(scope));
    }
    return errors;
}

TypeCheckErrors OperatorWhile::checkTypes() const
{
    TypeCheckErrors errors;
    try
    {
        auto realType = expression->getType();
        if (realType != "bool")
        {   
            errors.add(InvalidTypeException(expression->getNode(), realType, "bool"));
        }
    }
    catch (TypeCheckErrors e)
    {
        errors.add(e);
    }
    for (const IOperator::OperatorPtr & oper : operatorsList)
    {
        errors.add(oper->checkTypes());
    }
    return errors;
}