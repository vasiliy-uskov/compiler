#include "Operator.h"

void checkExpressionType(TypeCheckErrors & errors, const IExpression & expression, const std::string & expectedType)
{
    try
    {
        auto realType = expression.getType();
        if (realType != expectedType)
        {   
            errors.add(InvalidTypeException(expression.getNode(), realType, expectedType));
        }
    }
    catch (TypeCheckErrors e)
    {
        errors.add(e);
    }
}

OperatorIf::OperatorIf(const SyntaxTree & node)
    : node(node),
    expression(createExpression(node.children[1])),
    operatorsList(buildList<IOperator>(node.children[2], creaetOperator)),
    elseOperatorsList(node.children.size() > 3
        ? buildList<IOperator>(node.children[4], creaetOperator)
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
    checkExpressionType(errors, expression, "bool");

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

OperatorWhile::OperatorWhile(const SyntaxTree & node)
    : node(node),
    expression(createExpression(node.children[1])),
    operatorsList(buildList<IOperator>(node.children[2], creaetOperator)) {}
        
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
    checkExpressionType(errors, expression, "bool");
    for (const IOperator & oper : operatorsList)
    {
        errors.add(oper.checkTypes());
    }
    return errors;
}

OperatorReturn::OperatorReturn(const SyntaxTree & node)
    : node(node),
    expression(createExpression(node.children[1])) {}

TypeCheckErrors OperatorReturn::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    return {};
}
TypeCheckErrors OperatorReturn::checkTypes() const
{
    TypeCheckErrors errors;
    // TODO:: checkExpressionType(errors, expression, expectedType);
    return errors;
}

AssignmentOperator::AssignmentOperator(const SyntaxTree & node)
    : node(node),
    variable(Variable(node.children[0])),
    expression(createExpression(node.children[2])) {}

TypeCheckErrors AssignmentOperator::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    return {};
}
TypeCheckErrors AssignmentOperator::checkTypes() const
{
    TypeCheckErrors errors;
    try
    {
        auto variableType = variable.getType();
        auto expressionType = expression.getType();
        if (variableType != expressionType)
        {
            return TypeCheckErrors({InvalidTypeException(expression.getNode(), expressionType, variableType)});
        }
    }
    catch(TypeCheckErrors e)
    {
        return e;
    }
    
    return errors;
}

VariableDefinitionOperator::VariableDefinitionOperator(const SyntaxTree & node)
    : node(node),
    type(getTokenValue(node.children[0])),
    identifier(getTokenValue(node.children[1])),
    expression(node.children.size() > 2
        ? std::optional(createExpression(node.children[3]))
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
        checkExpressionType(errors, expression.value(), type);
    }
    return errors;
}

IOperator creaetOperator(const SyntaxTree & node)
{
    switch (node.rule)
    {
    case SyntaxRule::OperatorIf:
        return OperatorIf(node);
    case SyntaxRule::OperatorWhile:
        return OperatorWhile(node);
    case SyntaxRule::OperatorReturn:
        return OperatorReturn(node);
    case SyntaxRule::AssignmentOperator:
        return AssignmentOperator(node);
    case SyntaxRule::FunctionCall:
        return FunctionCallOperator(node);
    }
    throw std::exception();
}