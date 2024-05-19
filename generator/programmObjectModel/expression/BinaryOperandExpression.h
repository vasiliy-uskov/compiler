#include "BaseExpression.h"
#include "ExpressionFactory.h"

struct BinaryOperandExpression : public BaseExpression
{
    const std::string sign;
    const IExpression::ExpressionPtr operand1;
    const IExpression::ExpressionPtr operand2;

    BinaryOperandExpression(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
};