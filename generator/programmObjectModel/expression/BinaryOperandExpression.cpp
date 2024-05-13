#include "BinaryOperandExpression.h"

BinaryOperandExpression::BinaryOperandExpression(const SyntaxTree & node)
    : BaseExpression(node),
    sign(getTokenValue(node.children[0])),
    operand1(ExpressionFactory::create(node.children[1])),
    operand2(ExpressionFactory::create(node.children[2])) {}
            
std::string BinaryOperandExpression::getType() const
{
    auto operand1Type = operand1.getType();
    auto operand2Type = operand2.getType();

    if ((sign == "||" || sign == "&&"))
    {
        TypeCheckErrors errors;
        if (operand1Type != "bool")
        {
            errors.add(InvalidTypeException(operand1.getNode(), operand1Type, "bool"));
        }
        if (operand2Type != "bool")
        {
            errors.add(InvalidTypeException(operand2.getNode(), operand2Type, "bool"));
        }
        if (errors.isEmpty())
        {
            return "bool";
        }
        throw errors;
    }
    if (sign == "<" || sign == "<=" || sign == ">" || sign == ">=")
    {
        TypeCheckErrors errors;
        if (!isOperandArithmetic(operand1Type))
        {
            errors.add(InvalidTypeException(operand1.getNode(), operand1Type, "int|float"));
        }
        if (!isOperandArithmetic(operand2Type))
        {
            errors.add(InvalidTypeException(operand2.getNode(), operand2Type, "int|float"));
        }
        if (errors.isEmpty())
        {
            return "bool";
        }
        throw errors;
    }
    if ((sign == "+" || sign == "-" || sign == "*" || sign == "/") && isOperandArithmetic)
    {
        TypeCheckErrors errors;
        if (!isOperandArithmetic(operand1Type))
        {
            errors.add(InvalidTypeException(operand1.getNode(), operand1Type, "int|float"));
        }
        if (!isOperandArithmetic(operand2Type))
        {
            errors.add(InvalidTypeException(operand2.getNode(), operand2Type, "int|float"));
        }
        if (errors.isEmpty())
        {
            if (operand1Type == "float" || operand2Type == "float" || sign == "/")
            {
                return "float";
            }
            return "int";
        }
        throw errors;
    }
    throw std::exception();
}

TypeCheckErrors BinaryOperandExpression::checkTypes() const
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