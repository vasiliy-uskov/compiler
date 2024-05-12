#pragma once
#include <vector>
#include <optional>
#include <memory>
#include "common.h"
#include "Expression.h"

struct IOperator : public INode {};

struct OperatorIf : public IOperator
{
private:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;
    
    IExpression expression;
    std::vector<IOperator> operatorsList;
    std::vector<IOperator> elseOperatorsList;
public:

    OperatorIf(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};

struct OperatorWhile : public IOperator
{
private:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;

    IExpression expression;
    std::vector<IOperator> operatorsList;
public:

    OperatorWhile(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};

struct OperatorReturn : public IOperator
{
private:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;
public:
    const IExpression expression;

    OperatorReturn(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};

struct AssignmentOperator : public IOperator
{
private:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;
public:
    const Variable variable;
    const IExpression expression;

    AssignmentOperator(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    TypeCheckErrors checkTypes() const override;
};

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

struct FunctionCallOperator : public FunctionCall, public IOperator
{
    using FunctionCall::FunctionCall;
};

IOperator creaetOperator(const SyntaxTree & node);