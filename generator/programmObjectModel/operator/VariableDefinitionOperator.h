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
    const IExpression::ExpressionPtr expression;

    ~VariableDefinitionOperator() {}
    VariableDefinitionOperator(const SyntaxTree & node);
    virtual TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    virtual TypeCheckErrors checkTypes() const override;
};