#include "ExpressionFactory.h"
#include "FunctionCall.h"
#include "Variable.h"
#include "RawValue.h"
#include "BinaryOperandExpression.h"
#include "UnaryOperandExpression.h"

IExpression::ExpressionPtr ExpressionFactory::create(const SyntaxTree & node)
{
    if (node.rule != SyntaxRule::Expression)
    {
        switch (node.rule)
        {
            case SyntaxRule::Identifier:
                return std::make_shared<Variable>(node);
            case SyntaxRule::FunctionCall:
                return std::make_shared<FunctionCall>(node);
            case SyntaxRule::IntValue:
            case SyntaxRule::FloatValue:
            case SyntaxRule::Keyword:
                return std::make_shared<RawValue>(node);
        }
    }
    if (node.children.size() == 3)
    {
        return std::make_shared<BinaryOperandExpression>(node);
    }
    if (node.children.size() == 2)
    {
        return std::make_shared<UnaryOperandExpression>(node);
    }
    if (node.children.size() == 1)
    {
        return ExpressionFactory::create(node.children[0]);
    }
    throw "Unexpected expression type";
}

std::vector<IExpression::ExpressionPtr> ExpressionFactory::createList(const SyntaxTree & list)
{
    return buildList<IExpression::ExpressionPtr>(list, ExpressionFactory::create);
}