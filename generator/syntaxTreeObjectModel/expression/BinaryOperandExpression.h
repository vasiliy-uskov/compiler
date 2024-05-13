#include "BaseExpression.h"
#include "ExpressionFactory.h"

struct BinaryOperandExpression : public BaseExpression
{
    const std::string sign;
    const IExpression operand1;
    const IExpression operand2;

    BinaryOperandExpression(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};