#include "OperatorReturn.h"

OperatorReturn::OperatorReturn(const std::string & returnType, const SyntaxTree & node)
    : node(node),
    expectedReturnType(returnType),
    expression(ExpressionFactory::create(node.children[1])) {}

TypeCheckErrors OperatorReturn::initIdentifiersScope(const std::shared_ptr<IdentifiersScope> & _scope)
{
    scope = _scope;
    return {};
}
TypeCheckErrors OperatorReturn::checkTypes() const
{
    TypeCheckErrors errors;
        try
    {
        auto realType = expression.getType();
        if (realType != expectedReturnType)
        {   
            errors.add(InvalidTypeException(expression.getNode(), realType, expectedReturnType));
        }
    }
    catch (TypeCheckErrors e)
    {
        errors.add(e);
    }
    return errors;
}