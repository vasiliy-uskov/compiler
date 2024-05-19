#pragma once
#include "IOperator.h"
#include "OperatorFactory.h"
#include "../expression/ExpressionFactory.h"

struct OperatorWhile : public IOperator
{
private:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;

    IExpression::ExpressionPtr expression;
    std::vector<IOperator::OperatorPtr> operatorsList;

protected:
    OperatorWhile(const std::string & functionReturnType, const SyntaxTree & node);
public:

    OperatorWhile(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};

struct FunctionOperatorWhile : public OperatorWhile
{
    FunctionOperatorWhile(const std::string & functionReturnType, const SyntaxTree & node)
        : OperatorWhile(functionReturnType, node) {}
};