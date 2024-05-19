#include "BaseExpression.h"
#include "ExpressionFactory.h"

struct UnaryOperandExpression : public BaseExpression
{
    const std::string sign;
    const IExpression::ExpressionPtr operand;

    UnaryOperandExpression(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
};