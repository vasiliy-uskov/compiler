#pragma once
#include "IOperator.h"
#include "../expression/ExpressionFactory.h"
#include "../expression/Variable.h"

struct AssignmentOperator : public IOperator
{
private:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;
    Variable variable;
    IExpression::ExpressionPtr expression;
public:
    AssignmentOperator(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};