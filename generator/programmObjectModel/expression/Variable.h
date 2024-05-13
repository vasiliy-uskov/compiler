#include "BaseExpression.h"

struct Variable : public BaseExpression
{
    const std::string identifier;

    Variable(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};