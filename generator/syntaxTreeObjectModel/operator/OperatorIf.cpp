#include "OperatorIf.h"

OperatorIf::OperatorIf(const SyntaxTree & node)
    : node(node),
    expression(ExpressionFactory::create(node.children[1])),
    operatorsList(OperatorFactory::createList(node.children[2])),
    elseOperatorsList(node.children.size() > 3
        ? OperatorFactory::createList(node.children[4])
        : std::vector<IOperator>({})) {}

OperatorIf::OperatorIf(const std::string & functionReturnType, const SyntaxTree & operatorNode)
    : node(node),
    expression(ExpressionFactory::create(node.children[1])),
    operatorsList(OperatorFactory::createFunctionOperatorsList(functionReturnType, node.children[2])),
    elseOperatorsList(node.children.size() > 3
        ? OperatorFactory::createFunctionOperatorsList(functionReturnType, node.children[4])
        : std::vector<IOperator>({})) {}

        
TypeCheckErrors OperatorIf::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & parentScope)
{
    scope = std::make_shared<IdentifiersScope>(parentScope);
    TypeCheckErrors errors;

    errors.add(expression.initIdentifiersScope(scope));
    for (IOperator & oper : operatorsList)
    {
        errors.add(oper.initIdentifiersScope(scope));
    } 
    for (IOperator & oper : elseOperatorsList)
    {
        errors.add(oper.initIdentifiersScope(scope));
    }
    return errors;
}

TypeCheckErrors OperatorIf::checkTypes() const
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
    for (const IOperator & oper : elseOperatorsList)
    {
        errors.add(oper.checkTypes());
    }
    return errors;
}