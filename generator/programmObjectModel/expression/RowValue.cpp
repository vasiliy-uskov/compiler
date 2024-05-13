#include "RowValue.h"

RowValue::RowValue(const SyntaxTree & node)
    : BaseExpression(node),
    type(getRowValueType(node)) {}

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

std::string RowValue::getType() const
{
    return type;
}

TypeCheckErrors RowValue::checkTypes() const
{
    return {};
}