#pragma once
#include "IOperator.h"
#include "../expression/ExpressionFactory.h"

struct OperatorReturn : public IOperator
{
private:
    const SyntaxTree node;
    const std::string expectedReturnType;
    std::shared_ptr<IdentifiersScope> scope = nullptr;
public:
    const IExpression::ExpressionPtr expression;

    OperatorReturn(const std::string & returnType, const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};