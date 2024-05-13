#include "OperatorWhile.h"

OperatorWhile::OperatorWhile(const SyntaxTree & node)
    : node(node),
    expression(ExpressionFactory::create(node.children[1])),
    operatorsList(OperatorFactory::createList(node.children[2])) {}

OperatorWhile::OperatorWhile(const std::string & functionReturnType, const SyntaxTree & node)
    : node(node),
    expression(ExpressionFactory::create(node.children[1])),
    operatorsList(OperatorFactory::createFunctionOperatorsList(functionReturnType, node.children[2])) {}
        
TypeCheckErrors OperatorWhile::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & parentScope)
{
    scope = std::make_shared<IdentifiersScope>(parentScope);
    TypeCheckErrors errors;

    errors.add(expression.initIdentifiersScope(scope));
    for (IOperator & oper : operatorsList)
    {
        errors.add(oper.initIdentifiersScope(scope));
    }
    return errors;
}

TypeCheckErrors OperatorWhile::checkTypes() const
{
    TypeCheckErrors errors;
    try
    {
        auto realType = expression.getType();
        if (realType != "bool")
        {   
            errors.add(InvalidTypeException(expression.getNode(), realType, "bool"));
        }
    }
    catch (TypeCheckErrors e)
    {
        errors.add(e);
    }
    for (const IOperator & oper : operatorsList)
    {
        errors.add(oper.checkTypes());
    }
    return errors;
}