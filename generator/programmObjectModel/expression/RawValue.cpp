#include "RawValue.h"

RawValue::RawValue(const SyntaxTree & node)
    : BaseExpression(node), type(getRawValueType(node)) {}

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