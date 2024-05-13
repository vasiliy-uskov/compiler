#include "BaseExpression.h"
#include "ExpressionFactory.h"

struct UnaryOperandExpression : public BaseExpression
{
    const std::string sign;
    const IExpression operand;

    UnaryOperandExpression(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};