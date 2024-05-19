#include "BaseExpression.h"
#include "ExpressionFactory.h"

struct FunctionCall : public BaseExpression
{
    const std::string identifier;
    const std::vector<IExpression::ExpressionPtr> arguments;

    FunctionCall(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
    TypeCheckErrors initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & scope) override;
};