#include "RawValue.h"

RawValue::RawValue(const SyntaxTree & _node)
    : BaseExpression(_node), type(getRawValueType(_node)) {}

std::string RawValue::getRawValueType(const SyntaxTree & node)
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
    throw "Unexpected row value";
}

std::string RawValue::getType() const
{
    return type;
}

TypeCheckErrors RawValue::checkTypes() const
{
    return {};
}