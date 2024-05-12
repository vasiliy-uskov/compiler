#pragma once
#include <vector>
#include <optional>
#include <memory>
#include "common.h"
#include "Operator.h"
#include "Operator.h"
#include "FunctionDefinition.h"
#include "identifiersScope/IdentifiersScope.h"

struct Programm
{
private:
    std::shared_ptr<IdentifiersScope> scope = nullptr;
    std::optional<std::vector<FunctionDefinition>> functions;
    std::vector<IOperator> operators;

    void forEachNode(const std::function<void(INode & node)>&);
public:
    Programm(const SyntaxTree & programNode);

    TypeCheckErrors initIdentifiersScope();
    TypeCheckErrors checkTypes();
};