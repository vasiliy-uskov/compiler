#pragma once
#include <vector>
#include <optional>
#include <memory>
#include "common.h"
#include "../expression/ExpressionFactory.h"
#include "OperatorFactory.h"
#include "IOperator.h"

struct OperatorIf : public IOperator
{
private:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;
    
    IExpression expression;
    std::vector<IOperator> operatorsList;
    std::vector<IOperator> elseOperatorsList;

protected:
    OperatorIf(const std::string & functionReturnType, const SyntaxTree & node);
public:

    OperatorIf(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};

struct FunctionOperatorIf : public OperatorIf
{
    FunctionOperatorIf(const std::string & functionReturnType, const SyntaxTree & node)
        : OperatorIf(functionReturnType, node) {}
};