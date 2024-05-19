#include "OperatorIf.h"

OperatorIf::OperatorIf(const SyntaxTree & operatorNode)
    : node(operatorNode),
    expression(ExpressionFactory::create(operatorNode.children[1])),
    operatorsList(OperatorFactory::createList(operatorNode.children[2])),
    elseOperatorsList(operatorNode.children.size() > 3
        ? OperatorFactory::createList(operatorNode.children[4])
        : std::vector<IOperator::OperatorPtr>({})) {}

OperatorIf::OperatorIf(const std::string & functionReturnType, const SyntaxTree & operatorNode)
    : node(operatorNode),
    expression(ExpressionFactory::create(operatorNode.children[1])),
    operatorsList(OperatorFactory::createFunctionOperatorsList(functionReturnType, operatorNode.children[2])),
    elseOperatorsList(operatorNode.children.size() > 3
        ? OperatorFactory::createFunctionOperatorsList(functionReturnType, operatorNode.children[4])
        : std::vector<IOperator::OperatorPtr>({})) {}

        
TypeCheckErrors OperatorIf::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & parentScope)
{
    scope = std::make_shared<IdentifiersScope>(parentScope);
    TypeCheckErrors errors;

    errors.add(expression->initIdentifiersScope(scope));
    for (IOperator::OperatorPtr & oper : operatorsList)
    {
        errors.add(oper->initIdentifiersScope(scope));
    } 
    for (IOperator::OperatorPtr & oper : elseOperatorsList)
    {
        errors.add(oper->initIdentifiersScope(scope));
    }
    return errors;
}

TypeCheckErrors OperatorIf::checkTypes() const
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
    for (const IOperator::OperatorPtr & oper : elseOperatorsList)
    {
        errors.add(oper->checkTypes());
    }
    return errors;
}