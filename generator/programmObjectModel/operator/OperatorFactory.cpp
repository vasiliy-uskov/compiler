#include "OperatorFactory.h"
#include "OperatorIf.h"
#include "OperatorWhile.h"
#include "OperatorReturn.h"
#include "AssignmentOperator.h"
#include "VariableDefinitionOperator.h"
#include "FunctionCallOperator.h"

IOperator::OperatorPtr OperatorFactory::create(const SyntaxTree & node)
{
    switch (node.rule)
    {
    case SyntaxRule::OperatorIf:
        return std::make_shared<OperatorIf>(node);
    case SyntaxRule::OperatorWhile:
        return std::make_shared<OperatorWhile>(node);
    case SyntaxRule::AssignmentOperator:
        return std::make_shared<AssignmentOperator>(node);
    case SyntaxRule::FunctionCall:
        return std::make_shared<FunctionCallOperator>(node);
    case SyntaxRule::VariableDefinition:
        return std::make_shared<VariableDefinitionOperator>(node);
    }
    throw "Unexpected operator type";
}

IOperator::OperatorPtr OperatorFactory::createFunctionOperator(const std::string & functionReturnType, const SyntaxTree & node)
{
    switch (node.rule)
    {
    case SyntaxRule::OperatorIf:
        return std::make_shared<FunctionOperatorIf>(functionReturnType, node);
    case SyntaxRule::OperatorWhile:
        return std::make_shared<FunctionOperatorWhile>(functionReturnType, node);
    case SyntaxRule::OperatorReturn:
        return std::make_shared<OperatorReturn>(functionReturnType, node);
    case SyntaxRule::AssignmentOperator:
        return std::make_shared<AssignmentOperator>(node);
    case SyntaxRule::FunctionCall:
        return std::make_shared<FunctionCallOperator>(node);
    case SyntaxRule::VariableDefinition:
        return std::make_shared<VariableDefinitionOperator>(node);
    }
    throw "Unexpected function operator type";
}

std::vector<IOperator::OperatorPtr> OperatorFactory::createList(const SyntaxTree & list)
{
    return buildList<IOperator::OperatorPtr>(list, OperatorFactory::create);
}

std::vector<IOperator::OperatorPtr> OperatorFactory::createFunctionOperatorsList(const std::string & functionReturnType, const SyntaxTree & list)
{
    return buildList<IOperator::OperatorPtr>(list, [functionReturnType](const SyntaxTree & node) {
        return OperatorFactory::createFunctionOperator(functionReturnType, node);
    });
}