#pragma once
#include <memory>
#include "INode.h"
#include "buildList.h"
#include "operator/OperatorFactory.h"

struct FunctionDefinitionArgument
{
    const SyntaxTree node;
    const std::string identifier;
    const std::string type;

    FunctionDefinitionArgument(const SyntaxTree & argumentNode);
};

struct FunctionDefinition : public INode
{
private:
    std::shared_ptr<IdentifiersScope> scope = nullptr;
    std::vector<IOperator> operatorsList;
public:
    const SyntaxTree node;
    const std::string identifier;
    const std::string returnType;
    const std::vector<FunctionDefinitionArgument> arguments;

    FunctionDefinition(const SyntaxTree & functionNode);

    TypeCheckErrors checkTypes() const override;
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
};