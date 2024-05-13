#include "OperatorFactory.h"
#include "OperatorIf.h"
#include "OperatorWhile.h"
#include "OperatorReturn.h"
#include "AssignmentOperator.h"
#include "VariableDefinitionOperator.h"

IOperator OperatorFactory::create(const SyntaxTree & node)
{
    switch (node.rule)
    {
    case SyntaxRule::OperatorIf:
        return OperatorIf(node);
    case SyntaxRule::OperatorWhile:
        return OperatorWhile(node);
    case SyntaxRule::AssignmentOperator:
        return AssignmentOperator(node);
    case SyntaxRule::FunctionCall:
        return FunctionCallOperator(node);
    case SyntaxRule::VariableDefinition:
        return VariableDefinitionOperator(node);
    }
    throw std::exception();
}

IOperator OperatorFactory::createFunctionOperator(const std::string & functionReturnType, const SyntaxTree & node)
{
    switch (node.rule)
    {
    case SyntaxRule::OperatorIf:
        return FunctionOperatorIf(functionReturnType, node);
    case SyntaxRule::OperatorWhile:
        return FunctionOperatorWhile(functionReturnType, node);
    case SyntaxRule::OperatorReturn:
        return OperatorReturn(functionReturnType, node);
    case SyntaxRule::AssignmentOperator:
        return AssignmentOperator(node);
    case SyntaxRule::FunctionCall:
        return FunctionCallOperator(node);
    case SyntaxRule::VariableDefinition:
        return VariableDefinitionOperator(node);
    }
    throw std::exception();
}

std::vector<IOperator> OperatorFactory::createList(const SyntaxTree & list)
{
    return buildList<IOperator>(list, OperatorFactory::create);
}

std::vector<IOperator> OperatorFactory::createFunctionOperatorsList(const std::string & functionReturnType, const SyntaxTree & list)
{
    return buildList<IOperator>(list, [functionReturnType](const SyntaxTree & node) {
        return OperatorFactory::createFunctionOperator(functionReturnType, node);
    });
}