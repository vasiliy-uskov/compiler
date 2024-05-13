#include "UnaryOperandExpression.h"

UnaryOperandExpression::UnaryOperandExpression(const SyntaxTree & node)
    : BaseExpression(node),
    sign(getTokenValue(node.children[0])),
    operand(ExpressionFactory::create(node.children[1])) {}

std::string UnaryOperandExpression::getType() const
{
    auto operandType = operand.getType();
    if (sign == "!")
    {
        if (operandType == "bool")
        {
            return "bool";
        }
        throw TypeCheckErrors({InvalidTypeException(operand.getNode(), operandType, "bool")});
    }
    if (sign == "-")
    { 
        if (isOperandArithmetic(operandType))
        {
            return operandType;
        }
        throw TypeCheckErrors({InvalidTypeException(operand.getNode(), operandType, "int|float")});
    }
    throw std::exception();
}

TypeCheckErrors UnaryOperandExpression::checkTypes() const
{
    try
    {
        getType();
    }
    catch (const TypeCheckErrors & errors)
    {
        return errors;
    }
}