#pragma once
#include "IOperator.h"
#include "../expression/ExpressionFactory.h"

struct VariableDefinitionOperator : public IOperator
{
private:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;
public:
    const std::string type;
    const std::string identifier;
    const std::optional<IExpression> expression;

    VariableDefinitionOperator(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};