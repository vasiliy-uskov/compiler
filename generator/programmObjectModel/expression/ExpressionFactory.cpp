#include "ExpressionFactory.h"
#include "FunctionCall.h"
#include "Variable.h"
#include "RowValue.h"
#include "BinaryOperandExpression.h"
#include "UnaryOperandExpression.h"

IExpression ExpressionFactory::create(const SyntaxTree & node)
{
    if (node.rule != SyntaxRule::Expression)
    {
        switch (node.rule)
        {
            case SyntaxRule::Identifier:
                return Variable(node);
            case SyntaxRule::FunctionCall:
                return FunctionCall(node);
            case SyntaxRule::IntValue:
            case SyntaxRule::FloatValue:
            case SyntaxRule::Keyword:
                return RowValue(node);
        }
    }
    if (node.children.size() == 3)
    {
        return BinaryOperandExpression(node);
        
    }
    if (node.children.size() == 2)
    {
        return UnaryOperandExpression(node);
    }
    throw std::exception();
}

std::vector<IExpression> ExpressionFactory::createList(const SyntaxTree & list)
{
    return buildList<IExpression>(list, ExpressionFactory::create);
}