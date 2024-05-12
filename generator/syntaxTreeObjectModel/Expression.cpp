#include "Expression.h"

bool isOperandArithmetic(const std::string & operandType)
{
    return operandType == "int" || operandType == "float";
}

std::string RowValue::getRowValueType(const SyntaxTree & node)
{
    switch (node.rule)
    {
    case SyntaxRule::IntValue:
        return "int";
    case SyntaxRule::FloatValue:
        return "float";
    case SyntaxRule::Keyword:
        return "bool";
    }
    throw std::exception();
}

RowValue::RowValue(const SyntaxTree & node)
    : type(getRowValueType(node.children[1])) {}

std::string RowValue::getType(const IScopeStorage & storage) const
{
    return type;
}

Variable::Variable(const SyntaxTree & node)
    : identifier(getTokenValue(node.children[1])) {}

std::string Variable::getType(const IScopeStorage & storage) const
{
    return storage.getVariableType(identifier);
}

FunctionCall::FunctionCall(const SyntaxTree & node)
    : identifier(getTokenValue(node.children[0])),
        arguments(buildList<IExpression>(node.children[1], createExpression)) {};
        
std::string FunctionCall::getType(const IScopeStorage & storage) const
{
    return storage.getFunctionReturnType(identifier);
}

UnaryOperandExpression::UnaryOperandExpression(const SyntaxTree & node)
    : sign(getTokenValue(node.children[0])),
        operand(createExpression(node.children[1])) {}
        
std::string UnaryOperandExpression::getType(const IScopeStorage & storage) const
{
    auto operandType = operand.getType(storage);
    if (sign == "!" && operandType == "bool")
    {
        return "bool";
    }
    if (sign == "-" && isOperandArithmetic(operandType))
    { 
        return operandType;
    }
    throw std::exception();
}

BinaryOperandExpression::BinaryOperandExpression(const SyntaxTree & node)
    : sign(getTokenValue(node.children[0])),
    operand1(createExpression(node.children[1])),
    operand2(createExpression(node.children[2])) {}
            
std::string BinaryOperandExpression::getType(const IScopeStorage & storage) const
{
    auto operand1Type = operand1.getType(storage);
    auto operand2Type = operand2.getType(storage);
    auto isOperandsBool = (operand1Type == "bool" && operand2Type == "bool");
    auto isOperandsAtithmetic = isOperandArithmetic(operand1Type) && isOperandArithmetic(operand2Type);

    if ((sign == "||" || sign == "&&") && isOperandsBool
        || (sign == "<" || sign == "<=" || sign == ">" || sign == ">=") && isOperandsAtithmetic)
    {
        return "bool";
    }
    if ((sign == "+" || sign == "-" || sign == "*") && isOperandArithmetic)
    {
        if (operand1Type == "float" || operand2Type == "float")
        {
            return "float";
        }
        return "int";
    }
    if (sign == "/" && isOperandArithmetic)
    {
        return "float";
    }
    throw std::exception();
}

IExpression createExpression(const SyntaxTree & node)
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