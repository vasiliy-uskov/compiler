#include "BaseExpression.h"

struct RowValue : public BaseExpression
{
private:
    const std::string type;

    static std::string getRowValueType(const SyntaxTree & node);
public:
    RowValue(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};