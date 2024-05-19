#pragma once
#include <vector>
#include <optional>
#include <memory>
#include "INode.h"
#include "buildList.h"
#include "FunctionDefinition.h"
#include "identifiersScope/IdentifiersScope.h"
#include "operator/OperatorFactory.h"

struct Programm
{
private:
    std::shared_ptr<IdentifiersScope> scope = nullptr;
    std::optional<std::vector<FunctionDefinition>> functions;
    std::vector<IOperator::OperatorPtr> operators;

    void forEachNode(const std::function<void(INode & node)>&);
public:
    Programm(const SyntaxTree & programNode);

    TypeCheckErrors initIdentifiersScope();
    TypeCheckErrors checkTypes();
};