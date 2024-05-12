#pragma once
#include <functional>
#include <memory>
#include "common.h"

struct IExpression : public INode
{
    virtual SyntaxTree getNode() const;
    virtual std::string getType() const;
};

struct BaseExpression : public IExpression
{
protected:
    const SyntaxTree node;
    std::shared_ptr<IdentifiersScope> scope = nullptr;
public:
    BaseExpression(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    SyntaxTree getNode() const override;
};

struct RowValue : public BaseExpression
{
private:
    const std::string type;

    static std::string getRowValueType(const SyntaxTree & node);
public:
    RowValue(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};

struct Variable : public BaseExpression
{
    const std::string identifier;

    Variable(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};

struct FunctionCall : public BaseExpression
{
    const std::string identifier;
    const std::vector<IExpression> arguments;

    explicit FunctionCall(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};

struct UnaryOperandExpression : public BaseExpression
{
    const std::string sign;
    const IExpression operand;

    UnaryOperandExpression(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};

struct BinaryOperandExpression : public BaseExpression
{
    const std::string sign;
    const IExpression operand1;
    const IExpression operand2;

    BinaryOperandExpression(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};

IExpression createExpression(const SyntaxTree & node);