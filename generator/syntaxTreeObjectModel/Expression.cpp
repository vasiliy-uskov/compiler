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

BaseExpression::BaseExpression(const SyntaxTree & node) : node(node) {}

TypeCheckErrors BaseExpression::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    return {};
}
SyntaxTree BaseExpression::getNode() const
{
    return node;
}

RowValue::RowValue(const SyntaxTree & node)
    : BaseExpression(node),
    type(getRowValueType(node)) {}

std::string RowValue::getType() const
{
    return type;
}

TypeCheckErrors RowValue::checkTypes() const
{
    return {};
}

Variable::Variable(const SyntaxTree & node)
    : BaseExpression(node),
    identifier(getTokenValue(node)) {}

std::string Variable::getType() const
{
    try
    {
        return scope->getVariableType(identifier);
    }
    catch(IdentifiersScope::UnknownIdentifierException e)
    {
        throw TypeCheckErrors({UnknownVariableException(node)});
    }
}

TypeCheckErrors Variable::checkTypes() const
{
    try
    {
        getType();
    }
    catch(TypeCheckErrors e)
    {
        return e;
    }
}

FunctionCall::FunctionCall(const SyntaxTree & node)
    : BaseExpression(node),
    identifier(getTokenValue(node.children[0])),
    arguments(buildList<IExpression>(node.children[1], createExpression)) {}
        
std::string FunctionCall::getType() const
{
    auto errors = checkTypes();
    if (errors.isEmpty())
    {
        try
        {    
            return scope->getFunctionSignature(identifier).first;
        }
        catch(IdentifiersScope::UnknownIdentifierException e)
        {
            errors.add(UnknownFunctionException(node));
        }
        
    }
    throw errors;
}

TypeCheckErrors FunctionCall::checkTypes() const
{
    try
    {
        auto expectingArguments = scope->getFunctionSignature(identifier).second;    
        auto argumentsCount = arguments.size();
        auto expectingArgumentsCount = expectingArguments.size();
        if (argumentsCount != expectingArgumentsCount)
        {
            return TypeCheckErrors({InvalidArgumentsCountExcpetion(node, argumentsCount, expectingArgumentsCount)});
        }
        TypeCheckErrors errors;
        for (size_t i = 0; i < arguments.size(); ++i)
        {
            try
            {
                auto actualType = arguments[i].getType();
                auto expectingType = expectingArguments[i];
                if (actualType != expectingType)
                {
                    errors.add(InvalidTypeException(node, actualType, expectingType));
                }
            }
            catch (const TypeCheckErrors & newErrors)
            {
                errors.add(newErrors);
            }
        }
        return errors;
    }
    catch(IdentifiersScope::UnknownIdentifierException e)
    {
        return TypeCheckErrors({UnknownFunctionException(node)});
    }
}

UnaryOperandExpression::UnaryOperandExpression(const SyntaxTree & node)
    : BaseExpression(node),
    sign(getTokenValue(node.children[0])),
    operand(createExpression(node.children[1])) {}
        
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

BinaryOperandExpression::BinaryOperandExpression(const SyntaxTree & node)
    : BaseExpression(node),
    sign(getTokenValue(node.children[0])),
    operand1(createExpression(node.children[1])),
    operand2(createExpression(node.children[2])) {}
            
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