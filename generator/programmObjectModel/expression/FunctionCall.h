#include "BaseExpression.h"
#include "ExpressionFactory.h"

struct FunctionCall : public BaseExpression
{
    const std::string identifier;
    const std::vector<IExpression> arguments;

    explicit FunctionCall(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};