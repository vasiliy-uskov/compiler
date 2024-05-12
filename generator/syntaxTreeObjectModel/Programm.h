#pragma once
#include <vector>
#include <optional>
#include "common.h"
#include "Operator.h"
#include "FunctionDefinition.h"

struct Programm
{
    const std::optional<std::vector<FunctionDefinition>> functions;
    const std::vector<IOperator> operators;

    Programm(const SyntaxTree & programNode);
};