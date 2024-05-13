#pragma once
#include <functional>
#include <memory>
#include "../../../parser/grammar/parserEngin/syntaxTree/SyntaxTree.h"
#include "../common.h"

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
    static bool isOperandArithmetic(const std::string & operandType);
public:
    BaseExpression(const SyntaxTree & node);
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
    SyntaxTree getNode() const override;
};