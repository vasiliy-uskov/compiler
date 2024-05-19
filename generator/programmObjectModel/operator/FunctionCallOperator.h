#pragma once
#include "IOperator.h"

struct FunctionCallOperator : public IOperator 
{
private:
    FunctionCall expression;
public:
    FunctionCallOperator(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};