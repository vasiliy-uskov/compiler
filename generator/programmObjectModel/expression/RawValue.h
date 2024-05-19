#include "BaseExpression.h"

struct RawValue : public BaseExpression
{
private:
    const std::string type;

    static std::string getRawValueType(const SyntaxTree & node);
public:
    RawValue(const SyntaxTree & node);
    std::string getType() const override;
    TypeCheckErrors checkTypes() const override;
};