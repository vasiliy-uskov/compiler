#include "Operator.h"

OperatorIf::OperatorIf(const SyntaxTree & node)
    : expression(createExpression(node.children[1])),
    operatorsList(buildList<IOperator>(node.children[2], creaetOperator)),
    elseOperatorsList(node.children.size() > 3
        ? std::optional(buildList<IOperator>(node.children[4], creaetOperator))
        : std::nullopt){}

OperatorWhile::OperatorWhile(const SyntaxTree & node)
    : expression(createExpression(node.children[1])),
    operatorsList(buildList<IOperator>(node.children[2], creaetOperator)) {}

OperatorReturn::OperatorReturn(const SyntaxTree & node)
    : expression(createExpression(node.children[1])) {}

AssignmentOperator::AssignmentOperator(const SyntaxTree & node)
    : variable(Variable(node.children[0])),
    expression(createExpression(node.children[2])) {}

VariableDefinitionOperator::VariableDefinitionOperator(const SyntaxTree & node)
    : type(getTokenValue(node.children[0])),
    identifier(getTokenValue(node.children[1])),
    expression(node.children.size() > 2
        ? std::optional(createExpression(node.children[3]))
        : std::nullopt) {}

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