#pragma once
#include <functional>
#include "common.h"

struct IScopeStorage
{
    virtual std::string getFunctionReturnType(const std::string & identifier) const;
    virtual std::string getVariableType(const std::string & identifier) const;
};

struct IExpression
{
    virtual std::string getType(const IScopeStorage&) const;
};

struct RowValue : public IExpression
{
private:
    const std::string type;

    static std::string getRowValueType(const SyntaxTree & node);
public:
    RowValue(const SyntaxTree & node);
    std::string getType(const IScopeStorage & storage) const override;
};

struct Variable : public IExpression
{
    const std::string identifier;

    Variable(const SyntaxTree & node);
    std::string getType(const IScopeStorage & storage) const override;
};

struct FunctionCall : public IExpression
{
    const std::string identifier;
    const std::vector<IExpression> arguments;

    explicit FunctionCall(const SyntaxTree & node);
    std::string getType(const IScopeStorage & storage) const override;
};

struct UnaryOperandExpression : IExpression
{
    const std::string sign;
    const IExpression operand;

    UnaryOperandExpression(const SyntaxTree & node);
    std::string getType(const IScopeStorage & storage) const override;
};

struct BinaryOperandExpression : public IExpression
{
    const std::string sign;
    const IExpression operand1;
    const IExpression operand2;

    BinaryOperandExpression(const SyntaxTree & node);
    std::string getType(const IScopeStorage & storage) const override;
};

IExpression createExpression(const SyntaxTree & node);