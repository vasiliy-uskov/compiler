#pragma once
#include "common.h"
#include "Operator.h"

struct FunctionDefinitionArgument
{
    const std::string identifier;
    const std::string type;

    FunctionDefinitionArgument(const SyntaxTree & argumentNode);
};

struct FunctionDefinition
{
    const std::string identifier;
    const std::string returnType;
    const std::vector<FunctionDefinitionArgument> arguments;
    const std::vector<IOperator> operatorsList;

    FunctionDefinition(const SyntaxTree & functionNode);
};