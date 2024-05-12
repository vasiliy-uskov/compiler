#pragma once
#include <vector>
#include <optional>
#include "common.h"
#include "Expression.h"

struct IOperator
{
    
};

struct OperatorIf : public IOperator
{
    const IExpression expression;
    const std::vector<IOperator> operatorsList;
    const std::optional<std::vector<IOperator>> elseOperatorsList;

    OperatorIf(const SyntaxTree & node);
};

struct OperatorWhile : public IOperator
{
    const IExpression expression;
    const std::vector<IOperator> operatorsList;

    OperatorWhile(const SyntaxTree & node);
};

struct OperatorReturn : public IOperator
{
    const IExpression expression;

    OperatorReturn(const SyntaxTree & node);
};

struct AssignmentOperator : public IOperator
{
    const Variable variable;
    const IExpression expression;

    AssignmentOperator(const SyntaxTree & node);
};

struct VariableDefinitionOperator : public IOperator
{
    const std::string type;
    const std::string identifier;
    const std::optional<IExpression> expression;

    VariableDefinitionOperator(const SyntaxTree & node);
};

struct FunctionCallOperator : public FunctionCall, public IOperator
{
    using FunctionCall::FunctionCall;
};

IOperator creaetOperator(const SyntaxTree & node);